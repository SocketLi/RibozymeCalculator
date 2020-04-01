#include "login.h"
#include "ui_login.h"
#include"common.h"
#include "waitingdialog.h"
using namespace std;
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    NetworkManager=new QNetworkAccessManager(this);
    ServerReply=NULL;
}

Login::~Login()
{
    delete ui;
    delete NetworkManager;
}
void Login::StartWaiting()
{
    QTimer WaitTimer;
    WaitingDialog PaddingDialog;
    WaitTimer.setInterval(30000);  // 设置超时时间 30 秒
    WaitTimer.setSingleShot(true);  // 单次触发
    QEventLoop WaitLoop;
    connect(&WaitTimer, &QTimer::timeout, &WaitLoop, &QEventLoop::quit);
    connect(ServerReply, &QNetworkReply::finished, &WaitLoop, &QEventLoop::quit);
    PaddingDialog.show();
    WaitLoop.exec();
    if(WaitTimer.isActive()){
       PaddingDialog.close();
       OnRequestFinished();
    }
    else{
        PaddingDialog.close();
        disconnect(ServerReply, &QNetworkReply::finished, &WaitLoop, &QEventLoop::quit);
        ServerReply->abort();
        delete ServerReply;
    }
}
void Login::on_Ok_clicked()
{
    QNetworkRequest Request;
    Request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    Request.setUrl(QUrl("http://www.nankaitencent.club"));
    ServerReply= NetworkManager->post(Request,GenRequestContent().toUtf8());
    StartWaiting();
}
QString Login::GenRequestContent()
{
    QJsonObject RequestContent;
    RequestContent.insert("type","login");
    RequestContent.insert("username",ui->UserName->text());
    RequestContent.insert("password",ui->Passwd->text());
    QJsonDocument JsonDocument;
    JsonDocument.setObject(RequestContent);
    return QString(JsonDocument.toJson(QJsonDocument::Compact));
}
void Login::OnRequestFinished()
{
    QVariant statusCode = ServerReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid()){
        QNetworkReply::NetworkError err = ServerReply->error();
        if(err != QNetworkReply::NoError) {
            DEBUG_WARN(ServerReply->errorString());
        }
        else {//校验成功
            qDebug() << ServerReply->readAll()<<endl;
            //Login::close();
            delete ServerReply;
        }
    }
 }
void Login::on_Cancel_clicked()
{
    Login::close();
}
