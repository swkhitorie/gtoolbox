#include "multifileformattrans.h"
#include "ui_multifileformattrans.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

MultiFileFormatTrans::MultiFileFormatTrans(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiFileFormatTrans),
    filenum(0),
    fileidx(0),
    lst_dprocess(0),
    processvalue(0.0f)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
    this->setWindowTitle("Multi File Format Transformation");
    ui->multifile_input->setAcceptDrops(false);
    connect(ui->bt_clear, &QPushButton::clicked, this, &MultiFileFormatTrans::slot_clearFileList);
    connect(ui->bt_start, &QPushButton::clicked, this, &MultiFileFormatTrans::slot_startFileTrans);
    ui->pb_process->setRange(0, 100);
}

MultiFileFormatTrans::~MultiFileFormatTrans()
{
    delete ui;
}

void MultiFileFormatTrans::dragEnterEvent(QDragEnterEvent *event)
{
    if (true) {
        event->acceptProposedAction();
    }
}

void MultiFileFormatTrans::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) return;

    QList<QString> pathlist;
    foreach (QUrl u, urls) {
       QString filepath = u.toLocalFile();
       pathlist.append(filepath);
    }
    pathlist = pathlist.toSet().toList();
    ui->multifile_input->clear();
    for (int i = 0; i < pathlist.size(); i++) {
        QString path = pathlist.at(i);
        ui->multifile_input->append(path);
    }
}

void MultiFileFormatTrans::slot_clearFileList()
{
    ui->multifile_input->clear();
}

void MultiFileFormatTrans::slot_startFileTrans()
{
    QString a = ui->multifile_input->toPlainText();
    a.replace(tr("\r\n"), tr("\n"));
    QStringList filelist = a.split('\n',QString::SkipEmptyParts);
    foreach (QString tmp, filelist) {
        qDebug() << tmp;
        filenum++;
    }

    fileformat = new FileTransFormat[filenum]();
    for (int i = 0; i < filenum; i++) {
        fileformat[i].reset();
        fileformat[i].setFormatMethod(ui->method_sel->currentIndex());
        fileformat[i].setSourceFilePath(filelist.at(i));
        if (!fileformat[i].openSourceFile()) {
            delete [] fileformat;
            QMessageBox::critical(this, "critical",
               QString("打开文件失败, 文件不存在/文件过大(>300M), idx: " + QString::number(i)));
            return;
        }
    }
    fileidx = 0;
    nextstart(0);
}

void MultiFileFormatTrans::reset()
{
    fileidx = 0;
    filenum = 0;
    for (int i = 0; i < filenum; i++) {
        fileformat[i].reset();
    }
    delete []fileformat;
}

void MultiFileFormatTrans::nextstart(int idx)
{
    if (idx >= filenum) return;
    if (idx > 0) {
        filethread.disconnect(&fileformat[idx-1]);
    }
    connect(&filethread, &QThread::started, &fileformat[idx], &FileTransFormat::work);
    connect(&fileformat[idx], &FileTransFormat::progress_add, this, &MultiFileFormatTrans::slot_processadd);
    connect(&fileformat[idx], &FileTransFormat::progredd_complete, this, &MultiFileFormatTrans::slot_processcomplete);
    connect(&fileformat[idx], &FileTransFormat::progress_fail, this, &MultiFileFormatTrans::slot_processfail);
    filethread.start();
    qDebug() << "start " << idx;
}

void MultiFileFormatTrans::slot_processadd(int percent)
{
    float dval = static_cast<float>(percent - lst_dprocess) / static_cast<float>(filenum);
    lst_dprocess = percent;
    processvalue += dval;
    ui->pb_process->setValue(processvalue);
}

void MultiFileFormatTrans::slot_processcomplete()
{
    filethread.quit();
    filethread.exit();

    if (fileidx < filenum - 1) {
        qDebug() << "complete " << fileidx;
        fileidx++;
        lst_dprocess = 0;
        nextstart(fileidx);
    } else {
        ui->pb_process->setValue(100);
        QMessageBox::information(this, "information", "解析帧完毕");
        reset();
    }
}

void MultiFileFormatTrans::slot_processfail()
{
    filethread.quit();
    filethread.exit();
    QMessageBox::critical(this, "critical", "格式转换失败");
    reset();
}
