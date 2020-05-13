#include"imagepainter.h"
unsigned int PistolGTBackPos=0; //pistol 筛选标记中G的G距最后一个N的距离，绘图时使用
unsigned int HammerTCBackPos=0; //意义同上
void TwisterSisterPainter::DrawRibozymeImage(const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)
{
    QFont PictureFont("Microsoft YaHei",PICTURE_FONT_SIZE,75);
    Painter->setFont(PictureFont);
    smatch RegexResult;
    regex RegexPartern("GCU[AGCU]A[AGCU]");
    double Dx=10,degrees=120,ImageBeginX=((sqrt(5)-1)/2)*PictureWidth,ImageBeginY=0.28*PictureHeight; //x坐标的间距 角度（与x轴正向的夹角),第一个碱基的起始坐标
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
                  DrawBase(Painter,VerticalPartBeginCoord.x()+PICTURE_FONT_SIZE,VerticalPartBeginCoord.y()+VerticalPartDy*(j-1)+PICTURE_FONT_SIZE/2,*it);
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
void PistolPainter::DrawRibozymeImage(const string &MatchRNASeq, unsigned int PictureWidth, unsigned int PictureHeight, QPainter *Painter)
{
    QFont PictureFont("Microsoft YaHei",PICTURE_FONT_SIZE,75);
    Painter->setFont(PictureFont);
    double Dx=14,Dy=20,Degrees=0,ImageBeginX=PictureWidth-50,ImageBeginY=((sqrt(5)-1)/2)*PictureHeight;
    int k=0;

    Painter->setPen(Qt::black);//画5'和3'标识
    QPoint Mark5Cooord(ImageBeginX,ImageBeginY+PICTURE_FONT_SIZE+5);
    Painter->drawText(Mark5Cooord,"5'");
    Painter->drawText(Mark5Cooord.x(),Mark5Cooord.y()-PICTURE_FONT_SIZE*3-25,"3'");

    QPoint CirclePathBegin,CirclePathEnd,VerticalBeginPos,BackConservativeSeqBegin;
    for(unsigned int i=0;i<MatchRNASeq.length();++i){
        if(i<MatchRNASeq.length()-PistolGTBackPos){
            DrawBasePair(Painter,ImageBeginX-Dx*i,ImageBeginY,MatchRNASeq[i],25,Degrees);
        }
        else if(i==MatchRNASeq.length()-PistolGTBackPos){
            double r=56,GUDx=0;
            string RNASeq="GCGAAUCCCGAAUU";
            SetPointCoord(CirclePathBegin,ImageBeginX-Dx*(i-1),ImageBeginY-25);//设定中间圆形路径的起始坐标
            SetPointCoord(CirclePathEnd,ImageBeginX-Dx*(i-1)-r,ImageBeginY-25+r);
            GUDx=(r-25)/3;
            DrawCriclePathBase(Painter,RNASeq,CirclePathBegin,CirclePathEnd,r,false);
            DrawBase(Painter,ImageBeginX-Dx*(i-1)-GUDx,ImageBeginY+GUDx,'G');
            DrawBase(Painter,ImageBeginX-Dx*(i-1)-GUDx*2,ImageBeginY+GUDx*2,'U');
            VerticalBeginPos=TransCoord(CirclePathEnd,90,25,RIGHT_DOWN);
            i+=1;
        }
        else{
            DrawBasePair(Painter,VerticalBeginPos.x(),VerticalBeginPos.y()+Dy*k,MatchRNASeq[i],25,90);
            k++;
        }
    }
    Painter->setPen(Qt::black);//画底部的3'标识
    Painter->drawText(VerticalBeginPos.x(),VerticalBeginPos.y()+Dy*k,"3'");
    SetPointCoord(BackConservativeSeqBegin,CirclePathEnd.x(),CirclePathEnd.y()+Dy*(k-1));
    DrawConservativeSeq(Painter,BackConservativeSeqBegin);
}
void PistolPainter::DrawConservativeSeq(QPainter *Painter,const QPoint& BeginCoord)
{
    string ConservativeSeq="GAUAAAUU";
    double Dx=15,Dy=15;
    unsigned int i=0;
    for(;i<ConservativeSeq.length();++i){
        DrawBase(Painter,BeginCoord.x()-Dx*(i+1),BeginCoord.y(),ConservativeSeq[i]);
    }
    QPoint VerticalBeginPos(BeginCoord.x()-Dx*(i+1),BeginCoord.y());
    QPoint Mark5Cooord(TransCoord(VerticalBeginPos,90,25));//画底部的5'标识
    Mark5Cooord.ry()+=PICTURE_FONT_SIZE*1.5;
    Painter->setPen(Qt::black);
    Painter->drawText(Mark5Cooord,"5'");
    ConservativeSeq="GCAC";
    for(i=0;i<ConservativeSeq.length();++i){
        DrawBasePair(Painter,VerticalBeginPos.x(),VerticalBeginPos.y()-Dy*i,ConservativeSeq[i],25,90);
    }
    QPoint CircleBeginPos(VerticalBeginPos.x(),VerticalBeginPos.y()-Dy*3);
    ConservativeSeq="CGGGAUUG";
    QPoint CircleEndPos=TransCoord(CircleBeginPos,90,25);
    DrawCriclePathBase(Painter,ConservativeSeq,CircleBeginPos,CircleEndPos,30,false);
}
void TwisterPainter::DrawRibozymeImage(const string &MatchRNASeq, unsigned int PictureWidth, unsigned int PictureHeight, QPainter *Painter)
{
    QFont PictureFont("Microsoft YaHei",PICTURE_FONT_SIZE,75);
    Painter->setFont(PictureFont);
    smatch RegexResult;
    regex RegexPartern("[AGCU]AA[AGCU]{4}GC"); //NAANNNNGC
    int BeginX=((sqrt(5)-1)/2)*PictureWidth;
    int BeginY=PictureHeight-80;
    double Dy=16,Dx=16;
    if(regex_search(MatchRNASeq.begin(),MatchRNASeq.end(),RegexResult,RegexPartern)){
        int i=0; // 匹配部分之前的计数
        QPoint Mark5Pos(BeginX-10,BeginY+20);
        Painter->drawText(Mark5Pos,"5'");
        Painter->drawText(TransCoord(Mark5Pos,90,47,RIGHT_DOWN),"3'");
        QPoint ConservativeSeqEnd;
        for(auto it=MatchRNASeq.begin();it!=MatchRNASeq.end();++it){
            if(it<RegexResult[0].first){
                DrawBasePair(Painter,BeginX,BeginY-Dy*i,*it,25,90,RIGHT_DOWN);
                i++;
            }
            else if(it==RegexResult[0].first) {
                QPoint ConservativeSeqBegin;
                SetPointCoord(ConservativeSeqBegin,BeginX,BeginY-Dy*(i-1));
                ConservativeSeqEnd=DrawConservativeSeq(Painter,MatchRNASeq.substr(i,9),ConservativeSeqBegin);
                it+=8;
                i=0;
            }
            else{
                DrawBasePair(Painter,ConservativeSeqEnd.x()-Dx*i,ConservativeSeqEnd.y(),*it,25,0);
                i++;
            }
        }
    }
    return;
}
QPoint TwisterPainter::DrawConservativeSeq(QPainter *Painter,const string &MatchRNASeq, const QPoint &BeginPos)
{
    DrawBase(Painter,BeginPos.x()-15,BeginPos.y()-18,MatchRNASeq[0]); //先画前三个不配对
    DrawBase(Painter,BeginPos.x()-15-PICTURE_FONT_SIZE,BeginPos.y()-18-2*PICTURE_FONT_SIZE,MatchRNASeq[1]);
    DrawBase(Painter,BeginPos.x()-15,BeginPos.y()-18-3.5*PICTURE_FONT_SIZE,MatchRNASeq[2]);
    QPoint FrontPartEndPos;//画不配对的那些右边的5个
    SetPointCoord(FrontPartEndPos,BeginPos.x(),BeginPos.y()-80);
    QPoint FrontCircleBegin=TransCoord(BeginPos,90,25,RIGHT_DOWN);
    QPoint FrontCircleEnd=TransCoord(FrontPartEndPos,90,25,RIGHT_DOWN);
    string CircleSeq="GGGAG";
    DrawCriclePathBase(Painter,CircleSeq,FrontCircleBegin,FrontCircleEnd,45,false,LEFT_TOP);
    unsigned int dy=16,i=0; //画中间配对的四个
    for(i=4;i<8;++i){//i应该是从3开始的，但是我写完了才发现这些，而且下面有些坐标的计算和错误的结果有关，所以将错就错MatchRNASeq的下标就是i-1了
        DrawBasePair(Painter,FrontPartEndPos.x(),FrontPartEndPos.y()-dy*(i-4),MatchRNASeq[i-1],25,90,RIGHT_DOWN);
    }
    //画AAAUA左边的GC
    DrawBase(Painter,FrontPartEndPos.x()-1.5*PICTURE_FONT_SIZE,FrontPartEndPos.y()-dy*(i-4.5),'G');
    DrawBase(Painter,FrontPartEndPos.x()-2.5*PICTURE_FONT_SIZE,FrontPartEndPos.y()-dy*(i-3.5),'C');
    //画AAAUA序列
    QPoint RightBackPartBegin=TransCoord(QPoint(FrontPartEndPos.x(),FrontPartEndPos.y()-dy*(i-5)),90,25,RIGHT_DOWN);
    DrawBase(Painter,RightBackPartBegin.x()+PICTURE_FONT_SIZE,RightBackPartBegin.y()-PICTURE_FONT_SIZE,'A');
    DrawBase(Painter,RightBackPartBegin.x()+2*PICTURE_FONT_SIZE,RightBackPartBegin.y()-PICTURE_FONT_SIZE*2,'A');
    DrawBase(Painter,RightBackPartBegin.x()+1.5*PICTURE_FONT_SIZE,RightBackPartBegin.y()-PICTURE_FONT_SIZE*3.5,'A');
    DrawBase(Painter,RightBackPartBegin.x(),RightBackPartBegin.y()-PICTURE_FONT_SIZE*4,'U');
    DrawBase(Painter,RightBackPartBegin.x()-1.5*PICTURE_FONT_SIZE,RightBackPartBegin.y()-PICTURE_FONT_SIZE*4.5,'A');
    //画AAAUA之后的部分
    QPoint FinalPartBegin(RightBackPartBegin.x()-2.5*PICTURE_FONT_SIZE,RightBackPartBegin.y()-PICTURE_FONT_SIZE*5.5);
    string seq="GGCC";
    for(i=0;i<seq.length();++i){
        DrawBasePair(Painter,FinalPartBegin.x(),FinalPartBegin.y()-dy*i,seq[i],25,90);
    }
    CircleSeq="CGAACCCU";
    QPoint Begin(FinalPartBegin.x(),FinalPartBegin.y()-dy*(i-1));
    QPoint End=TransCoord(Begin,90,25);
    DrawCriclePathBase(Painter,CircleSeq,Begin,End,30,false,LEFT_TOP);
    //生成后方序列开始点的坐标
    QPoint NextBegin=TransCoord(FinalPartBegin,90,25);
    NextBegin.rx()-=PICTURE_FONT_SIZE;
    NextBegin.ry()+=PICTURE_FONT_SIZE;
    return TransCoord(NextBegin,0,25,RIGHT_DOWN);
}
void HammerHeadPainter::DrawRibozymeImage(const string &MatchRNASeq, unsigned int PictureWidth, unsigned int PictureHeight, QPainter *Painter)
{
    QFont PictureFont("Microsoft YaHei",PICTURE_FONT_SIZE,75);
    Painter->setFont(PictureFont);
    double Dx=14,Dy=20,ImageBeginX=((sqrt(5)-1)/2)*PictureWidth,ImageBeginY=75;
    QPoint SecondMatchPartBegin;
    unsigned int k=0;
    QPoint Mark5Coord(ImageBeginX,ImageBeginY-Dy);
    Painter->drawText(Mark5Coord,"5'");
    Painter->drawText(TransCoord(Mark5Coord,90,25),"3'");
    for(unsigned int i=0;i<MatchRNASeq.length();++i){
        if(i<MatchRNASeq.length()-HammerTCBackPos+1){
            DrawBasePair(Painter,ImageBeginX,ImageBeginY+Dy*i,MatchRNASeq[i],25,90);
        }
        else if(i==MatchRNASeq.length()-HammerTCBackPos+1){
            QPoint Begin=TransCoord(QPoint(ImageBeginX,ImageBeginY+Dy*(i-1)),90,25);
            DrawBase(Painter,ImageBeginX+PICTURE_FONT_SIZE,ImageBeginY+Dy*(i-1)+PICTURE_FONT_SIZE,MatchRNASeq[i]);
            SecondMatchPartBegin=DrawConservativeSeqPart1(Painter,Begin);
        }
        else{
            DrawBasePair(Painter,SecondMatchPartBegin.x()+Dx*k,SecondMatchPartBegin.y(),MatchRNASeq[i],25,0,RIGHT_DOWN);
            k++;
        }
    }
    QPoint ConservativeSeqPart2Begin;
    SetPointCoord(ConservativeSeqPart2Begin,SecondMatchPartBegin.x()+Dx*(k-1)+12,SecondMatchPartBegin.y()+34);
    DrawConservativeSeqPart2(Painter,ConservativeSeqPart2Begin);
}
QPoint HammerHeadPainter::DrawConservativeSeqPart1(QPainter *Painter,QPoint &BeginPos)
{
    string seq="AAG";
    int r=60;
    QPoint AAGSeqEndPos;//AAG所在圆形轨迹的终点
    SetPointCoord(AAGSeqEndPos,BeginPos.x()-60,BeginPos.y()+30);
    DrawCriclePathBase(Painter,seq,BeginPos,AAGSeqEndPos,r,false,RIGHT_DOWN);
    //画第一段保守序列配对的那对CG
    DrawBasePair(Painter,AAGSeqEndPos.x(),AAGSeqEndPos.y(),'C',25,0,RIGHT_DOWN);
    //画保守序列配对GC左边的圆
    QPoint FirstCircleEnd=TransCoord(AAGSeqEndPos,0,25,RIGHT_DOWN);
    seq="AGGUCA";
    DrawCriclePathBase(Painter,seq,AAGSeqEndPos,FirstCircleEnd,25,false);
    //GC左边圆的终点也是右边圆的起点
    QPoint SeccondCircleEnd;
    seq="AGUAGUC";
    SetPointCoord(SeccondCircleEnd,FirstCircleEnd.x()+100,FirstCircleEnd.y());
    DrawCriclePathBase(Painter,seq,FirstCircleEnd,SeccondCircleEnd,50,false,LEFT_TOP);
    //返回下一部分的起点
    return TransCoord(SeccondCircleEnd,0,25,LEFT_TOP);
}
void HammerHeadPainter::DrawConservativeSeqPart2(QPainter *Painter, const QPoint &BeginPos)
{
    string seq="UAAAAUA";
    unsigned int dy=18,i=0;
    for(;i<seq.length();++i){
        DrawBase(Painter,BeginPos.x(),BeginPos.y()+dy*i,seq[i]);
    }
    //画最底下的A
    DrawBase(Painter,BeginPos.x()-0.5*PICTURE_FONT_SIZE,BeginPos.y()+160,'A');
    QPoint SecondSeqBegin;
    SetPointCoord(SecondSeqBegin,BeginPos.x()-14,BeginPos.y()+dy*(i-1)+10);
    seq="UCUG";
    for(i=0;i<seq.length();++i){
        DrawBasePair(Painter,SecondSeqBegin.x()-18*i,SecondSeqBegin.y(),seq[i],25,0,RIGHT_DOWN);
    }

    //画带圆的部分
    QPoint CircleBegin,CircleEnd;
    SetPointCoord(CircleBegin,SecondSeqBegin.x()-18*(i-1),SecondSeqBegin.y());
    CircleEnd=TransCoord(CircleBegin,0,25,RIGHT_DOWN);
    seq="AGGACCAA";
    DrawCriclePathBase(Painter,seq,CircleBegin,CircleEnd,30,false);
}
RibozymeImagePainter::RibozymeImagePainter(const string& RibozymeType)
{
    if(RibozymeType==TWISTER_SISTER){
        ImagePainter=new TwisterSisterPainter();
    }
    else if(RibozymeType==PISTOL){
        ImagePainter=new PistolPainter();
    }
    else if(RibozymeType==TWISTER){
        ImagePainter=new TwisterPainter();
    }
    else if(RibozymeType==HAMMER_HEAD){
        ImagePainter=new HammerHeadPainter();
    }
}
void RibozymeImagePainter::DrawRibozymeImage(const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)
{
    if(ImagePainter==NULL || MatchRNASeq.empty() ){
        return;
    }
    ImagePainter->DrawRibozymeImage(MatchRNASeq,PictureWidth,PictureHeight,Painter);
}
RibozymeImagePainter::~RibozymeImagePainter()
{
    delete ImagePainter;
}
