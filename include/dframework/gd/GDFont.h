#ifndef DFRAMEWORK_GD_GDFONT_H
#define DFRAMEWORK_GD_GDFONT_H

#include <dframework/base/Retval.h>

typedef void* dfw_gdfont_t;

namespace dframework {

  class GDFont : public Object
  {
  public:
      enum type_t {
          TINY, SMALL, BOLD, LARGE, GIANT
      };

  public:
      static int useFTConfig(bool flag);
      static int cacheSetup();
      static void cacheShutdown();

      inline static GDFont* getTiny()  { return new GDFont(TINY); }
      inline static GDFont* getSmall() { return new GDFont(SMALL); }
      inline static GDFont* getBold()  { return new GDFont(BOLD); }
      inline static GDFont* getLarge() { return new GDFont(LARGE); }
      inline static GDFont* getGiant() { return new GDFont(GIANT); }

      static dfw_gdfont_t getFont_l(type_t type);

#ifdef DFW_FONT_L
#undef DFW_FONT_L
#endif
#define DFW_FONT_L(x) inline static dfw_gdfont_t x()
      DFW_FONT_L(getTiny_l)   { return getFont_l(TINY);  }
      DFW_FONT_L(getSmall_l)  { return getFont_l(SMALL); }
      DFW_FONT_L(getBold_l)   { return getFont_l(BOLD); }
      DFW_FONT_L(getLarge_l)  { return getFont_l(LARGE); }
      DFW_FONT_L(getGiant_l)  { return getFont_l(GIANT); }
#undef DFW_FONT_L

  private:
      type_t       m_type;
      dfw_gdfont_t m_font;

  public:
      GDFont(type_t type);
      virtual ~GDFont();

      inline type_t       getType() { return m_type; }
      inline dfw_gdfont_t getFont() { return m_font; } // gdFontPtr;

  };

};

#endif /* DFRAMEWORK_GD_GDFONT_H */

