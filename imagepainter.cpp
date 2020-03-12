#include"imagepainter.h"
void ImagePainterBase::DrawBase(QPainter *Painter,double x, double y, char Base)
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
void ImagePainterBase::DrawBasePair(QPainter *Painter, double BaseX, double BaseY, char Base,double PairGap,double Degrees,bool Oritation)
{
    char MatchBase;
    DrawBase(Painter,BaseX,BaseY,Base);
    MatchBase=GenRNAMatchBase(Base);
    if(MatchBase){
        double PairX,PairY;
        Painter->setPen(Qt::black);
        if(Degrees>0 && Degrees<90){
            PairX=BaseX+PairGap*qSin(Degrees/180*M_PI);
            PairY=BaseY-PairGap*qCos(Degrees/180*M_PI);
            Painter->drawLine(QPoint(BaseX,BaseY),QPoint(PairX+PICTURE_FONT_SIZE,PairY-PICTURE_FONT_SIZE));
        }
        else if(Degrees>90 && Degrees<180){
            PairX=BaseX-PairGap*qSin(Degrees/180*M_PI);
            PairY=BaseY+PairGap*qCos(Degrees/180*M_PI);
            Painter->drawLine(QPoint(BaseX,BaseY-PICTURE_FONT_SIZE),QPoint(PairX+PICTURE_FONT_SIZE,PairY));
        }
        else if(!Degrees){
            PairX=BaseX;
            if(Oritation==LEFT_TOP){
                PairY=BaseY+PairGap;
            }
            else if(Oritation==RIGHT_DOWN){
                PairY=BaseY-PairGap;
            }
            else{
                DEBUG_WARN("invalid Oritation");
                return;
            }
        }
        else if(Degrees==90){
                PairY=BaseY;
                if(Oritation==LEFT_TOP){
                    PairX=BaseX-PairGap;
                }
                else if(Oritation==RIGHT_DOWN){
                    PairX=BaseX+PairGap;
                }
                else{
                    DEBUG_WARN("invalid Oritation");
                    return;
                }
        }
        else{
            DEBUG_WARN(Degrees);
            return;
        }
        DrawBase(Painter,PairX,PairY,MatchBase);
    }
    else{
        DEBUG_WARN("Invalid Base,fail to draw\n");
        return;
    }
   return;
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
    double Dx=16,degrees=135; //x坐标的间距 角度（与x轴正向的夹角）
    if(regex_search(MatchRNASeqBeg,MatchRNASeqEnd,RegexResult,RegexPartern)){
        int i=0;
        for(auto it=MatchRNASeq.begin();it!=MatchRNASeq.end();++it){
              if (it<RegexResult[0].first - 4){  //TODO 完善异常处理 5端存在过短情况
                  DrawBasePair(Painter,TWISTER_SISTER_BEGIN_X-Dx*i,TWISTER_SISTER_BEGIN_Y-Dy(Dx,degrees)*i,*it,25,degrees);
                  i++; //从右向下,所以是-dx -dy
              }
              else if(it==RegexResult[0].first - 4){
                  DrawBase(Painter,TWISTER_SISTER_BEGIN_X-Dx*i,TWISTER_SISTER_BEGIN_Y+Dy(Dx,degrees)*i,*it);
                  DrawConservativeSeq(Painter,TWISTER_SISTER_BEGIN_X-Dx*i,TWISTER_SISTER_BEGIN_Y-Dy(Dx,degrees)*i,degrees);
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
