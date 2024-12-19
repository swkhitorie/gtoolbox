#ifndef MULTIFILEFORMATTRANS_H
#define MULTIFILEFORMATTRANS_H

#include <QWidget>

namespace Ui {
class MultiFileFormatTrans;
}

class MultiFileFormatTrans : public QWidget
{
    Q_OBJECT

public:
    explicit MultiFileFormatTrans(QWidget *parent = nullptr);
    ~MultiFileFormatTrans();

private:
    Ui::MultiFileFormatTrans *ui;
};

#endif // MULTIFILEFORMATTRANS_H
