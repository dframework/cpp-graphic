#include <dframework/gd/GDImage.h>
#include "gd.h"

namespace dframework {

#ifdef IMAGE
#undef IMAGE
#endif
#define IMAGE(x) ((gdImagePtr)(x))
#define FONT(x)  ((gdFontPtr)(x))

  GDImage::GDImage(){
      m_image = NULL;
      m_width = 0;
      m_height = 0;
      m_is_true_color = false;
  }

  GDImage::~GDImage(){
      release();
  }

  void GDImage::release(){
      if(m_image){
          gdImageDestroy(IMAGE(m_image));
          m_image = NULL;
      }
      m_width = 0;
      m_height = 0;
      m_is_true_color = false;
  }

  sp<Retval> GDImage::create(size_t w, size_t h){
      sp<Retval> retval;

      release();

      m_width = w;
      m_height = h;
      if( !(m_image = gdImageCreate(w, h)) )
          return DFW_RETVAL_NEW_MSG(DFW_E_NOMEM, ENOMEM
                     , "Not create gd image. w=%u, h=%u", w, h);

      return NULL;
  }

  sp<Retval> GDImage::createTrueColor(size_t w, size_t h){
      sp<Retval> retval;

      release();

      m_width = w;
      m_height = h;
      if( !(m_image = gdImageCreateTrueColor(w, h)) )
          return DFW_RETVAL_NEW_MSG(DFW_E_NOMEM, ENOMEM
                     , "Not create gd true color image. w=%u, h=%u", w, h);

      m_is_true_color = true;
      return NULL;
  }

  int GDImage::color(int r, int g, int b){
      if( m_is_true_color )
          return gdTrueColor(r, g, b);
      return gdImageColorAllocate(IMAGE(m_image), r, g, b);
  }

  int GDImage::trueColor(int r, int g, int b){
      return gdTrueColor(r, g, b);
  }

  int GDImage::trueColorAlpha(int r, int g, int b, int c){
      return gdTrueColorAlpha(r, g, b, c); //gdAlphaTransparent/c
  }

  int GDImage::getPixel(int x, int y){
      return gdImageGetPixel(IMAGE(m_image), x, y);
  }

  int GDImage::getTrueColorPixel(int x, int y){
      return gdImageGetTrueColorPixel(IMAGE(m_image), x, y);
  }

  void GDImage::alphaBlending(int v){
      gdImageAlphaBlending(IMAGE(m_image), v);
  }

  void GDImage::saveAlpha(int v){
      gdImageSaveAlpha(IMAGE(m_image), v);
  }

  void GDImage::AABlend(){
      gdImageAABlend(IMAGE(m_image));
  }

  void GDImage::pixel(int x, int y, int color){
      gdImageSetPixel(IMAGE(m_image), x, y, color);
  }

  void GDImage::line(int x1, int y1, int x2, int y2, int color){
      gdImageLine(IMAGE(m_image), x1, y1, x2, y2, color);
  }

  void GDImage::dashedLine(int x1, int y1, int x2, int y2, int color){
      gdImageDashedLine(IMAGE(m_image), x1, y1, x2, y2, color);
  }

#if 0
  void GDImage::polygon(gdPointPtr points, int pointsTotal, int color){
      gdImagePolygon((gdImagePtr)m_image, (gdPointPtr)points, pointsTotal, color);
  }

  void GDImage::openPolygon(gdPointPtr points, int pointsTotal, int color){
      gdImageOpenPolygon((gdImagePtr)m_image, (gdPointPtr)points, pointsTotal, color);
  }

  void GDImage::fillPolygon(gdPointPtr points, int pointsTotal, int color){
      gdImageFilledPolygon((gdImagePtr)m_image, (gdPointPtr)points, pointsTotal, color);
  }
#endif

  void GDImage::rectangle(int x1, int y1, int x2, int y2, int color){
      gdImageRectangle(IMAGE(m_image), x1, y1, x2, y2, color);
  }

  void GDImage::fillRectangle(int x1, int y1, int x2, int y2, int color){
      gdImageFilledRectangle(IMAGE(m_image), x1, y1, x2, y2, color);
  }

  void GDImage::arc(int cx, int cy, int w, int h, int s, int e, int color){
      gdImageArc(IMAGE(m_image), cx, cy, w, h, s, e, color);
  }

  void GDImage::fillArc(int cx, int cy, int w, int h
                        , int s, int e, int color, int style)
  {
      gdImageFilledArc(IMAGE(m_image), cx, cy, w, h, s, e, color, style);
  }

  void GDImage::fillEllipse(int cx, int cy, int w, int h, int color){
      gdImageFilledEllipse(IMAGE(m_image), cx, cy, w, h, color);
  }

  void GDImage::fillToBorder(int x, int y, int border, int color){
      gdImageFillToBorder(IMAGE(m_image), x, y, border, color);
  }

  void GDImage::fill(int x, int y, int color){
      gdImageFill(IMAGE(m_image), x, y, color);
  }

  void GDImage::setAntiAliased(int c){
      gdImageSetAntiAliased(IMAGE(m_image), c);
  }

#if 0
  void GDImage::setAntiAliasedDontBlend(int c){
      gdImageSetAntiAliasedDontBlend((gdImagePtr)m_image, c);
  }
#endif

  void GDImage::setBrush(sp<GDImage>& brush){
      gdImageSetBrush(IMAGE(m_image), IMAGE(brush->m_image));
  }

  void GDImage::setTile(sp<GDImage>& tile){
      gdImageSetTile(IMAGE(m_image), IMAGE(tile->m_image));
  }

  void GDImage::setStyle(int *style, int styleLength){
      gdImageSetStyle(IMAGE(m_image), style, styleLength);
  }

  void GDImage::setThickness(int thickness){
      gdImageSetThickness(IMAGE(m_image), thickness);
  }

  void GDImage::setClip(int x1, int y1, int x2, int y2){
      gdImageSetClip(IMAGE(m_image), x1, y1, x2, y2);
  }

  void GDImage::getClip(int* x1, int* y1, int* x2, int* y2){
      gdImageGetClip(IMAGE(m_image), x1, y1, x2, y2);
  }

  int  GDImage::boundsSafe(int x, int y){
      return gdImageBoundsSafe(IMAGE(m_image), x, y);
  }

  void GDImage::resolution(const unsigned int res_x, const unsigned int res_y){
      gdImageSetResolution(IMAGE(m_image), res_x, res_y);
  }

  // ---------------------------------------------------------------

  void GDImage::character(dfw_gdfont_t font, int x, int y, int c, int color){
      gdImageChar(IMAGE(m_image), FONT(font), x, y, c, color);
  }

  void GDImage::charUp(dfw_gdfont_t font, int x, int y, int c, int color){
      gdImageCharUp(IMAGE(m_image), FONT(font), x, y, c, color);
  }

  void GDImage::string(dfw_gdfont_t f, int x,int y,const char* s,int c){
      gdImageString(IMAGE(m_image), FONT(f), x, y, (unsigned char*)s, c);
  }

  void GDImage::stringUp(dfw_gdfont_t f, int x, int y, const char* s, int c){
      gdImageStringUp(IMAGE(m_image), FONT(f), x, y, (unsigned char*)s, c);
  }

  void GDImage::string16(dfw_gdfont_t f,int x,int y, const short* s,int c){
      gdImageString16(IMAGE(m_image), FONT(f), x, y, (unsigned short*)s, c);
  }

  void GDImage::stringUp16(dfw_gdfont_t f, int x, int y,const short* s,int c){
      gdImageStringUp16(IMAGE(m_image), FONT(f), x, y, (unsigned short*)s, c);
  }

  void GDImage::small(int x, int y, const char* s, int c, int type){
      switch(type){
      case 1 :
        if(s){
          string(GDFont::getFont_l(GDFont::SMALL), x-(strlen(s)*6),y,s,c);
        }
        break;

      case 2 :
        if(s){
          string(GDFont::getFont_l(GDFont::SMALL), x-(strlen(s)*3),y,s,c);
        }
        break;

      default:
        string(GDFont::getFont_l(GDFont::SMALL), x,y,s,c);
        break;
      }
  }

  void GDImage::small(int x, int y, const String& s, int c, int type){
      switch(type){
      case 1 :
        string(GDFont::getFont_l(GDFont::SMALL), x-(s.length()*6),y,s,c);
        break;

      case 2 :
        string(GDFont::getFont_l(GDFont::SMALL), x-(s.length()*3),y,s,c);
        break;

      default:
        string(GDFont::getFont_l(GDFont::SMALL), x,y,s,c);
        break;
      }
  }

  // ---------------------------------------------------------------

  char* GDImage::stringFT(int* brect, int fg, char *fontname
                        , double ptsize, double angle, int x, int y
                        , char *string){
	gdFTUseFontConfig(1);
      return gdImageStringFT(NULL, brect, fg, fontname
                        , ptsize, angle, x, y, string);
      //return gdImageStringFT((gdImagePtr)m_image, brect, fg, fontname
      //                  , ptsize, angle, x, y, string);
  }

#if 0
  char* GDImage::stringFTEx(int *brect, int fg, char *fontname
                        , double ptsize, double angle, int x, int y
                        , gdFTStringExtraPtr strex){
      return gdImageStringFTEx((gdImagePtr)m_image, brect, fg, fontname
                        , ptsize, angle, x, y, strex);
  }
#endif

  char* GDImage::stringFTCircle(int cx, int cy, double radius
                              , double textRadius, double fillPortion
                              , char *font, double points
                              , char *top, char *bottom, int fgcolor){
      return gdImageStringFTCircle((gdImagePtr)m_image, cx, cy, radius
                              , textRadius, fillPortion, font, points
                              , top, bottom, fgcolor);
  }

  char* GDImage::stringTTF(int* brect, int fg, char *fontname
                              , double ptsize, double angle, int x, int y
                              , char *string){
      return gdImageStringTTF((gdImagePtr)m_image, brect, fg, fontname
                              , ptsize, angle, x, y, string);
  }

  sp<Retval> GDImage::savePng(const char* path){
      FILE* fd;
      if( !(fd = fopen(path, "wb")) ){
          return DFW_RETVAL_NEW_MSG(DFW_ERROR, errno
                     , "Not open %s", path);
      }
      gdImagePng((gdImagePtr)m_image, fd);
      fclose(fd);
      return NULL;
  }

  sp<Retval> GDImage::saveJpeg(const char* path, int quality){
      FILE* fd;
      if( !(fd = fopen(path, "wb")) ){
          return DFW_RETVAL_NEW_MSG(DFW_ERROR, errno
                     , "Not open %s", path);
      }
      gdImageJpeg((gdImagePtr)m_image, fd, quality);
      fclose(fd);
      return NULL;
  }


};

