#include"imagepainter.h"
void ImagePainterBase::DrawBase(QPainter *Painter,int x, int y, char Base)
{
    if(Painter==NULL){
        return;
    }
    switch (Base) {
     case 'A':
        Painter->setPen(QColor(108,106,107));
        break;
     case 'C':
        Painter->setPen(QColor(148,147,140));
        break;
     case 'G':
        Painter->setPen(QColor(204,159,148));
        break;
    case  'U':
       Painter->setPen(QColor(199,152,127));
       break;
    default:
        return;
    }
    Painter->drawText(x,y,QString(Base));
}
void ImagePainterBase::DrawBasePair(QPainter *Painter, int BaseX, int BaseY, char Base,double PairGap,double Degrees,bool Oritation)
{
    if(Painter==NULL){
        return;
    }
    if(Base=='T'){
        Base='U';
    }
    char MatchBase;
    DrawBase(Painter,BaseX,BaseY,Base);
    if(Oritation!=LEFT_TOP && Oritation!=RIGHT_DOWN){
        DEBUG_WARN("invalid Oritation");
        return;
    }
    if(Degrees<0 || Degrees>180){
        DEBUG_WARN(Degrees);
        return;
    }
    MatchBase=GenRNAMatchBase(Base);
    if(MatchBase){
        QPoint BaseCoord=QPoint(BaseX,BaseY);
        QPoint PairCoord=TransCoord(BaseCoord,Degrees,PairGap,Oritation);
        DrawMatchLine(Painter,BaseCoord,PairCoord,Degrees>=90 ? Degrees-90 : Degrees+90);
        DrawBase(Painter,PairCoord.x(),PairCoord.y(),MatchBase);
    }
    else{
        DEBUG_WARN("Invalid Base,fail to draw\n");
        return;
    }
   return;
}
void ImagePainterBase::SetPointCoord(QPoint &Point, int x, int y)
{
    Point.setX(x);
    Point.setY(y);
}
QPoint ImagePainterBase::TransCoord(const QPoint &OriginCoord, double Degrees, double PairGap, bool Oritation)
{
    QPoint NewPoint;
    if(Degrees>=0 && Degrees<90){
        if(Oritation==LEFT_TOP){
            NewPoint.setX(OriginCoord.x()+PairGap*qSin(Degrees/180*M_PI));
            NewPoint.setY(OriginCoord.y()-PairGap*qCos(Degrees/180*M_PI));
        }
        else{
            NewPoint.setX(OriginCoord.x()-PairGap*qSin(Degrees/180*M_PI));
            NewPoint.setY(OriginCoord.y()+PairGap*qCos(Degrees/180*M_PI));
        }
    }
    else if(Degrees>=90 && Degrees<180){
        if(Oritation==LEFT_TOP){
            NewPoint.setX(OriginCoord.x()-PairGap*qSin(Degrees/180*M_PI));
            NewPoint.setY(OriginCoord.y()+PairGap*qCos(Degrees/180*M_PI));
        }
        else{
            NewPoint.setX(OriginCoord.x()+PairGap*qSin(Degrees/180*M_PI));
            //qCos(Degrees/180*M_PI)在Degress为90的时候不为0
            if(Degrees==90){
                NewPoint.setY(OriginCoord.y());
            }
            else{
                NewPoint.setY(OriginCoord.y()-PairGap*qCos(Degrees/180*M_PI));
            }

        }
    }
    return NewPoint;
}
void ImagePainterBase::DrawMatchLine(QPainter *Painter, const QPoint &BaseCoord, const QPoint &MatchedBaseCoord, double Degrees)
{
    QPoint LineBeginCoord,LineEndCoord;
    if(!Degrees){
        Painter->setRenderHints(QPainter::Antialiasing, false);//启用会导致此时画出的匹配线过粗，不美观
        if(BaseCoord.x()>MatchedBaseCoord.x()){//判断两个碱基的位置关系从而确定直线的两个端点
            SetPointCoord(LineBeginCoord,MatchedBaseCoord.x()+PICTURE_FONT_SIZE+LOW_BASE_SPACE*2,MatchedBaseCoord.y()-PICTURE_FONT_SIZE/2);
            SetPointCoord(LineEndCoord,BaseCoord.x()-LOW_BASE_SPACE*2,BaseCoord.y()-PICTURE_FONT_SIZE/2);
        }
        else{
            SetPointCoord(LineBeginCoord,BaseCoord.x()+PICTURE_FONT_SIZE+LOW_BASE_SPACE*2,BaseCoord.y()-PICTURE_FONT_SIZE/2);
            SetPointCoord(LineEndCoord,MatchedBaseCoord.x()-LOW_BASE_SPACE*2,MatchedBaseCoord.y()-PICTURE_FONT_SIZE/2);
        }
    }
    else{
        QPoint HigherBase,LowBase;
        if(BaseCoord.y()<MatchedBaseCoord.y()){
            HigherBase=BaseCoord;
            LowBase=MatchedBaseCoord;
        }
        else{
            HigherBase=MatchedBaseCoord;
            LowBase=BaseCoord;
        }
        if(Degrees>0 && Degrees<90){
            if(Degrees>=45){
                SetPointCoord(LineBeginCoord,HigherBase.x()+PICTURE_FONT_SIZE+LOW_BASE_SPACE/qTan(Degrees/180*M_PI),HigherBase.y()+LOW_BASE_SPACE);
                SetPointCoord(LineEndCoord,LowBase.x()+PICTURE_FONT_SIZE-(PICTURE_FONT_SIZE+LOW_BASE_SPACE)/qTan(Degrees/180*M_PI),LowBase.y()-(PICTURE_FONT_SIZE+LOW_BASE_SPACE));
                LineBeginCoord.rx()-=K*(Degrees-45);
                LineEndCoord.rx()-=K*(Degrees-45);
            }
            else{
                 SetPointCoord(LineBeginCoord,HigherBase.x()+PICTURE_FONT_SIZE+2/qTan(Degrees/180*M_PI),HigherBase.y()+LOW_BASE_SPACE);
                 SetPointCoord(LineEndCoord,LowBase.x()-LOW_BASE_SPACE,LowBase.y()-(PICTURE_FONT_SIZE+LOW_BASE_SPACE)*qTan(Degrees/180*M_PI));
                 LineBeginCoord.ry()-=K*(45-Degrees);
                 LineEndCoord.ry()-=K*(45-Degrees);
            }
        }
        else if(Degrees>90 && Degrees<180){
            if(Degrees<135){
                SetPointCoord(LineBeginCoord,HigherBase.x()+2/qTan(Degrees/180*M_PI),HigherBase.y()+LOW_BASE_SPACE);
                SetPointCoord(LineEndCoord,LowBase.x()-(PICTURE_FONT_SIZE+LOW_BASE_SPACE)/qTan(Degrees/180*M_PI),LowBase.y()-(PICTURE_FONT_SIZE+LOW_BASE_SPACE));
                LineBeginCoord.rx()+=K*(135-Degrees);
                LineEndCoord.rx()+=K*(135-Degrees);
            }
            else{
                SetPointCoord(LineBeginCoord,HigherBase.x()-LOW_BASE_SPACE,HigherBase.y()-LOW_BASE_SPACE*qTan(Degrees/180*M_PI));
                SetPointCoord(LineEndCoord,LowBase.x()+(PICTURE_FONT_SIZE+LOW_BASE_SPACE),LowBase.y()+(PICTURE_FONT_SIZE+LOW_BASE_SPACE)*qTan(Degrees/180*M_PI));
                LineBeginCoord.ry()-=K*(Degrees-135);
                LineEndCoord.ry()-=K*(Degrees-135);
            }
        }
        else if(Degrees==90){
            Painter->setRenderHints(QPainter::Antialiasing, false);//启用会导致此时画出的匹配线过粗，不美观
            SetPointCoord(LineBeginCoord,LowBase.x()+PICTURE_FONT_SIZE/2,LowBase.y()-PICTURE_FONT_SIZE-LOW_BASE_SPACE*2);
            SetPointCoord(LineEndCoord,HigherBase.x()+PICTURE_FONT_SIZE/2,HigherBase.y()+LOW_BASE_SPACE*2);
        }
        else{
            DEBUG_WARN("invalid degrees");
            return;
        }
    }
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    Painter->setPen(pen);
    Painter->drawLine(LineBeginCoord,LineEndCoord);
    Painter->setRenderHints(QPainter::Antialiasing,true);
}
double ImagePainterBase::GetLineDegree(const QPoint &p1, const QPoint &p2) //取值范围0到2 pi 这里的line应该为从p1到p2的向量
//该函数获取的是向量与x轴正半轴的夹角
{
    if(p1.x()==p2.x()){
        if(p2.y()>p1.y()){
            return M_PI/2;
        }
        else{
            return 3*M_PI/2;
        }
    }
    else{
       double Degrees=qAtan(double(p1.y()-p2.y())/(p1.x()-p2.x()));
       //因为反正切的取值范围是-2分之pi到2分之pi,所以需要判断象限去获取度数
       if(p2.x()>p1.x() && p2.y()>=p1.y()){
           return Degrees; //一象限
       }
       else if(p2.x()<p1.x() && p2.y()>=p1.y()){
           return Degrees+M_PI;
       }
       else if(p2.x()<p1.x() && p2.y()<p1.y()){
           return Degrees+M_PI;
       }
       else if(p2.x()>p1.x() && p2.y()<p1.y()){
           return 2*M_PI+Degrees;
       }
       return Degrees;
    }
}
void ImagePainterBase::DrawCriclePathBase(QPainter *Painter,const string& RNAseq,QPoint &BeginCoord, QPoint &EndCoord, double r,bool IsClockWise,bool Oritation)
{
    if(Painter==NULL){
        return;
    }
    if(Oritation!=LEFT_TOP && Oritation!=RIGHT_DOWN){
        return;
    }
    double length=sqrt(pow(BeginCoord.x()-EndCoord.x(),2)+pow(BeginCoord.y()-EndCoord.y(),2));
    if(length>2*r){ //判断能不能给的两点和半径能不能构成一个圆
        DEBUG_WARN("r is too low");
        return;
    }
    QPoint Center=GetCenterCoord(BeginCoord,EndCoord,r,Oritation);//获取圆心
    double BeginDegrees=GetLineDegree(Center,BeginCoord);
    double EndDegrees=GetLineDegree(Center,EndCoord);
    //获取开始和结束的弧度
    Painter->save();//开始绘图
    Painter->translate(Center.x(),Center.y());
    double RotateAngle=0;//计算每个碱基的旋转弧度
    if(IsClockWise){
        RotateAngle= EndDegrees>BeginDegrees ? (EndDegrees-BeginDegrees)/(RNAseq.length()+1) : (2*M_PI-(EndDegrees-BeginDegrees))/(RNAseq.length()+1);
    }
    else{
        RotateAngle= EndDegrees>BeginDegrees ? -(2*M_PI-(EndDegrees-BeginDegrees))/(RNAseq.length()+1):(EndDegrees-BeginDegrees)/(RNAseq.length()+1);
    }
    for(unsigned int i=0;i<RNAseq.length();++i){
        double x=r*qCos(BeginDegrees+RotateAngle*(i+1));
        double y=r*qSin(BeginDegrees+RotateAngle*(i+1));
        DrawBase(Painter,x,y,RNAseq[i]);
    }
    Painter->restore();
}
QPoint ImagePainterBase::GetCenterCoord(QPoint &BeginCoord, QPoint &EndCoord, double r, bool Oritation)
{
    QPoint ConnectionCenter((BeginCoord.x()+EndCoord.x())/2,(BeginCoord.y()+EndCoord.y())/2);//begin end中点
    if(sqrt(pow(BeginCoord.x()-EndCoord.x(),2)+pow(BeginCoord.y()-EndCoord.y(),2))==2*r){
        return ConnectionCenter;
    }
    if(BeginCoord.y()==EndCoord.y()){
       int x=(BeginCoord.x()+EndCoord.x())/2;
       double y1=BeginCoord.y()+sqrt(pow(r,2)-pow(x-BeginCoord.x(),2));
       double y2=BeginCoord.y()-sqrt(pow(r,2)-pow(x-BeginCoord.x(),2));
       if(Oritation==LEFT_TOP){
           return y1<y2 ? QPoint(x,y1) : QPoint(x,y2);
       }
       else{
           return y1>y2 ? QPoint(x,y1) : QPoint(x,y2);
       }
    }//处理斜率不存在的情况
    BeginCoord.ry()=-BeginCoord.y();
    EndCoord.ry()=-EndCoord.y();
    ConnectionCenter.ry()=-ConnectionCenter.y();
    double Rate=double(EndCoord.y()-BeginCoord.y())/double(EndCoord.x()-BeginCoord.x()); //begin end 连线斜率
    double CenterLineRate=-1/Rate; //begin end中垂线斜率，求圆心用
    double CenterLineB=ConnectionCenter.y()-CenterLineRate*ConnectionCenter.x();
    double a=1+pow(CenterLineRate,2); //abc为联立圆和直线方程得到的二元一次方程中的二次一次常数项
    double b=-2*(BeginCoord.x()-CenterLineRate*CenterLineB+CenterLineRate*BeginCoord.y());
    double c=pow(BeginCoord.x(),2)+pow(BeginCoord.y(),2)-2*CenterLineB*BeginCoord.y()+pow(CenterLineB,2)-pow(r,2);
    double x1=(-b+sqrt(pow(b,2)-4*a*c))/(2*a);
    double x2=(-b-sqrt(pow(b,2)-4*a*c))/(2*a);
    double y1=CenterLineRate*x1+CenterLineB;
    double y2=CenterLineRate*x2+CenterLineB;
    y1=-y1;
    y2=-y2;
    BeginCoord.ry()=-BeginCoord.y();
    EndCoord.ry()=-EndCoord.y();
    if(Oritation==LEFT_TOP){
        if(x1==x2){
            return y1<y2 ? QPoint(x1,y1) :QPoint(x2,y2);
        }
        return x1<x2 ? QPoint(x1,y1) : QPoint(x2,y2);
    }
    else{
        if(x1==x2){
            return y1>y2 ? QPoint(x1,y1) :QPoint(x2,y2);
        }
        return x1>x2 ? QPoint(x1,y1) : QPoint(x2,y2);
    }
}
double ImagePainterBase::Dy(double Dx,double Degrees)
{
    if(Degrees>0 && Degrees<180){
        return Dx*qTan(Degrees/180*M_PI);
    }
    return 0; //TODO 完善异常处理
}
