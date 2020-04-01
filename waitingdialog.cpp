#include "waitingdialog.h"
#include "ui_waitingdialog.h"

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

        //加载gif图片
    movie = new QMovie("E:\\QT_Project\\Ribozyme_Calculator\\pictures\\loading.gif");
    ui->PaddingLable->setMovie(movie);
    movie->start();
    //ui->PaddingLable->show();
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
    delete movie;
}
