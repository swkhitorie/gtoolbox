#include "qtb.h"
#include "ui_qtb.h"

QTB::QTB(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QTB)
{
    ui->setupUi(this);
}

QTB::~QTB()
{
    delete ui;
}
