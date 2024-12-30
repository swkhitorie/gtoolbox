#include "qtb.h"
#include "ui_qtb.h"
#include <QDebug>

QTB::QTB(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTB)
{
    ui->setupUi(this);
    connect(ui->actionMultiFileFormatTrans, &QAction::triggered, this, &QTB::open_multiFileFormatTrans);
    connect(ui->actionserialdebugger, &QAction::triggered, this, &QTB::open_serialProtocolDebugger);
}

QTB::~QTB()
{
    delete ui;
}

void QTB::open_multiFileFormatTrans()
{
    ui_multifiletrans = new MultiFileFormatTrans();
    ui_multifiletrans->setWindowModality(Qt::ApplicationModal);
    ui_multifiletrans->show();
}

void QTB::open_serialProtocolDebugger()
{
    ui_serialprotocoldebugger = new SerialProtocolDebugger();
    ui_serialprotocoldebugger->setWindowModality(Qt::ApplicationModal);
    ui_serialprotocoldebugger->show();
}
