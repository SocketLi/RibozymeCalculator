#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H
#include"common.h"
#include<QPainter>
#include<QtMath>
#define LEFT_TOP 1
#define RIGHT_DOWN 0
#define PICTURE_FONT_SIZE 16
#define LOW_BASE_SPACE 2 //DrawMatchLine中为了避免画出的匹配线离低处碱基太近造成的不美观
#define TWISTER_SISTER_BEGIN_X PictureWidth-50
#define TWISTER_SISTER_BEGIN_Y 50
#define K 0.5/45 // DrawMatchLine中使用的修正因子，使斜率变化时可以让匹配线向中间移动一些
using namespace std;
class ImagePainterBase{
    public:
          virtual void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)=0;
          virtual ~ImagePainterBase(){}
    protected:
          void DrawBase(QPainter *Painter, int x, int y, char Base);
          void DrawBasePair(QPainter *Painter, int BaseX, int BaseY, char Base,double PairGap,double Degrees,bool Oritation=LEFT_TOP);
          QPoint TransCoord(const QPoint& OriginCoord,double Degrees,double PairGap,bool Oritation=LEFT_TOP);
          void DrawMatchLine(QPainter* Painter,const QPoint& BaseCoord,const QPoint& MatchedBaseCoord,double Degrees);
          void SetPointCoord(QPoint& Point,int x,int y);
          double Dy(double Dx,double Degrees);
};
class TwisterSisterPainter:public ImagePainterBase{
    public:
        void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter);
    protected:
        void DrawConservativeSeq(QPainter *Painter,double BeginX,double BeginY,double Degrees);

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
