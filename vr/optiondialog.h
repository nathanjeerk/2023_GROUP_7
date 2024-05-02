#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    struct DialogData getMenuData();
    void setMenuData(DialogData data);

private:
    Ui::OptionDialog *ui;
};

struct DialogData {
    QString name;
    bool isVisible;
    unsigned int R;
    unsigned int G;
    unsigned int B;
};

#endif // OPTIONDIALOG_H
