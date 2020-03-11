#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H
#include"common.h"
#include<QPainter>
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
          void DrawBase(QPainter *Painter, unsigned int x, unsigned int y, char Base);
          void DrawBasePair(QPainter *Painter, unsigned int BaseX, unsigned int BaseY, char Base,
                            unsigned int PairX, unsigned int PairY);
};
class TwisterSisterPainter:public ImagePainterBase{
    public:
        void DrawRibozymeImage(const string& RibozymeSeq,const string& MatchRNASeq,unsigned int PictureWidth,unsigned int PictureHeight,QPainter *Painter);
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
