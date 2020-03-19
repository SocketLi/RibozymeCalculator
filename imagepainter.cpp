#include"imagepainter.h"
void TwisterSisterPainter::DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)
{
    QFont PictureFont;
    PictureFont.setFamily("Microsoft YaHei");
    PictureFont.setPointSize(PICTURE_FONT_SIZE);
    Painter->setFont(PictureFont);
    smatch RegexResult;
    regex RegexPartern("GCT[A,G,C,T]A[A,G,C,T]");
    double Dx=10,degrees=120,ImageBeginX=(sqrt(5)-1)/2*PictureWidth,ImageBeginY=0.28*PictureHeight; //x坐标的间距 角度（与x轴正向的夹角),第一个碱基的起始坐标
    Painter->setPen(Qt::black);
    QPoint Mark5Cooord(ImageBeginX+Dx,ImageBeginY+Dy(Dx,degrees));//画5'和3'标识
    Painter->drawText(Mark5Cooord,"5'");
    Painter->drawText(TransCoord(Mark5Cooord,degrees,25),"3'");
    QPoint NoMatchBaseCoord,VerticalPartBeginCoord;
    if(regex_search(MatchRNASeq.begin(),MatchRNASeq.end(),RegexResult,RegexPartern)){
        int i=0,j=0;
        double VerticalPartDy=16;
        for(auto it=MatchRNASeq.begin();it!=MatchRNASeq.end();++it){
              if (it<RegexResult[0].first - 4){  //TODO 完善异常处理 5端存在过短情况
                  DrawBasePair(Painter,ImageBeginX-Dx*i,ImageBeginY-Dy(Dx,degrees)*i,*it,25,degrees);
                  i++; //从右向下,所以是-dx -dy
              }
              else if(it==RegexResult[0].first - 4){
                  QPoint ConservativeSeqBegin=TransCoord(QPoint(ImageBeginX-Dx*i,ImageBeginY-Dy(Dx,degrees)*i),
                                                         degrees,25);//生成一下上方保守序列的起点坐标
                  NoMatchBaseCoord.rx()=ImageBeginX-Dx*i-5;//*it的坐标，这个坐标也会被用于推断竖直序列的坐标起点
                  NoMatchBaseCoord.ry()=ImageBeginY-Dy(Dx,degrees)*i-Dy(5,degrees);
                  DrawBase(Painter,NoMatchBaseCoord.x(),NoMatchBaseCoord.y(),*it);//画出MatchRNASeq中不参与配对的那个碱基
                  DrawConservativeSeq(Painter,ConservativeSeqBegin.x(),ConservativeSeqBegin.y(),degrees);
                  VerticalPartBeginCoord.rx()=NoMatchBaseCoord.x()-Dx,
                  VerticalPartBeginCoord.ry()=NoMatchBaseCoord.y()-Dy(Dx,degrees);
              }
              else if(it==RegexResult[0].first){
                  DrawBase(Painter,VerticalPartBeginCoord.x()+PICTURE_FONT_SIZE,VerticalPartBeginCoord.y()+VerticalPartDy*j+PICTURE_FONT_SIZE/2,*it);
              }
              else if(it==RegexResult[0].first+3){
                  QPoint BeginCoord(VerticalPartBeginCoord.x(),VerticalPartBeginCoord.y()+VerticalPartDy*(j-1));//RegexResult[0].first+2的坐标
                  DrawBase(Painter,BeginCoord.x()+PICTURE_FONT_SIZE*1.4,BeginCoord.y()+PICTURE_FONT_SIZE*1.3,*it);
                  DrawBase(Painter,BeginCoord.x()-25-PICTURE_FONT_SIZE,BeginCoord.y()+PICTURE_FONT_SIZE*1.3,'A');
                  DrawBase(Painter,BeginCoord.x()+PICTURE_FONT_SIZE*2.5,BeginCoord.y()+PICTURE_FONT_SIZE*2,*(it+1));
                  DrawBase(Painter,BeginCoord.x()+PICTURE_FONT_SIZE,BeginCoord.y()+PICTURE_FONT_SIZE*3,*(it+2));
                  DrawBase(Painter,BeginCoord.x()-25-PICTURE_FONT_SIZE,BeginCoord.y()+PICTURE_FONT_SIZE*3,'C');
                  it+=2;
                  j+=2;
              }
              else{
                  DrawBasePair(Painter,VerticalPartBeginCoord.x(),VerticalPartBeginCoord.y()+VerticalPartDy*j,*it,25,90);
                  j++;
              }
        }
    }
}
void TwisterSisterPainter::DrawConservativeSeq(QPainter *Painter,int BeginX,int BeginY,double Degrees)
{
    string ConservativeSeqPart="CAGGGC";
    unsigned int i=0;
    double Dx=16,degress=Degrees-90;
    for(;i<ConservativeSeqPart.length();++i){
            DrawBasePair(Painter,BeginX-Dx*(i+1),BeginY-Dy(Dx,degress)*(i+1),ConservativeSeqPart[i],25,degress,RIGHT_DOWN);//右下往左上 -dx -dy
        }
    QPoint CirclePathBegin(BeginX-Dx*i,BeginY-Dy(Dx,degress)*i);
    QPoint CirclePathEnd=TransCoord(CirclePathBegin,degress,25,RIGHT_DOWN);
    ConservativeSeqPart="CGAACGU";
    DrawCriclePathBase(Painter,ConservativeSeqPart,CirclePathBegin,CirclePathEnd,25,false);//画圆形的部分
    QPoint ACBegin=TransCoord(QPoint(BeginX-Dx,BeginY-Dy(Dx,degress)),degress,25,RIGHT_DOWN);//获得与保守序列第一个碱基互补配对的碱基坐标
    ACBegin.rx()-=6;//画最后的两个AC
    ACBegin.ry()-=Dy(6,Degrees);
    DrawBase(Painter,ACBegin.x(),ACBegin.y(),'A');
    DrawBase(Painter,ACBegin.x()+2*PICTURE_FONT_SIZE/3,ACBegin.y()+PICTURE_FONT_SIZE,'C');
}
/*
 *
 * PistolPainter代码开始
 *
 */
void PistolPainter::DrawRibozymeImage(const string &RibozymeSeq, const string &MatchRNASeq, unsigned int PictureWidth, unsigned int PictureHeight, QPainter *Painter)
{

}

RibozymeImagePainter::RibozymeImagePainter(const string& RibozymeType)
{
    if(RibozymeType==TWISTER_SISTER){
        ImagePainter=new TwisterSisterPainter();
    }
    else if(RibozymeType==PISTOL){
        ImagePainter=new PistolPainter();
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
