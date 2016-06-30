#include <dframework/monitor/GdMonMemory.h>
#include <dframework/gd/GDImage.h>
#include <dframework/util/Time.h>
#include <dframework/util/Util.h>
#include <math.h>

namespace dframework {

  GdMonMemory::GdMonMemory(uint64_t sec)
          : MonMemory(sec)
  {
  }

  GdMonMemory::~GdMonMemory(){
  }

  void GdMonMemory::getMax(struct max_st* max, sp<info>& info, int limit){
      int mk, mc = 0;
      uint64_t swUsed;
      max->a = max->b = 0;

      for(mk=info->m_aLists.size(); (mk>0 && mc < limit); mk--, mc++){
        sp<GdMonMemory> e = info->m_aLists.get(mk-1);

        if( max->a < e->m_total )
            max->a = e->m_total;

        swUsed = e->m_swapTotal - e->m_swapFree;
        if( max->b < (e->m_total+swUsed) )
            max->b = (e->m_total+swUsed);
      }
  }

  void GdMonMemory::setData(struct data_st* dt)
  {
    ::memset(dt, 0, sizeof(struct data_st));

    int alpha    = 127/2;
    // http://www.hipenpal.com/tool/html_color_charts_rgb_color_table_in_korean.php
    // http://www.n2n.pe.kr/lev-1/color.htm
    int gray     = m_img->alpha(0xAA, 0xAA, 0xAA, alpha);
    int red      = m_img->color(0xFF, 0x00, 0x00);
    int green    = m_img->color(0x00, 0xFF, 0x00);
    int blue     = m_img->alpha(0x00, 0x00, 0xFF, alpha);
    int violet   = m_img->color(0x5A, 0x5A, 0xFF);

#define SET_GD2_U(x, c, datum)    \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 1, 0, 2)

    if( m_draw_no == 0 ){
        SET_GD2_U(free,    gray,  0);
        SET_GD2_U(buffers, blue,  0);
        SET_GD2_U(cached,  green, 0);
        SET_GD2_U(kernel,  red,   m_g.cr.ey);
    }else{
        SET_GD2_U(free,    gray,  0);
        SET_GD2_U(buffers, blue,  0);
        SET_GD2_U(cached,  green, 0);
        SET_GD2_U(kernel,  violet,0);

        SET_GD2_U(swUsed,  red,   0);
        SET_GD2_U(vmUsed,  green, m_g.cr.ey);
    }
  }

  void GdMonMemory::procData(int dx, struct max_st* max, struct data_st* dt
                         , sp<GdMonMemory>& p, sp<GdMonMemory>& pp)
  {
    DFW_UNUSED(dx);
    DFW_UNUSED(pp);

    int h = m_g.cs.h;
    uint64_t kern = p->m_total - (p->m_free + p->m_buffers + p->m_cached);

    if( m_draw_no == 0 ){
      uint64_t mxa = max->a;
      DGD_GRAPH_DATA_V( dt->kernel,                kern,         h, mxa);
      DGD_GRAPH_DATA_VD(dt->cached, dt->kernel.v,  p->m_cached,  h, mxa);
      DGD_GRAPH_DATA_VD(dt->buffers,dt->cached.v,  p->m_buffers, h, mxa);
      DGD_GRAPH_DATA_VD(dt->free,   dt->buffers.v, p->m_free,    h, mxa);
    }else{
      uint64_t mxb = max->b;
      uint64_t swUsed = p->m_swapTotal - p->m_swapFree;
      kern -= p->m_vmallocUsed;

      DGD_GRAPH_DATA_V( dt->vmUsed,                p->m_vmallocUsed, h, mxb);
      DGD_GRAPH_DATA_VD(dt->swUsed, dt->vmUsed.v,  swUsed,       h, mxb);
      DGD_GRAPH_DATA_VD(dt->kernel, dt->swUsed.v,  kern,         h, mxb);
      DGD_GRAPH_DATA_VD(dt->cached, dt->kernel.v,  p->m_cached,  h, mxb);
      DGD_GRAPH_DATA_VD(dt->buffers,dt->cached.v,  p->m_buffers, h, mxb);
      DGD_GRAPH_DATA_VD(dt->free,   dt->buffers.v, p->m_free,    h, mxb);
    }
  }

  void GdMonMemory::drawData(int dx, data_st* dt)
  {
    DFW_UNUSED(dx);

    m_img->alphaBlending(0);
    if( m_draw_no == 0 ){
      drawDatumLine(dx, &dt->kernel, 0);
      drawDatumLine(dx, &dt->cached, 0);
      drawDatumLine(dx, &dt->buffers,0);
      drawDatumLine(dx, &dt->free,   0);
    }else{
      drawDatumLine(dx, &dt->vmUsed, 0);
      drawDatumLine(dx, &dt->swUsed, 0);
      drawDatumLine(dx, &dt->kernel, 0);
      drawDatumLine(dx, &dt->cached, 0);
      drawDatumLine(dx, &dt->buffers,0);
      drawDatumLine(dx, &dt->free,   0);
    }
    m_img->alphaBlending(1);
  }

  void GdMonMemory::drawBottom(sp<MonBase>& dest 
          , struct max_st* max, struct data_st* dt)
  {
    DFW_UNUSED(dest);

    String sText;
    String sZero = "0 G";

    uint64_t msize;

    if( m_draw_no == 0 ){
      msize = max->a;
    }else{
      msize = max->b;
    }

    float msize_g = ((float)msize / (float)(1024*1024));

    sText = String::format("%.02f G", msize_g);
    m_img->small(m_g.cr.sx-3, m_g.cr.sy,    sText, m_cblack, 1);
    m_img->small(m_g.cr.sx-3, m_g.cr.ey-14, sZero, m_cblack, 1);

    GdBaseMonitor::drawIndex("free",    &dt->free);
    GdBaseMonitor::drawIndex("buffers", &dt->buffers);
    GdBaseMonitor::drawIndex("cached",  &dt->cached);
    GdBaseMonitor::drawIndex("kernel",  &dt->kernel);
    if( m_draw_no != 0 ){
      GdBaseMonitor::drawIndex("swap used", &dt->swUsed);
      GdBaseMonitor::drawIndex("vm used",   &dt->vmUsed);
    }
  }

  sp<Retval> GdMonMemory::draw(int num, sp<info>& info, sp<MonBase>& dest
                       , const char* path)
  {
    sp<Retval> retval;

    if( DFW_RET(retval, MonMemory::draw(num, info, dest, path)) )
        return DFW_RETVAL_D(retval);

    struct max_st  max;
    struct data_st dt;

    for(int draw_no=0; draw_no<2; draw_no++){ 
      m_draw_no = draw_no;

      if( DFW_RET(retval, baseInit()) ){
          return DFW_RETVAL_D(retval);
      }

      if( draw_no==0 ){ getMax(&max, info, m_g.cs.w); }
      setData(&dt);

      drawHead(dest->m_sec, info->m_sec);

      uint64_t first_sec;
      uint64_t last_sec = dest->m_sec;
      for(int k=info->m_aLists.size(), dx=m_g.cr.ex; ((k>0)&&(dx>m_g.cr.sx));)
      {
          sp<GdMonMemory> p  = info->m_aLists.get(k-1);
          sp<GdMonMemory> pp = info->m_aLists.get(k-2);
          procData(dx, &max, &dt, p, pp);
          drawData(dx, &dt);
          k--;
          dx--;
          first_sec = p->m_sec;
      }

      drawBottom(dest, &max, &dt);
      drawLast(first_sec, last_sec);
      if( DFW_RET(retval, savePng(path, savename(), 0)) )
          return DFW_RETVAL_D(retval);
    } // end for[int draw_no=0; draw_no<2; draw_no++]
    return NULL;
  }

}

