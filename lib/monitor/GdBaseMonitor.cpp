#include <dframework/monitor/GdBaseMonitor.h>
#include <dframework/util/Time.h>
#include <dframework/io/File.h>

namespace dframework {

  GdBaseMonitor::GdBaseMonitor(){
      m_index_num = 0;
  }

  GdBaseMonitor::~GdBaseMonitor(){
  }

  sp<Retval> GdBaseMonitor::baseInit(){
      sp<Retval> retval;

      int w  = 540;
      int h  = 280;
      int sx = 100;
      int sy =  20;
      int ex = w - 10;
      int ey = 180;

      ::memset(&m_g, 0, sizeof(dgd_graph2_t));
      Graph::SetGraphInfo(&m_g, w, h, sx, sy, ex, ey);

      m_mx1_4 = ((m_g.m.x   - m_g.cr.sx) / 2) + m_g.cr.sx;
      m_mx3_4 = ((m_g.cr.ex - m_g.m.x)   / 2) + m_g.m.x;
      m_my1_4 = ((m_g.m.y   - m_g.cr.sy) / 2) + m_g.cr.sy;
      m_my3_4 = ((m_g.cr.ey - m_g.m.y)   / 2) + m_g.m.y;

      m_mh1_4 = m_g.m.h / 2;
      m_mh2_4 = m_g.m.h;
      m_mh3_4 = m_mh1_4 * 3;

      // -----------------------------------------------------------
      m_img = new GDImage();
      if( DFW_RET(retval, m_img->createTrueColor(m_g.as.w, m_g.as.h)) )
          return DFW_RETVAL_D(retval);

      m_cwhite  = m_img->color(0xFF,0xFF,0xFF);
      m_cblack  = m_img->color(0x00,0x00,0x00);
      m_cbg     = m_img->color(0xD0,0xD0,0xD0);
      m_coutline= m_img->color(0x2D,0x7C,0xFF);

      return NULL;
  }

  void GdBaseMonitor::drawHead(uint64_t current_sec, uint64_t depth_sec){
      m_depth_sec = depth_sec;
      m_index_num = 0;
      String sText = Time::format("Last update: %Y-%m-%d %H:%M:%S"
                                , (dfw_time_t)current_sec);

      m_img->fillRectangle(0, 0, m_g.as.w, m_g.as.h, m_cbg);
      m_img->alphaBlending(1);
      m_img->rectangle    ( 0, 0, m_g.as.w-1, m_g.as.h-1, m_coutline);
      m_img->fillRectangle(m_g.cr.sx, m_g.cr.sy, m_g.cr.ex, m_g.cr.ey, m_cwhite);

      m_img->bold(10,           2, title(), m_cblack);
      m_img->small(m_g.as.w-10, 2, sText,   m_cblack, 1);
  }

  void GdBaseMonitor::drawDatumLine(int dx, dgd_graph_data_t* d, int offset)
  {
      Graph::drawDatumLine(dx, m_img, d, offset);
  }

  void GdBaseMonitor::drawDescLine(int dx, dgd_graph_data_t* d)
  {
      Graph::drawDescLine(dx, m_img, d);
  }

  void GdBaseMonitor::drawVertical(uint64_t last_sec
                                   , int unit_d, int unit_a, const char* fmt)
  {
      String sTime;
      uint64_t ss = last_sec - (m_depth_sec * m_g.cs.w); // start seconds
      int      dp = unit_a - (ss%unit_a);                // depth seconds
      int      dx = m_g.cr.sx + (dp/unit_d);             // current x-pixel
      uint64_t dt = ss + dp;                             // current seconds

      if( dx > m_g.cr.sx ){
          sTime = Time::format(fmt,  dt);
          m_img->dashedLine(dx, m_g.cr.sy, dx, m_g.cr.ey, m_cbg);
          m_img->small     (dx, m_g.cr.ey, sTime, m_cblack, 2); 
      }

      do{
          dx += unit_a / unit_d;
          dt += unit_a;
          if( dx >= m_g.cr.ex )
              return;

          sTime = Time::format(fmt,  dt);
          m_img->dashedLine(dx, m_g.cr.sy, dx, m_g.cr.ey, m_cbg);
          m_img->small     (dx, m_g.cr.ey, sTime, m_cblack, 2); 
      }while(true);
  }

  void GdBaseMonitor::drawVertical2(uint64_t last_sec
                                   , int unit_d, int unit_a, const char* fmt)
  {
      DFW_UNUSED(last_sec);
      DFW_UNUSED(unit_d);
      DFW_UNUSED(unit_a);
      DFW_UNUSED(fmt);
  }

  void GdBaseMonitor::drawLast(uint64_t first_sec_i, uint64_t last_sec)
  {
      DFW_UNUSED(first_sec_i);
      uint64_t len_sec = m_depth_sec * m_g.cs.w;
      if(             !(3600*2 < len_sec) ){         // 1~2 hour
        drawVertical(last_sec,    5,  300, "%M");    // 5 minute (5s)
      }else if(  !((3600*24*2) < len_sec) ){         // 1~2 day
        drawVertical(last_sec,  240, 7200, "%H");    // 2 hour (4m)
      }else if( !((3600*24*14) < len_sec) ){         // 1~2 week
        drawVertical(last_sec, 1800, 3600*24, "%d"); // 1 day (30m): 24*2 = 48
      }else if( !((3600*24*62) < len_sec) ){         // 1~2 month
        drawVertical(last_sec, 7200, 3600*72, "%d"); // 3 day (2h) : 72/2 = 36
      }else{
      }

      m_img->dashedLine(m_g.cr.sx, m_g.m.y,   m_g.cr.ex, m_g.m.y,   m_cbg);
      m_img->dashedLine(m_g.cr.sx, m_my1_4,   m_g.cr.ex, m_my1_4,   m_cbg);
      m_img->dashedLine(m_g.cr.sx, m_my3_4,   m_g.cr.ex, m_my3_4,   m_cbg);
  }

  void GdBaseMonitor::drawIndex(const char* name, dgd_graph_data_t* d){
      int s_grp = 4;
      int g, x, y;

      if(m_index_num<s_grp){
          x = 20;
      }else{
          x = (m_g.as.w / 2) + 20;
      }

      g  = m_index_num % 4;
      y = m_g.cr.ey + 30;

      m_img->fillRectangle(x, y+(g*15), x+10, y+10+(g*15), d->c);
      m_img->small(x+20, y+(g*15), name, m_cblack);
      m_index_num++;
  }

  sp<Retval> GdBaseMonitor::savePng(const char* path, const char* name, unsigned v){
      DFW_UNUSED(v);
      sp<Retval> retval;

      String sSubdir = "/png";
      String sPath = String::format("%s%s", path, sSubdir.toChars());
      if( !File::isDirectory(sPath) ){
          if( DFW_RET(retval, File::makeDir(path, sSubdir, 0755)) )
              return DFW_RETVAL_D(retval);
      }

      String sName = String::format("%s/%s-%lu.png"
                         , sPath.toChars(), name, m_depth_sec);
      String sTemp = String::format("%s/%s-%lu.png.tmp"
                         , sPath.toChars(), name, m_depth_sec);

      if( DFW_RET(retval, m_img->savePng(sTemp)) )
          return DFW_RETVAL_D(retval);
      if( File::isFile(sName) ){
          if( DFW_RET(retval, File::remove(sName)) )
              return DFW_RETVAL_D(retval);
      }
      if( DFW_RET(retval, File::rename(sTemp, sName)) )
          return DFW_RETVAL_D(retval);
      return NULL;
  }

};

