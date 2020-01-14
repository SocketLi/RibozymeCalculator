#include"mywidget.h"
#include<qpainter.h>
#include<qDebug>
using namespace std;
MyWidget::MyWidget(QWidget *parent):QWidget(parent)
{

}
void MyWidget::paintEvent(QPaintEvent *ev)
{
    qDebug()<<"in paint"<<endl;
    if(IsDrawClicked){
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::green);
        qDebug()<<PictureX<<PictureY<<endl;
        painter.drawRect(400,100,400+20,100+20);
       SetDrawClicked(false);
   }
}
void MyWidget::SetPictureParas(const int &pPictureX, const int &pPictureY,
                               const int &pPictureWidth, const int &pPictureHeight)
{
   PictureX=pPictureX;
   PictureY=pPictureY;
   PictureWidth=pPictureWidth;
   PictureHeight=pPictureHeight;
}
void MyWidget::SetDrawClicked(bool Clikcked)
{
    IsDrawClicked=Clikcked;
}
