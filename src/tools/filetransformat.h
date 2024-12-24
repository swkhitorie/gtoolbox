#ifndef FILETRANSFORMAT_H
#define FILETRANSFORMAT_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QDataStream>
#include <QTextStream>

#define BIGFILE_SUPPORT

/* not good support for big file (size > 300M) */
class FileTransFormat : public QObject
{
    Q_OBJECT
public:
    explicit FileTransFormat(QObject *parent = nullptr);

    static constexpr uint8_t max_multi_dstfilelen = 5;
    static constexpr uint32_t bigfile_bufferlen = 100*1024;
    static constexpr uint32_t bigfile_minlen = 300*1024*1024;
    enum Format {
        /* $RTK_RAW,d300be43500133ce3e62000000280169...<CR><LF>
         * and
         * $RTK_BASE,d300be43500133ce3e62000000280169...<CR><LF>
         * -->
         * RTCM3.x Data Binary
        */
        USR_1,

        /*
         * $IMURAW,0.017257,-0.015577,-0.008399,0.438436,-0.112297,9.841511,688.221849,118222.869000...<CR><LF>
         * -->
         * NovAtel Inertial Data Formta [https://docs.novatel.com/Waypoint/Content/Data_Formats/IE_Data_Formats.htm?]
        */
        USR_2,

        /*
         * LSM6DSR: (KingstVIS analyszed i2c bus result .txt)
         * 0.6435248000,29,0xD4,0x10,Write,ACK
         * ...
         * ->
         * 0xD4,0x28,Write ->6
         * 0xD4,0x20,Write ->8
        */
        USR_3,
    };

    void reset();

    void setSourceFilePath(QString path);

    int getSourceFileSize();

    bool openSourceFile();

    void setFormatMethod(int method);

    int format_usr1();

    int format_usr2();

    int format_usr3();

public slots:
    int work();

signals:
    void progress_fail(int code);
    void progress_add(int percent);
    void progredd_complete();

private:
    QFile           _file_src;
    int             _file_src_len;
    QString         _file_src_path;
    QByteArray      _file_src_data;

    int             _format_method;
    int             _file_trans_percent[2];

    QFile           _file_dst[max_multi_dstfilelen];
    QTextStream     _file_dst_txtstream[max_multi_dstfilelen];
    QDataStream     _file_dst_binstream[max_multi_dstfilelen];

    void            filebigbuffer_update(int ridx);
    void            filemap_update(int idx);
    uint8_t         xor_sum_check(const uint8_t *buf);  /* $GNGGA,...,*<CR><LF> */
    uint8_t         format_usr1_hexascii_to_byte(const char *p);

    void            fileidx_percent(int read_idx);
    char            getfile_buffer(int &read_idx, int &buffer_idx);
};


/*
  use example
  init:
    FileTransFormat *format1;
    format1 = new FileTransFormat();
    format1->setFormatMethod(FileTransFormat::USR_1);
    format1->setSourceFilePath(fileFullName);
    if (!format1->openSourceFile()) {
        QMessageBox::critical(this, "critical", "打开文件失败");
        return;
    }
    qDebug () << "source file size: " << format1->getSourceFileSize() / 1024 << "kB";

    multi thread parse example process:

    QThread fthread;
    format1->moveToThread(&fthread);
    connect(&fthread, &QThread::started, format1, &FileTransFormat::work);
    connect(format1, &FileTransFormat::progress_add, this, &MainWindow::slotprogress);
    connect(format1, &FileTransFormat::progredd_complete, this, &MainWindow::slotEnd);
    connect(format1, &FileTransFormat::progress_fail, this, &MainWindow::slotfailEnd);
    fthread.start();

    void slotprogress(int percent)
    {  processbar->setValue(static_cast<int>(percent)); }

    void slotEnd()
    {
        qDebug() << "end time: " << QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
        fthread.quit();
        fthread.exit();
        processbar->setValue(100);
        QMessageBox::information(this, "information", "format complete");
    }

    void slotfailEnd(int code)
    {
        fthread.quit();
        fthread.exit();
        QMessageBox::information(this, "information", "format fail");
    }
*/

#endif // FILETRANSFORMAT_H
