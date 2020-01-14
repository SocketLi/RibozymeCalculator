#ifndef MYWIDGET_H
#define MYWIDGET_H
#include<QWidget>
class MyWidget:public QWidget
{
  //  Q_OBJECT
public:
    MyWidget(QWidget *parent);
    void paintEvent(QPaintEvent *ev);
    void SetPictureParas(const int &pPictureX,const int &pPictureY,
                         const int &pPictureWidth,const int& pPictureHeight);
    void SetDrawClicked(bool Clikcked);
private:
    int PictureX,PictureY,PictureWidth,PictureHeight;
    bool IsDrawClicked;
};

#endif // MYWIDGET_H
