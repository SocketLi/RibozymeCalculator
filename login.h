#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QNetworkAccessManager>
#include<QString>
#include<QJsonObject>
#include<QJsonDocument>
#include<QTime>
#include <QTimer>
#include<QEventLoop>
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_Ok_clicked();
    void on_Cancel_clicked();
private:
    QString GenRequestContent();
    void StartWaiting();
    void OnRequestFinished();
    Ui::Login *ui;
    QNetworkAccessManager* NetworkManager;
    QNetworkReply* ServerReply;
};

#endif // LOGIN_H
