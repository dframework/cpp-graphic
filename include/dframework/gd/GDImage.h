#ifndef DFRAMEWORK_GD_GDIMAGE_H
#define DFRAMEWORK_GD_GDIMAGE_H

#include <dframework/base/Retval.h>
#include <dframework/gd/GDFont.h>

namespace dframework {

  class GDImage : public Object
  {
  private:
      void* m_image; // gdImagePtr

      size_t m_width;
      size_t m_height;
      bool   m_is_true_color;

  public:
      GDImage();
      virtual ~GDImage();

      inline size_t width() { return m_width; }
      inline size_t height() { return m_height; }
      inline bool isTrueColor() { return m_is_true_color; }

      void release();
      sp<Retval> create(size_t w, size_t h);
      sp<Retval> createTrueColor(size_t w, size_t h);

      int color(int r, int g, int b);
      int trueColor(int r, int g, int b);
      int trueColorAlpha(int r, int g, int b, int c=10);
      inline int alpha(int r, int g, int b, int c=10){
          return trueColorAlpha(r,g,b,c);
      }

      int getPixel(int x, int y);
      int getTrueColorPixel(int x, int y);

      void alphaBlending(int v);
      void saveAlpha(int v);

      void AABlend();

      void pixel(int x, int y, int color);
      void line(int x1, int y1, int x2, int y2, int color);
      void dashedLine(int x1, int y1, int x2, int y2, int color);

#if 0
      void polygon(gdPointPtr points, int pointsTotal, int color);
      void openPolygon(gdPointPtr points, int pointsTotal, int color);
      void fillPolygon(gdPointPtr points, int pointsTotal, int color);
#endif

      void rectangle(int x1, int y1, int x2, int y2, int color);
      void fillRectangle(int x1, int y1, int x2, int y2, int color);

      void arc(int cx, int cy, int w, int h, int s, int e, int color);
      void fillArc(int cx, int cy, int w, int h
                        , int s, int e, int color, int style);
      void fillEllipse(int cx, int cy, int w, int h, int color);
      void fillToBorder(int x, int y, int border, int color);
      void fill(int x, int y, int color);
      void setAntiAliased(int c);
#if 0
      void setAntiAliasedDontBlend(int c);
#endif
      void setBrush(sp<GDImage>& brush);
      void setTile(sp<GDImage>& tile);
      void setStyle(int *style, int styleLength);
      void setThickness(int thickness);

      void setClip(int x1, int y1, int x2, int y2);
      void getClip(int* x1, int* y1, int* x2, int* y2);

      void resolution(const unsigned int res_x, const unsigned int res_y);

      int  boundsSafe(int x, int y);

      // ------------------------------------------------------------

      void character(dfw_gdfont_t f,   int x, int y, int c, int color);
      inline void character(GDFont::type_t t, int x, int y, int c, int color){
          character(GDFont::getFont_l(t), x, y, c, color);
      }
      inline void character(GDFont* f, int x, int y, int c, int color){
          character(f->getFont(), x, y, c, color);
      }
      inline void character(sp<GDFont>& f, int x, int y, int c, int color){
          character(f->getFont(), x, y, c, color);
      }

      // ------------------------------------------------------------

      void charUp(dfw_gdfont_t font,int x, int y, int c, int color);
      inline void charUp(GDFont::type_t t, int x, int y, int c, int color){
          character(GDFont::getFont_l(t), x, y, c, color);
      }
      inline void charUp(GDFont* f, int x, int y, int c, int color){
          character(f->getFont(), x, y, c, color);
      }
      inline void charUp(sp<GDFont>& f, int x, int y, int c, int color){
          character(f->getFont(), x, y, c, color);
      }

      // ------------------------------------------------------------

      void string(dfw_gdfont_t f, int x, int y, const char* s,int c);
      inline void string(GDFont::type_t f,int x,int y,const char* s,int c){
          string(GDFont::getFont_l(f), x, y, s, c);
      }
      inline void string(GDFont* f,int x, int y, const char* s, int c){
          string(f->getFont(), x, y, s, c);
      }
      inline void string(sp<GDFont>& f,int x, int y, const char* s, int c){
          string(f->getFont(), x, y, s, c);
      }

      // ------------------------------------------------------------

      inline void string(dfw_gdfont_t f,int x,int y,const String& s, int c){
          string(f, x, y, s.toChars(), c);
      }
      inline void string(GDFont::type_t t,int x,int y,const String& s,int c){
          string(GDFont::getFont_l(t), x, y, s, c);
      }
      inline void string(GDFont* f,int x, int y, const String& s, int c){
          string(f->getFont(), x, y, s, c);
      }
      inline void string(sp<GDFont>& f,int x, int y, const String& s, int c){
          string(f->getFont(), x, y, s, c);
      }

      // ------------------------------------------------------------

      inline void tiny(int x, int y, const char* s, int c){
          string(GDFont::getFont_l(GDFont::TINY), x, y, s, c);
      }
      void small(int x, int y, const char* s, int c, int t=0);
      inline void bold(int x, int y, const char* s, int c){
          string(GDFont::getFont_l(GDFont::BOLD), x, y, s, c);
      }
      inline void large(int x, int y, const char* s, int c){
          string(GDFont::getFont_l(GDFont::LARGE), x, y, s, c);
      }
      inline void giant(int x, int y, const char* s, int c){
          string(GDFont::getFont_l(GDFont::GIANT), x, y, s, c);
      }

      // ------------------------------------------------------------

      inline void tiny(int x, int y, const String& s, int c){
          string(GDFont::getFont_l(GDFont::TINY), x, y, s, c);
      }
      void small(int x, int y, const String& s, int c, int t=0);
      inline void bold(int x, int y, const String& s, int c){
          string(GDFont::getFont_l(GDFont::BOLD), x, y, s, c);
      }
      inline void large(int x, int y, const String& s, int c){
          string(GDFont::getFont_l(GDFont::LARGE), x, y, s, c);
      }
      inline void giant(int x, int y, const String& s, int c){
          string(GDFont::getFont_l(GDFont::GIANT), x, y, s, c);
      }

      // ------------------------------------------------------------

      void stringUp(dfw_gdfont_t f, int x, int y, const char* s, int c);
      inline void stringUp(GDFont::type_t t,int x,int y,const char* s,int c){
          stringUp(GDFont::getFont_l(t), x, y, s, c);
      }
      inline void stringUp(GDFont* f,int x, int y, const char* s, int c){
          stringUp(f->getFont(), x, y, s, c);
      }
      inline void stringUp(sp<GDFont>& f,int x, int y, const char* s, int c){
          stringUp(f->getFont(), x, y, s, c);
      }

      // ------------------------------------------------------------

      inline void stringUp(dfw_gdfont_t f, int x,int y,const String& s,int c){
          stringUp(f, x, y, s.toChars(), c);
      }
      inline void stringUp(GDFont::type_t t,int x,int y,const String& s,int c){
          stringUp(GDFont::getFont_l(t), x, y, s, c);
      }
      inline void stringUp(GDFont* f,int x, int y, const String& s, int c){
          stringUp(f->getFont(), x, y, s, c);
      }
      inline void stringUp(sp<GDFont>& f,int x, int y, const String& s, int c){
          stringUp(f->getFont(), x, y, s, c);
      }

      // ------------------------------------------------------------

      void string16(dfw_gdfont_t f, int x,int y,const short* s,int c);
      inline void string16(GDFont::type_t t,int x,int y,const short* s,int c){
          string16(GDFont::getFont_l(t), x, y, s, c);
      }
      inline void string16(GDFont* f,int x, int y, const short* s, int c){
          string16(f->getFont(), x, y, s, c);
      }
      inline void string16(sp<GDFont>& f,int x, int y, const short* s, int c){
          string16(f->getFont(), x, y, s, c);
      }

      void stringUp16(dfw_gdfont_t f, int x,int y,const short* s,int c);
      inline void stringUp16(GDFont::type_t t,int x,int y,const short* s,int c){
          stringUp16(GDFont::getFont_l(t), x, y, s, c);
      }
      inline void stringUp16(GDFont* f,int x, int y, const short* s, int c){
          stringUp16(f->getFont(), x, y, s, c);
      }
      inline void stringUp16(sp<GDFont>& f,int x,int y,const short* s,int c){
          stringUp16(f->getFont(), x, y, s, c);
      }

      // ------------------------------------------------------------

      char* stringFT(int* brect, int fg, char *fontname
                          , double ptsize, double angle, int x, int y
                          , char *string);
#if 0
      char* stringFTEx(int *brect, int fg, char *fontname
                          , double ptsize, double angle, int x, int y
                         , gdFTStringExtraPtr strex);
#endif
      char* stringFTCircle(int cx, int cy, double radius
                          , double textRadius, double fillPortion
                          , char *font, double points
                          , char *top, char *bottom, int fgcolor);
      char* stringTTF(int* brect, int fg, char *fontname
                          , double ptsize, double angle, int x, int y
                          , char *string);


      sp<Retval> savePng(const char* path);
      inline sp<Retval> savePng(const String& path){
          return savePng(path.toChars());
      }

      sp<Retval> saveJpeg(const char* path, int quality=-1);
      inline sp<Retval> saveJpeg(const String& path, int quality=-1){
          return saveJpeg(path.toChars(), quality);
      }

  };

};

#endif /* DFRAMEWORK_GD_GDIMAGE_H */

