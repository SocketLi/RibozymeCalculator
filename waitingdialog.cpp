#include "waitingdialog.h"
#include "ui_waitingdialog.h"
#include<QDir>
#include"common.h"
WaitingDialog::WaitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);//无边框
    setAttribute(Qt::WA_TranslucentBackground);//背景透明

        //屏幕居中显示
    int frmX = width();
    int frmY = height();

    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();

    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    move(movePoint);
    QString path=QDir::currentPath();
    path+="/pictures/loading.gif";
    DEBUG_WARN(path);
    movie = new QMovie(path); //加载gif图片
    ui->PaddingLable->setMovie(movie);
    movie->start();
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
    delete movie;
}
