#include "filetransformat.h"
#include <QDateTime>
#include <QDir>
#include <cmath>
#include <QDebug>

void FileTransFormat::setSourceFilePath(QString path)
{
    _file_src_path = path;
}
int FileTransFormat::getSourceFileSize()
{
    return _file_src_len;
}
void FileTransFormat::setFormatMethod(int method)
{
    _format_method = method;
}

FileTransFormat::FileTransFormat(QObject *parent) : QObject(parent),
    _file_trans_percent({0,0})
{

}

void FileTransFormat::reset()
{
    if (_file_src.isOpen()) {
        _file_src.close();
    }
    _file_src_path.clear();
    _file_src_data.clear();
    _file_src_len = 0;
    _format_method = 0;
    _file_trans_percent[0] = 0;
    _file_trans_percent[1] = 0;
    for (int i = 0; i < max_multi_dstfilelen; i++) {
        if (_file_dst[i].isOpen()) {
            _file_dst[i].close();
        }
        _file_dst_txtstream[i].reset();
        _file_dst_txtstream[i].resetStatus();
    }
}

uint8_t FileTransFormat::format_usr1_hexascii_to_byte(const char *p)
{
    uint8_t high4bit = 0;
    uint8_t low4bit = 0;
    bool judge11 = p[0] >= '0' && p[0] <= '9';
    bool judge12 = p[0] >= 'a' && p[0] <= 'f';
    bool judge13 = p[0] >= 'A' && p[0] <= 'F';
    bool high4bit_judge = judge11 || judge12 || judge13;

    bool judge21 = p[1] >= '0' && p[1] <= '9';
    bool judge22 = p[1] >= 'a' && p[1] <= 'f';
    bool judge23 = p[1] >= 'A' && p[1] <= 'F';
    bool low4bit_judge =  judge21 || judge22 || judge23;

    // detect illegal char
    if (!high4bit_judge || !low4bit_judge) { return 0; }
    if (judge11) { high4bit = static_cast<unsigned char>(p[0] - '0'); }
    if (judge12) { high4bit = static_cast<unsigned char>(p[0] - 'a' + 10); }
    if (judge13) { high4bit = static_cast<unsigned char>(p[0] - 'A' + 10); }
    if (judge21) { low4bit = static_cast<unsigned char>(p[1] - '0'); }
    if (judge22) { low4bit = static_cast<unsigned char>(p[1] - 'a' + 10); }
    if (judge23) { low4bit = static_cast<unsigned char>(p[1] - 'A' + 10); }
    return static_cast<unsigned char>(high4bit << 4 | low4bit);
}

void FileTransFormat::fileidx_percent(int read_idx)
{
    int interval = 3;
    float a = static_cast<unsigned int>(read_idx) * 100.0f;
    float b = static_cast<unsigned int>(getSourceFileSize());
    _file_trans_percent[0] = static_cast<unsigned int>(a/b);
    if (_file_trans_percent[0] - _file_trans_percent[1] >= interval) {
        _file_trans_percent[1] = _file_trans_percent[0];
        emit progress_add(_file_trans_percent[0]);
    }
}

char FileTransFormat::getfile_buffer(int &read_idx, int &buffer_idx)
{
    char c = 0;
#ifndef BIGFILE_SUPPORT
    c = _file_src_data.at(read_idx);
#else
    if (getSourceFileSize() > bigfile_minlen) {
        if (buffer_idx >= bigfile_bufferlen) {
            buffer_idx = 0;
            filebigbuffer_update(read_idx);
        }
        c = _file_src_data.at(buffer_idx);
        buffer_idx++;
    } else {
        c = _file_src_data.at(read_idx);
    }
#endif
    return c;
}

void FileTransFormat::filebigbuffer_update(int ridx)
{
    int len = (getSourceFileSize() - ridx <= bigfile_bufferlen) ?
              (getSourceFileSize() - ridx) :
              bigfile_bufferlen;
    try {
        _file_src_data = _file_src.read(len);
    } catch (...) {
        qDebug() << "big buffer copy error";
    }
}

bool FileTransFormat::openSourceFile()
{
    if (_file_src_path.isEmpty())
        return false;

#ifndef BIGFILE_SUPPORT
    try {
        _file_src.setFileName(_file_src_path);
        if (!_file_src.open(QIODevice::ReadOnly)) {
            _file_src.close();
            return false;
        }
        _file_src_data = _file_src.readAll();
        _file_src_len = _file_src_data.size();
    } catch (std::bad_alloc) {
        qDebug() << "[format] open file error, too big";
        _file_src.close();
        return false;
    }
#else
    try {
        _file_src.setFileName(_file_src_path);
        if (!_file_src.open(QIODevice::ReadOnly)) {
            _file_src.close();
            return false;
        }
        _file_src_len = _file_src.size();
        if (getSourceFileSize() < bigfile_minlen) {
            _file_src_data = _file_src.readAll();
        } else {
            filebigbuffer_update(0);
        }
    } catch (std::bad_alloc) {
        qDebug() << "[format] open file error, too big";
        _file_src.close();
        return false;
    }
#endif
    return true;
}

int FileTransFormat::work()
{
    switch (_format_method) {
    case USR_1:
        format_usr1();
        break;
    case USR_2:
        format_usr2();
        break;
    case USR_3:
        format_usr3();
        break;
    default: break;
    }
    emit progredd_complete();
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// USER Pack1 Trans ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int FileTransFormat::format_usr1()
{
    qDebug() << "[format] enter";
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString time_suffix = QString("_["+now+"]");
    QString name1 = QString(_file_src_path.section(".",0,0) + time_suffix + "_RTK_RAW.txt");
    QString name2 = QString(_file_src_path.section(".",0,0) + time_suffix + "_RTK_BASE.txt");
    try {
        _file_dst[0].setFileName(name1);
        _file_dst[1].setFileName(name2);
        bool r1 = _file_dst[0].open(QIODevice::WriteOnly);
        bool r2 = _file_dst[1].open(QIODevice::WriteOnly);
        if (!r1 || !r2) {
            _file_dst[0].close();
            _file_dst[1].close();
            emit progress_fail(1);
            return -2;
        }
        _file_dst_binstream[0].setDevice(&_file_dst[0]);
        _file_dst_binstream[1].setDevice(&_file_dst[1]);
    } catch (std::bad_alloc) {
        qDebug() << "exception catch ";
    }

    qDebug() << "[format] open dstfile" << name1;
    qDebug() << "[format] open dstfile" << name2;

    int idx_read = 0;
    int idx_buffer = 0;
    QString msg;
    while (idx_read < getSourceFileSize()) {
        char c = getfile_buffer(idx_read, idx_buffer);
        idx_read++;
        msg.append(c);
        if (c == 0x0A) {
            if (msg.contains("$RTK_RAW", Qt::CaseSensitive)) {
                std::string str = msg.toStdString();
                int strlen = msg.size();
                int stridx = 9;
                uint8_t data_hex = 0;
                const char *msg_data;
                msg_data = str.c_str();
                for (; stridx < strlen; stridx += 2) {
                    if (msg_data[stridx] == '*') break;
                    data_hex = format_usr1_hexascii_to_byte(&msg_data[stridx]);
                    _file_dst_binstream[0].operator<<(data_hex);
                }
            }
            if (msg.contains("$RTK_BASE", Qt::CaseSensitive)) {
                std::string str = msg.toStdString();
                int strlen = msg.size();
                int stridx = 10;
                uint8_t data_hex = 0;
                const char *msg_data;
                msg_data = str.c_str();
                for (; stridx < strlen; stridx += 2) {
                    if (msg_data[stridx] == '*') break;
                    data_hex = format_usr1_hexascii_to_byte(&msg_data[stridx]);
                    _file_dst_binstream[1].operator<<(data_hex);
                }
            }
            msg.clear();
        }
        fileidx_percent(idx_read);
    }
    qDebug () << "[format] end format trans";
    _file_dst[0].close();
    _file_dst[1].close();
    _file_src.close();
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// USER Pack2 Trans ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int FileTransFormat::format_usr2()
{
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString time_suffix = QString("_["+now+"]");
    QString name = QString(_file_src_path.section(".",0,0) + time_suffix + ".txt");
    _file_dst[0].setFileName(name);
    qDebug() << "[format] enter";
    try {
        bool r1 = _file_dst[0].open(QIODevice::WriteOnly);
        if (!r1) {
            _file_dst[0].close();
            delete _file_dst;
            emit progress_fail(1);
            return -2;
        }
        _file_dst_txtstream[0].setDevice(_file_dst);
    } catch (std::bad_alloc) {
        qDebug() << "exception catch";
    }
    qDebug() << "[format] open dstfile" << name;

    struct __a {
        double GpsTime;
        int32_t GyroX;
        int32_t GyroY;
        int32_t GyroZ;
        int32_t AccelX;
        int32_t AccelY;
        int32_t AccelZ;
    } _ineral_imu;
    double time_lst = 0;
    int cnt_f = 0;
    int gaps = 0;

    int idx_read = 0;
    int idx_buffer = 0;
    QString msg;
    while (idx_read < getSourceFileSize()) {
        char c = getfile_buffer(idx_read, idx_buffer);
        idx_read++;
        msg.append(c);
        if (c == '\n') {
            if (msg.contains("$IMURAW", Qt::CaseSensitive)) {
                time_lst = _ineral_imu.GpsTime;
                std::string str = msg.toStdString();
                int strlen = msg.size();
                int stridx = 8;
                const char *msg_data;
                char *endp;
                msg_data = str.c_str();
                msg_data += stridx;
                if (msg_data && *(++msg_data) != ',') {
                    float gyroX = strtod(msg_data, &endp);
                    _ineral_imu.GyroX = static_cast<int32_t>(gyroX *1000000);
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    float gyroY = strtod(msg_data, &endp);
                    _ineral_imu.GyroY = static_cast<int32_t>(gyroY *1000000);
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    float gyroZ = strtod(msg_data, &endp);
                    _ineral_imu.GyroZ = static_cast<int32_t>(gyroZ *1000000);
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    float accelX = strtod(msg_data, &endp);
                    _ineral_imu.AccelX = static_cast<int32_t>(accelX *1000000);
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    float accelY = strtod(msg_data, &endp);
                    _ineral_imu.AccelY = static_cast<int32_t>(accelY *1000000);
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    float accelZ = strtod(msg_data, &endp);
                    _ineral_imu.AccelZ = static_cast<int32_t>(accelZ *1000000);
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    double startTime = strtod(msg_data, &endp);
                    // _ineral_imu.GpsTime = gpsTime;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    double gpsTime = strtod(msg_data, &endp);
                    _ineral_imu.GpsTime = gpsTime;
                    msg_data = endp;
                }
                //_ineral_imu.GpsTime = time_lst + 0.01f;
                if ((fabs(_ineral_imu.GpsTime - time_lst) > 0.015)) {
                    gaps++;
                }
                cnt_f++;
                int size = sizeof(_ineral_imu);
                _file_dst[0].write(reinterpret_cast<const char *>(&_ineral_imu), size);
            }
            msg.clear();
        }
        fileidx_percent(idx_read);
    }
    qDebug () << "[format] end format trans";

    _file_dst[0].close();
    _file_src.close();
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// USER Pack3 Trans ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
int FileTransFormat::format_usr3()
{
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString time_suffix = QString("_["+now+"]");
    QString name = QString(_file_src_path.section(".",0,0) + time_suffix + ".txt");
    _file_dst[0].setFileName(name);
    qDebug() << "[format] enter";
    try {
        bool r1 = _file_dst[0].open(QIODevice::WriteOnly);
        if (!r1) {
            _file_dst[0].close();
            delete _file_dst;
            emit progress_fail(1);
            return -2;
        }
        _file_dst_txtstream[0].setDevice(_file_dst);
    } catch (std::bad_alloc) {
        qDebug() << "exception catch";
    }
    qDebug() << "[format] open dstfile" << name;

    uint8_t accel_origin_t[6];
    uint8_t gyro_tmp_origin_t[8];

    int16_t accel_origin[3];
    int16_t gyro_tmp_origin[4];

    float accel[3];
    float gyro[3];
    double sample_timestamp = 0;

    int accel_scale = 0;
    int gyro_scale = 0;

    int is_begin = 0;
    int state_0x28_accel = -1;
    int state_0x20_tmp_gyro = -1;
    int state_0x11_ctrl_g = -1;
    int state_0x10_ctrl_xl = -1;

    int cnt_accel_0x28_origin = 0;
    int cnt_accel_0x28_frame = 0;
    int cnt_gyro_tmp_0x20_origin = 0;
    int cnt_gyro_tmp_0x20_frame = 0;

    int idx_read = 0;
    int idx_buffer = 0;
    QString msg;
    while (idx_read < getSourceFileSize()) {
        char c = getfile_buffer(idx_read, idx_buffer);
        idx_read++;
        msg.append(c);
        if (c == '\n') {
            if (msg.contains("0xD4,0x11,Write", Qt::CaseSensitive) && is_begin == 0 && gyro_scale == 0) {
                state_0x11_ctrl_g = 0;
                msg.clear();
                continue;
            }
            if (msg.contains("0xD4,0x10,Write", Qt::CaseSensitive) && is_begin == 0 && accel_scale == 0) {
                state_0x10_ctrl_xl = 0;
                msg.clear();
                continue;
            }
            if (msg.contains("0xD4,0x1E,Write", Qt::CaseSensitive) && is_begin == 0) {
                is_begin = 1;
            }
            if (msg.contains("0xD4,0x28,Write", Qt::CaseSensitive) && is_begin == 1) {
                state_0x28_accel = 0;
            }
            if (msg.contains("0xD4,0x20,Write", Qt::CaseSensitive) && is_begin == 1) {
                state_0x20_tmp_gyro = 0;
            }
            if (state_0x20_tmp_gyro >= 0 && state_0x28_accel >= 0) {
                qDebug() << "error seq";
            }
            if (msg.contains("0xD4", Qt::CaseSensitive) &&
                msg.contains("Write", Qt::CaseSensitive) &&
                state_0x11_ctrl_g >= 0 && state_0x10_ctrl_xl < 0) {
                std::string str = msg.toStdString();
                const char *msg_data;
                char *endp;
                msg_data = str.c_str();
                if (msg_data && *(msg_data) != ',') {
                    double sample_time = strtod(msg_data, &endp); (void)sample_time;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int cmd_cnt = strtol(msg_data, &endp, 10); (void)cmd_cnt;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int addr = strtol(msg_data, &endp, 16); (void)addr;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int data = strtol(msg_data, &endp, 16);
                    uint8_t scale_setting = ((uint8_t)data & 0x0C) >> 2;
                    gyro_scale = 250 << scale_setting;
                    msg_data = endp;
                }
                state_0x11_ctrl_g = -1;
                // qDebug() << "gyro scale" << gyro_scale;
            }

            if (msg.contains("0xD4", Qt::CaseSensitive) &&
                msg.contains("Write", Qt::CaseSensitive) &&
                state_0x10_ctrl_xl >= 0 && state_0x11_ctrl_g < 0) {
                std::string str = msg.toStdString();
                const char *msg_data;
                char *endp;
                msg_data = str.c_str();
                if (msg_data && *(msg_data) != ',') {
                    double sample_time = strtod(msg_data, &endp); (void)sample_time;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int cmd_cnt = strtol(msg_data, &endp, 10); (void)cmd_cnt;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int addr = strtol(msg_data, &endp, 16); (void)addr;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int data = strtol(msg_data, &endp, 16);
                    uint8_t scale_setting = ((uint8_t)data & 0x0C) >> 2;
                    if (scale_setting == 0) {
                        accel_scale = 2;
                    }
                    if (scale_setting == 1) {
                        accel_scale = 16;
                    }
                    if (scale_setting == 2) {
                        accel_scale = 4;
                    }
                    if (scale_setting == 3) {
                        accel_scale = 8;
                    }
                    msg_data = endp;
                }
                state_0x10_ctrl_xl = -1;
                // qDebug() << "accel scale: " << accel_scale;
            }

            if (msg.contains("0xD5", Qt::CaseSensitive) &&
                msg.contains("Read", Qt::CaseSensitive) &&
                state_0x28_accel >= 0 && state_0x20_tmp_gyro < 0) {
                std::string str = msg.toStdString();
                int strlen = msg.size();
                int stridx = 0;
                const char *msg_data;
                char *endp;
                msg_data = str.c_str();
                msg_data += stridx;
                if (msg_data && *(msg_data) != ',') {
                    double sample_time = strtod(msg_data, &endp); (void)sample_time;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int cmd_cnt = strtol(msg_data, &endp, 10); (void)cmd_cnt;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int addr = strtol(msg_data, &endp, 16); (void)addr;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int data = strtol(msg_data, &endp, 16);
                    accel_origin_t[state_0x28_accel] = data;
                    msg_data = endp;
                }
                state_0x28_accel++;
                cnt_accel_0x28_origin++;
                if (state_0x28_accel == 6) {
                    state_0x28_accel = -1;
                    cnt_accel_0x28_frame++;
                    accel_origin[0] =(uint16_t) accel_origin_t[0] | (uint16_t)(accel_origin_t[1] << 8);
                    accel_origin[1] =(uint16_t) accel_origin_t[2] | (uint16_t)(accel_origin_t[3] << 8);
                    accel_origin[2] =(uint16_t) accel_origin_t[4] | (uint16_t)(accel_origin_t[5] << 8);
                    accel[0] = ((float)accel_origin[0] * 2 * accel_scale * 9.7914) / 65536.0f;
                    accel[1] = ((float)accel_origin[1] * 2 * accel_scale * 9.7914) / 65536.0f;
                    accel[2] = ((float)accel_origin[2] * 2 * accel_scale * 9.7914) / 65536.0f;
                    // qDebug() << "accel: " << accel[0] << ", " << accel[1] << ", " << accel[2];
                }
            }

            if (msg.contains("0xD5", Qt::CaseSensitive) &&
                msg.contains("Read", Qt::CaseSensitive) &&
                state_0x20_tmp_gyro >= 0 && state_0x28_accel < 0) {
                std::string str = msg.toStdString();
                int strlen = msg.size();
                int stridx = 0;
                const char *msg_data;
                char *endp;
                msg_data = str.c_str();
                //msg_data += stridx;
                if (msg_data && *(msg_data) != ',') {
                    double sample_time = strtod(msg_data, &endp); (void)sample_time;
                    sample_timestamp = sample_time;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int cmd_cnt = strtol(msg_data, &endp, 10); (void)cmd_cnt;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int addr = strtol(msg_data, &endp, 16); (void)addr;
                    msg_data = endp;
                }
                if (msg_data && *(++msg_data) != ',') {
                    int data = strtol(msg_data, &endp, 16);
                    gyro_tmp_origin_t[state_0x20_tmp_gyro] = data;
                    msg_data = endp;
                }
                state_0x20_tmp_gyro++;
                cnt_gyro_tmp_0x20_origin++;
                if (state_0x20_tmp_gyro == 8) {
                    state_0x20_tmp_gyro = -1;
                    cnt_gyro_tmp_0x20_frame++;
                    gyro_tmp_origin[1] =(uint16_t) gyro_tmp_origin_t[2] | (uint16_t)(gyro_tmp_origin_t[3] << 8);
                    gyro_tmp_origin[2] =(uint16_t) gyro_tmp_origin_t[4] | (uint16_t)(gyro_tmp_origin_t[5] << 8);
                    gyro_tmp_origin[3] =(uint16_t) gyro_tmp_origin_t[6] | (uint16_t)(gyro_tmp_origin_t[7] << 8);
                    gyro[0] = ((float)gyro_tmp_origin[1] * 2 * gyro_scale * 3.14 / 180.0) / 65536.0f;
                    gyro[1] = ((float)gyro_tmp_origin[2] * 2 * gyro_scale * 3.14 / 180.0) / 65536.0f;
                    gyro[2] = ((float)gyro_tmp_origin[3] * 2 * gyro_scale * 3.14 / 180.0) / 65536.0f;
                    if (cnt_accel_0x28_frame == cnt_gyro_tmp_0x20_frame) {
                        QString output("$IMURAW,"+ QString::number(gyro[0],'f',6) + ","
                                + QString::number(gyro[1],'f',6) + ","
                                + QString::number(gyro[2],'f',6) + ","
                                + QString::number(accel[0],'f',6) + ","
                                + QString::number(accel[1],'f',6) + ","
                                + QString::number(accel[2],'f',6) + ","
                                + QString::number(sample_timestamp,'f',6)
                                + "\r\n");
                        _file_dst_txtstream[0].operator <<(output);
                        // qDebug() << output;
                    }
                }
            }
            msg.clear();
        }
        fileidx_percent(idx_read);
    }
    qDebug () << "[format] end format trans";
    qDebug() << "debug:" <<cnt_accel_0x28_origin << ", " << cnt_accel_0x28_frame <<
                ", " << cnt_gyro_tmp_0x20_origin << ", " << cnt_gyro_tmp_0x20_frame;

    _file_dst[0].close();
    _file_src.close();
    return 0;
}
