#include"imagepainter.h"
void ImagePainterBase::DrawBase(QPainter *Painter,int x, int y, char Base)
{
    if(Painter==NULL){
        return;
    }
    if(Base=='T'){
        Base='U';
    }
    switch (Base) {
     case 'A':
        Painter->setPen(Qt::red);
        break;
     case 'C':
        Painter->setPen(Qt::yellow);
        break;
     case 'G':
        Painter->setPen(Qt::green);
        break;
    case  'U':
       Painter->setPen(Qt::gray);
       break;
    default:
        break;
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
            NewPoint.setY(OriginCoord.y()-PairGap*qCos(Degrees/180*M_PI));
        }
    }
    return NewPoint;
}
void ImagePainterBase::DrawMatchLine(QPainter *Painter, const QPoint &BaseCoord, const QPoint &MatchedBaseCoord, double Degrees)
{
    QPoint LineBeginCoord,LineEndCoord;
    if(!Degrees){
        if(BaseCoord.x()>MatchedBaseCoord.x()){//判断两个碱基的位置关系从而确定直线的两个端点
            SetPointCoord(LineBeginCoord,MatchedBaseCoord.x()+PICTURE_FONT_SIZE,MatchedBaseCoord.y()-PICTURE_FONT_SIZE/2);
            SetPointCoord(LineEndCoord,BaseCoord.x(),BaseCoord.y()-PICTURE_FONT_SIZE/2);
        }
        else{
            SetPointCoord(LineBeginCoord,BaseCoord.x()+PICTURE_FONT_SIZE,BaseCoord.y()-PICTURE_FONT_SIZE/2);
            SetPointCoord(LineEndCoord,MatchedBaseCoord.x(),MatchedBaseCoord.y()-PICTURE_FONT_SIZE/2);
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
            SetPointCoord(LineBeginCoord,HigherBase.x()+PICTURE_FONT_SIZE,HigherBase.y());
            if(Degrees>=45){
                SetPointCoord(LineEndCoord,LowBase.x()+PICTURE_FONT_SIZE-PICTURE_FONT_SIZE/qTan(Degrees/180*M_PI),LowBase.y()-PICTURE_FONT_SIZE-LOW_BASE_SPACE);
                LineBeginCoord.rx()-=K*(Degrees-45);
                LineEndCoord.rx()-=K*(Degrees-45);
            }
            else{
                 SetPointCoord(LineEndCoord,LowBase.x()-LOW_BASE_SPACE,LowBase.y()-PICTURE_FONT_SIZE*qTan(Degrees/180*M_PI));
                 LineBeginCoord.ry()-=K*(45-Degrees);
                 LineEndCoord.ry()-=K*(45-Degrees);
            }
        }
        else if(Degrees>90 && Degrees<180){
            SetPointCoord(LineBeginCoord,HigherBase.x(),HigherBase.y());
            if(Degrees<135){
                SetPointCoord(LineEndCoord,LowBase.x()-PICTURE_FONT_SIZE/qTan(Degrees/180*M_PI),LowBase.y()+PICTURE_FONT_SIZE+LOW_BASE_SPACE);
                LineBeginCoord.rx()+=K*(135-Degrees);
                LineEndCoord.rx()+=K*(135-Degrees);
            }
            else{
                SetPointCoord(LineEndCoord,LowBase.x()+PICTURE_FONT_SIZE+LOW_BASE_SPACE,LowBase.y()+PICTURE_FONT_SIZE*qTan(Degrees/180*M_PI));
                LineBeginCoord.ry()-=K*(Degrees-135);
                LineEndCoord.ry()-=K*(Degrees-135);
            }
        }
        else{
            DEBUG_WARN("invalid degrees");
            return;
        }
    }
    Painter->setPen(Qt::black);
    Painter->drawLine(LineBeginCoord,LineEndCoord);
}
double ImagePainterBase::Dy(double Dx,double Degrees)
{
    if(Degrees>0 && Degrees<180){
        return Dx*qTan(Degrees/180*M_PI);
    }
    return 0; //TODO 完善异常处理
}
void TwisterSisterPainter::DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)
{
    QFont PictureFont;
    PictureFont.setFamily("Microsoft YaHei");
    PictureFont.setPointSize(PICTURE_FONT_SIZE);
    Painter->setFont(PictureFont);
    smatch RegexResult;
    regex RegexPartern("GCT[A,G,C,T]A[A,G,C,T]");
    string::const_iterator MatchRNASeqBeg = MatchRNASeq.begin();
    string::const_iterator MatchRNASeqEnd=MatchRNASeq.end();
    double Dx=16,degrees=120; //x坐标的间距 角度（与x轴正向的夹角）
    if(regex_search(MatchRNASeqBeg,MatchRNASeqEnd,RegexResult,RegexPartern)){
        int i=0;
        for(auto it=MatchRNASeq.begin();it!=MatchRNASeq.end();++it){
              if (it<RegexResult[0].first - 4){  //TODO 完善异常处理 5端存在过短情况
                  DrawBasePair(Painter,TWISTER_SISTER_BEGIN_X-Dx*i,TWISTER_SISTER_BEGIN_Y-Dy(Dx,degrees)*i,*it,40,degrees);
                  i++; //从右向下,所以是-dx -dy
              }
              else if(it==RegexResult[0].first - 4){
                  DrawBase(Painter,TWISTER_SISTER_BEGIN_X-Dx*i,TWISTER_SISTER_BEGIN_Y+Dy(Dx,degrees)*i,*it);
                 // DrawConservativeSeq(Painter,TWISTER_SISTER_BEGIN_X-Dx*i,TWISTER_SISTER_BEGIN_Y-Dy(Dx,degrees)*i,degrees);
              }
        }
    }
}
void TwisterSisterPainter::DrawConservativeSeq(QPainter *Painter,double BeginX,double BeginY,double Degrees)
{
    string ConservativeSeqPart="CAGGGCCGAACGU";
    for(int i=0;i<ConservativeSeqPart.length();++i){
        if(i<6){
            double Dx=16,degress=Degrees-90;
            DrawBasePair(Painter,BeginX-Dx*i,BeginY-Dy(Dx,degress)*i,ConservativeSeqPart[i],25,degress);
            //右下往左上 -dx -dy
        }
    }
}
RibozymeImagePainter::RibozymeImagePainter(const string& RibozymeType)
{
    if(RibozymeType==TWISTER_SISTER){
        ImagePainter=new TwisterSisterPainter();
    }
}
void RibozymeImagePainter::DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)
{
    if(ImagePainter==NULL){
        return;
    }
    ImagePainter->DrawRibozymeImage(RibozymeSeq,MatchRNASeq,PictureWidth,PictureHeight,Painter);
}
RibozymeImagePainter::~RibozymeImagePainter()
{
    delete ImagePainter;
}
