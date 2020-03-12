#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H
#include"common.h"
#include<QPainter>
#include<QtMath>
#define LEFT_TOP 1
#define RIGHT_DOWN 0
#define PICTURE_FONT_SIZE 16
#define PICTURE_BASE_SPACE 2
#define TWISTER_SISTER_BEGIN_X PictureWidth-50
#define TWISTER_SISTER_BEGIN_Y 50
using namespace std;
class ImagePainterBase{
    public:
          virtual void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter)=0;
          virtual ~ImagePainterBase(){}
    protected:
          void DrawBase(QPainter *Painter, double x, double y, char Base);
          void DrawBasePair(QPainter *Painter, double BaseX, double BaseY, char Base,double PairGap,double Degrees,bool Oritation=LEFT_TOP);
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
