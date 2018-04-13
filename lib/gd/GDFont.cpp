#include <dframework/gd/GDFont.h>
#include "gd.h"
#include "gdfontg.h"
#include "gdfontl.h"
#include "gdfontt.h"
#include "gdfonts.h"
#include "gdfontmb.h"

namespace dframework {

  dfw_gdfont_t GDFont::getFont_l(type_t type){
      switch(type){
      case TINY :
          return (dfw_gdfont_t)gdFontGetSmall();
      case BOLD :
          return (dfw_gdfont_t)gdFontGetMediumBold();
      case LARGE :
          return (dfw_gdfont_t)gdFontGetLarge();
      case GIANT :
          return (dfw_gdfont_t)gdFontGetGiant();
      case SMALL :
      default :
          return (dfw_gdfont_t)gdFontGetSmall();
      }
  }

  GDFont::GDFont(type_t type)
          : Object()
  {
      m_type = type;
      switch(type){
      case TINY :
          m_font = (dfw_gdfont_t)gdFontGetSmall();
          break;

      case BOLD :
          m_font = (dfw_gdfont_t)gdFontGetMediumBold();
          break;

      case LARGE :
          m_font = (dfw_gdfont_t)gdFontGetLarge();
          break;

      case GIANT :
          m_font = (dfw_gdfont_t)gdFontGetGiant();
          break;

      case SMALL :
      default :
          m_type = SMALL;
          m_font = (dfw_gdfont_t)gdFontGetSmall();
          break;
      }
  }

  GDFont::~GDFont(){
  }

  int GDFont::useFTConfig(bool flag){
      return gdFTUseFontConfig(flag);
  }

  int GDFont::cacheSetup(){
      return -1;
      //return gdFontCacheSetup();
  }

  void GDFont::cacheShutdown(){
      //gdFontCacheShutdown();
  }

};
