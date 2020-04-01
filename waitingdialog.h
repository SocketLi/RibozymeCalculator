#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H
#include <QDialog>
#include <QDesktopWidget>
#include <QMovie>
#include <QDialog>

namespace Ui {
class WaitingDialog;
}

class WaitingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDialog(QWidget *parent = 0);
    ~WaitingDialog();

private:
    Ui::WaitingDialog *ui;
    QMovie *movie;
};

#endif // WAITINGDIALOG_H
