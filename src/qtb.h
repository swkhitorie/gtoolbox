#ifndef QTB_H
#define QTB_H

#include <QMainWindow>
#include "./uiforms/multifileformattrans.h"
#include "./uiforms/serialprotocoldebugger.h"

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
    void open_serialProtocolDebugger();

private:
    Ui::QTB *ui;
    MultiFileFormatTrans *ui_multifiletrans;
    SerialProtocolDebugger *ui_serialprotocoldebugger;
};

#endif // QTB_H
