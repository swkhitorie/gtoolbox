#ifndef QTB_H
#define QTB_H

#include <QMainWindow>
#include "./uiforms/multifileformattrans.h"

namespace Ui {
class QTB;
}

class QTB : public QMainWindow
{
    Q_OBJECT

public:
    explicit QTB(QWidget *parent = nullptr);
    ~QTB();

private slots:
    void open_multiFileFormatTrans();

private:
    Ui::QTB *ui;
    MultiFileFormatTrans *ui_multifiletrans;

};

#endif // QTB_H
