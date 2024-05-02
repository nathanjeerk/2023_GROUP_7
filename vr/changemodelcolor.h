#ifndef CHANGEMODELCOLOR_H
#define CHANGEMODELCOLOR_H

#include <QDialog>

namespace Ui {
class changeModelColor;
}

class changeModelColor : public QDialog
{
    Q_OBJECT

public:
    explicit changeModelColor(QWidget *parent = nullptr);
    ~changeModelColor();

private:
    Ui::changeModelColor *ui;
};

#endif // CHANGEMODELCOLOR_H
