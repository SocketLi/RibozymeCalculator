#include"imagepainter.h"
void ImagePainterBase::DrawBase(QPainter *Painter, unsigned int x, unsigned int y, char Base)
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
void ImagePainterBase::DrawBasePair(QPainter *Painter, unsigned int BaseX, unsigned int BaseY,
                                    char Base, unsigned int PairX, unsigned int PairY)
{
    char MatchBase;
    DrawBase(Painter,BaseX,BaseY,Base);
    MatchBase=GenRNAMatchBase(Base);
    if(MatchBase){
        DrawBase(Painter,PairX,PairY,MatchBase);
    }
    else{
        DEBUG_WARN("Invalid Base,fail to draw\n");
        return;
    }
   return;
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
    if(regex_search(MatchRNASeqBeg,MatchRNASeqEnd,RegexResult,RegexPartern)){
        int i=0;
        for(auto it=MatchRNASeq.begin();it!=MatchRNASeq.end();++it){
              if (it <RegexResult[0].first - 4){  //TODO 完善异常处理 5端存在过短情况
                  DrawBase(Painter,TWISTER_SISTER_BEGIN_X-16*i,TWISTER_SISTER_BEGIN_Y+16*i,*it);
                  i++;
              }
              else if(it==RegexResult[0].first - 4){
                 DrawBase(Painter,TWISTER_SISTER_BEGIN_X-16*i,TWISTER_SISTER_BEGIN_Y+16*i,*it);
              }
        }
    }
    DrawBase(Painter,PictureWidth/3,PictureHeight-10,RibozymeSeq[0]);
    Painter->drawText(PictureWidth/3,PictureHeight-28,QString(RibozymeSeq[0]));
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
