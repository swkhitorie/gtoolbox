#ifndef QTB_H
#define QTB_H

#include <QMainWindow>

namespace Ui {
class QTB;
}

class QTB : public QMainWindow
{
    Q_OBJECT

public:
    explicit QTB(QWidget *parent = nullptr);
    ~QTB();

private:
    Ui::QTB *ui;
};

#endif // QTB_H
