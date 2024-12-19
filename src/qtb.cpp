#include "qtb.h"
#include "ui_qtb.h"
#include <QDebug>

QTB::QTB(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTB)
{
    ui->setupUi(this);
    connect(ui->actionMultiFileFormatTrans, &QAction::triggered, this, &QTB::open_multiFileFormatTrans);
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
