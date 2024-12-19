#ifndef MULTIFILEFORMATTRANS_H
#define MULTIFILEFORMATTRANS_H

#include <QWidget>
#include <QThread>
#include "../tools/filetransformat.h"

namespace Ui {
class MultiFileFormatTrans;
}

class MultiFileFormatTrans : public QWidget
{
    Q_OBJECT

public:
    explicit MultiFileFormatTrans(QWidget *parent = nullptr);
    ~MultiFileFormatTrans();

private slots:
    void slot_clearFileList();
    void slot_startFileTrans();

    void slot_processadd(int percent);
    void slot_processcomplete();
    void slot_processfail();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Ui::MultiFileFormatTrans *ui;

    FileTransFormat *fileformat;
    int filenum;
    int fileidx;
    int lst_dprocess;
    float processvalue;
    QThread filethread;

    void nextstart(int idx);
    void reset();
};

#endif // MULTIFILEFORMATTRANS_H
