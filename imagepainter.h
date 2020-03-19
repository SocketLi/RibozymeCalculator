#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H
#include"common.h"
#include<QPainter>
#include<QtMath>
#define LEFT_TOP 1
#define RIGHT_DOWN 0
#define PICTURE_FONT_SIZE 12
#define LOW_BASE_SPACE 2 //DrawMatchLine中为了避免画出的匹配线离低处碱基太近造成的不美观,定义为两个小正方形右下角连线和下方小正方形底边所组成的夹角的对边的溢出量
#define TWISTER_SISTER_BEGIN_X (sqrt(5)-1)/2*PictureWidth
#define TWISTER_SISTER_BEGIN_Y 0.28*PictureHeight
#define K 0.5/45 // DrawMatchLine中使用的修正因子，使斜率变化时可以让匹配线向中间移动一些
using namespace std;
class ImagePainterBase{
    public:
          virtual void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)=0;
          virtual ~ImagePainterBase(){}
    protected:
          void DrawBase(QPainter *Painter, int x, int y, char Base);
          void DrawBasePair(QPainter *Painter, int BaseX, int BaseY, char Base,double PairGap,double Degrees,bool Oritation=LEFT_TOP);
          QPoint TransCoord(const QPoint& OriginCoord,double Degrees,double PairGap,bool Oritation=LEFT_TOP);//生成互补配对的碱基坐标,参数degrees是OriginCoord所在线的夹角
          void DrawMatchLine(QPainter* Painter,const QPoint& BaseCoord,const QPoint& MatchedBaseCoord,double Degrees);//Degrees是matchline与x正半轴的夹角
          void SetPointCoord(QPoint& Point,int x,int y);
          void DrawCriclePathBase(QPainter *Painter,const string& RNAseq,QPoint &BeginCoord,QPoint &EndCoord,double r,bool IsClockWise,bool Oritation=LEFT_TOP);
          double GetLineDegree(const QPoint& p1,const QPoint& p2);
          QPoint GetCenterCoord(QPoint &BeginCoord,QPoint &EndCoord,double r,bool Oritation=LEFT_TOP);
          double Dy(double Dx,double Degrees);
};
class TwisterSisterPainter:public ImagePainterBase{
    public:
        void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter);
    protected:
        void DrawConservativeSeq(QPainter *Painter,int BeginX,int BeginY,double Degrees);

};
class TwisterPainter:public ImagePainterBase{
    public:
        void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter);
};
class PistolPainter:public ImagePainterBase{
    public:
        void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter);
};
class RibozymeImagePainter{
    public:
        RibozymeImagePainter(const string& RibozymeType);
        ~RibozymeImagePainter();
        void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter);
    private:
        ImagePainterBase* ImagePainter;
};
#endif // IMAGEPAINTER_H
