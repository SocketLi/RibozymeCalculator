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
#include<QMessageBox>
#include<QDir>
#define STATUS "status" //JSON字段
#define AVAILABLE_TIME "AvailableTime"
enum{
    BAD_REQUEST=0,
    SUCCESS,
    NO_AVAILABLE_TIME,
    ERR_NAME_OR_PASSWORD,
    SQL_QUERY_ERROR
};
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
    void Exit();
    bool IsResponseValid(const QJsonObject& RootObj);
    void OnSeverError();
    void closeEvent(QCloseEvent *e);
    void SetUpRememberFile();
    Ui::Login *ui;
    QNetworkAccessManager* NetworkManager;
    QNetworkReply* ServerReply;
    bool IsPass;
};

#endif // LOGIN_H
