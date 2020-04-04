#include "login.h"
#include "ui_login.h"
#include"common.h"
#include "waitingdialog.h"
#include<QCloseEvent>
#include<windows.h>
using namespace std;
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    NetworkManager=new QNetworkAccessManager(this);
    ui->Passwd->setEchoMode(QLineEdit::Password);
    ui->Passwd->setStyleSheet(QString("QLineEdit{font-size:10px;}"));
    QFile RemFile(QDir::currentPath()+"/user.txt");
    DEBUG_WARN(QDir::currentPath()+"/user.txt");
    if(RemFile.exists()){
        if(RemFile.open( QIODevice::ReadOnly | QFile::Text)){
            QString Username=RemFile.readLine();
            ui->UserName->setText(Username);
            QByteArray Password=RemFile.readLine();
            if(Password.isEmpty()){
                ui->RemPasswd->setChecked(false);
            }
            else{
                ui->Passwd->setText(QString(QByteArray::fromBase64(Password)));
                ui->RemPasswd->setChecked(true);
            }
            RemFile.close();
        }
    }
    ServerReply=NULL;
    IsPass=false;
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
    WaitTimer.start();
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
        QMessageBox::critical(this,"ERROR","Connection Timeout,please check your network",QMessageBox::Ok);
        disconnect(ServerReply, &QNetworkReply::finished, &WaitLoop, &QEventLoop::quit);
        ServerReply->abort();
        delete ServerReply;
    }
}
void Login::SetUpRememberFile()
{
    QString FilePath=QDir::currentPath()+"/user.txt";
    QFile RemFile(FilePath);
    if(RemFile.open( QIODevice::ReadWrite | QFile::Truncate)){
        QString Username=ui->UserName->text().simplified();//每个涉及Username的部分调用一下进行预处理
        RemFile.write(Username.toStdString().c_str(),Username.length());
        RemFile.write("\n",1);
        if(ui->RemPasswd->isChecked()){
            QByteArray Password=ui->Passwd->text().toLocal8Bit();
            RemFile.write(Password.toBase64().toStdString().c_str(),Password.toBase64().length());
        }
        SetFileAttributes((LPCWSTR)FilePath.unicode(),FILE_ATTRIBUTE_HIDDEN);
        RemFile.close();
    }
    return;
}
void Login::on_Ok_clicked()
{
    QNetworkRequest Request;
    Request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    Request.setUrl(QUrl("http://www.nankaitencent.club"));
    ServerReply= NetworkManager->post(Request,GenRequestContent().toUtf8());
    StartWaiting();
}
bool Login::IsResponseValid(const QJsonObject& RootObj)
{
    return RootObj.contains(STATUS);//
}
QString Login::GenRequestContent()
{
    QJsonObject RequestContent;
    RequestContent.insert("type","login");
    QString Username=ui->UserName->text().simplified();
    RequestContent.insert("username",Username);
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
            OnSeverError();
            DEBUG_WARN(ServerReply->errorString());
        }
        else {//收到数据
            QString Response(ServerReply->readAll());
            DEBUG_WARN(ServerReply->readAll());
            QJsonDocument JsonDocument = QJsonDocument::fromJson(Response.toUtf8());
            QJsonObject RootObj;
            if(!JsonDocument.isNull()){
               if(JsonDocument.isObject()){
                   RootObj = JsonDocument.object();
               }
               else{
                   OnSeverError();
                   return;
               }
           }
           else{
                OnSeverError();
                return;
           }
           if(IsResponseValid(RootObj)){
                QString ResponseType=RootObj.value(STATUS).toString();
                int Status=ResponseType.toInt();
                DEBUG_WARN(Status);
                if(Status==SUCCESS){
                    QString Time=RootObj.value(AVAILABLE_TIME).toString();
                    Time.push_front("Login Success!Your license will expire at ");
                    QMessageBox::information(this,"SUCCESS",Time,QMessageBox::Ok);
                    SetUpRememberFile();
                    IsPass=true;
                    Login::close();
                }
                else if(Status==NO_AVAILABLE_TIME){
                    QMessageBox::critical(this,"ERROR","Sorry,your license has expired!",QMessageBox::Ok);
                }
                else if(Status==ERR_NAME_OR_PASSWORD){
                    QMessageBox::critical(this,"ERROR","Invalid username or password!",QMessageBox::Ok);
                }
                else{
                    OnSeverError();
                }
            }
            else{
                OnSeverError();
            }
        }
    }
    else{
         OnSeverError();
    }
    delete ServerReply;
    return;
 }
void Login::OnSeverError()
{
    IsPass=false;
    QMessageBox::critical(this,"ERROR","Server returns an error,please try again later",QMessageBox::Ok);
}
void Login::closeEvent(QCloseEvent *e)
{
    if(IsPass){
        qApp->exit();
    }
    else{
        Exit();
    }
}
void Login::Exit()
{
    exit(EXIT_SUCCESS);
}
void Login::on_Cancel_clicked()
{
   Exit();
}
