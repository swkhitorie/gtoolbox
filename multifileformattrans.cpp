#include "multifileformattrans.h"
#include "ui_multifileformattrans.h"

MultiFileFormatTrans::MultiFileFormatTrans(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiFileFormatTrans)
{
    ui->setupUi(this);
}

MultiFileFormatTrans::~MultiFileFormatTrans()
{
    delete ui;
}
