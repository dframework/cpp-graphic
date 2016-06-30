#if 0
#include <dframework/monitor/GdMonDiskspace.h>
#include <dframework/gd/GDImage.h>
#include <dframework/util/Time.h>
#include <dframework/util/Util.h>
#include <math.h>

namespace dframework {

  GdMonDiskspace::GdMonDiskspace(uint64_t sec)
          : MonDiskspace(sec)
  {
  }

  GdMonDiskspace::~GdMonDiskspace(){
  }

  void GdMonDiskspace::getMax(struct max_st* max, sp<info>& info, int limit){
      int mk, mc = 0;
      max->t = max->i = max->w = max->p = 0;

      for(mk=info->m_aLists.size(); (mk>0 && mc < limit); mk--, mc++){
        sp<GdMonDiskspace> e = info->m_aLists.get(mk-1);
        if( e.has() && e->m_all.has() ){
            if( max->t < e->m_all->m_total )
                max->t = e->m_all->m_total;

            if( max->i < e->m_all->m_idle )
                max->i = e->m_all->m_idle;

            uint64_t mtwi = getMaxWithoutIdle(e);
            if( max->w < mtwi )
                max->w = mtwi;

            uint64_t mprc = e->m_processes + e->m_running + e->m_blocked;
            if( max->p < mprc )
                max->p = mprc;
        }
      }
  }

  void GdMonDiskspace::setData(struct data_st* dt)
  {
    ::memset(dt, 0, sizeof(struct data_st));

    int alpha    = 127/2;
    // http://www.hipenpal.com/tool/html_color_charts_rgb_color_table_in_korean.php
    // http://www.n2n.pe.kr/lev-1/color.htm
    int gray     = m_img->alpha(0xAA, 0xAA, 0xAA, alpha);
    int gray_l   = m_img->alpha(0x82, 0x82, 0x82, alpha);
    int red      = m_img->color(0xFF, 0x00, 0x00);
    int red_l    = m_img->color(0x80, 0x00, 0x00);
    int green    = m_img->color(0x00, 0xFF, 0x00);
    int green_l  = m_img->color(0x00, 0x64, 0x00);
    int blue     = m_img->alpha(0x00, 0x00, 0xFF, alpha);
    int blue_l   = m_img->alpha(0x00, 0x00, 0x69, alpha);
    int blue_2   = m_img->alpha(0x52, 0xE4, 0xDC, alpha);
    int violet   = m_img->color(0x5A, 0x5A, 0xFF);
    int violet_l = m_img->alpha(0x99, 0x32, 0xCC);
    int violet2  = m_img->color(0xFF, 0x14, 0x93);
    int yellow   = m_img->color(0xFF, 0xFF, 0x00);
    int black    = m_img->color(0x00, 0x00, 0x00);
    int test     = m_img->color(0xFF, 0x56, 0x75);

#define SET_GD(x, c, cl, datum) \
    Graph::SetGraphData(&dt->x, c, cl, 0,0, datum, 1, -1, 2)
#define SET_GD2(x, c, datum)    \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 1, -1, 2)

    SET_GD (idle,  gray,   gray_l, m_g.cr.ey);
    SET_GD (total, blue,   blue_l, 0);
    SET_GD2(iowa,  red,    m_my3_4);
    SET_GD2(irq,   black,  0);
    SET_GD2(soft,  yellow, 0);
    SET_GD2(nice,  test,   0);
    SET_GD2(syst,  violet, 0);
    SET_GD2(user,  green,  0);
  }

  void GdMonDiskspace::procData(int dx, struct max_st* max, struct data_st* dt
                         , sp<GdMonDiskspace>& p, sp<GdMonDiskspace>& pp)
  {
    bool is = pp.has();
    int  h1_4 = m_mh1_4 - 2;
    int  h3_4 = m_mh3_4 - 6;
    int  left  = p->m_all->m_total - p->m_all->m_idle;
    int  pleft = (is ? (pp->m_all->m_total - pp->m_all->m_idle) : left);

    DGD_GRAPH_DATA_V( dt->idle,             p->m_all->m_idle,   h1_4, max->t);
    DGD_GRAPH_DATA_PV(dt->idle, is, (is ?  pp->m_all->m_idle:0),h1_4, max->t);
    DGD_GRAPH_DATA_VD(dt->total,dt->idle.v, left,               h1_4, max->t);

    DGD_GRAPH_DATA_V( dt->iowa,             p->m_all->m_iowait, h3_4, max->w);
    DGD_GRAPH_DATA_VD(dt->irq,  dt->iowa.v, p->m_all->m_irq,    h3_4, max->w);
    DGD_GRAPH_DATA_VD(dt->soft, dt->irq.v,  p->m_all->m_softirq,h3_4, max->w);
    DGD_GRAPH_DATA_VD(dt->nice, dt->soft.v, p->m_all->m_nice,   h3_4, max->w);
    DGD_GRAPH_DATA_VD(dt->syst, dt->nice.v, p->m_all->m_system, h3_4, max->w);
    DGD_GRAPH_DATA_VD(dt->user, dt->syst.v, p->m_all->m_user,   h3_4, max->w);
  }

  void GdMonDiskspace::drawData(int dx, data_st* dt)
  {
    m_img->alphaBlending(0);
    drawDatumLine(dx, &dt->idle);
    drawDescLine( dx, &dt->idle);
    drawDatumLine(dx, &dt->total, -1);
    m_img->alphaBlending(1);

    m_img->alphaBlending(0);
    drawDatumLine(dx, &dt->iowa, -1);
    drawDatumLine(dx, &dt->irq,  -1);
    drawDatumLine(dx, &dt->soft, -1);
    drawDatumLine(dx, &dt->nice, -1);
    drawDatumLine(dx, &dt->syst, -1);
    drawDatumLine(dx, &dt->user, -1);
    m_img->alphaBlending(1);
  }

  void GdMonDiskspace::draw(int num, sp<info>& info, sp<MonBase>& dest
                          , const char* path)
  {
      sp<Retval> retval;
      MonDiskspace::draw(num, info, dest, path);
      if( DFW_RET(retval, baseInit()) ){
          // FIXME: log ?
          return;
      }

      struct max_st  max;
      struct data_st dt;
      getMax(&max, info, m_g.cs.w);
      setData(&dt);

      drawHead(dest->m_sec, info->m_sec);

      uint64_t first_sec;
      uint64_t last_sec = dest->m_sec;
      for(int k=info->m_aLists.size(), dx=m_g.cr.ex; ((k>0)&&(dx>m_g.cr.sx));)
      {
          sp<GdMonDiskspace> p  = info->m_aLists.get(k-1);
          sp<GdMonDiskspace> pp = info->m_aLists.get(k-2);
          procData(dx, &max, &dt, p, pp);
          drawData(dx, &dt);
          k--;
          dx--;
          first_sec = p->m_sec;
      }

      drawBottom(dest, &max, &dt);
      drawLast(first_sec, last_sec);
      savePng(path, savename(), 0);
  }

  void GdMonDiskspace::drawBottom(sp<MonBase>& dest 
          , struct max_st* max, struct data_st* dt)
  {
      String sText;
      String sZero = "0";

      // max w
      sText = String::format("%lu", max->w);
      m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);

      // max t
      sText = String::format("%lu", max->t);
      m_img->small(m_g.cr.sx-3, m_my3_4, sText, m_cblack, 1);

      // zero
      m_img->small(m_g.cr.sx-3, m_g.cr.ey-m_mh1_4-14, sZero, m_cblack, 1);
      m_img->small(m_g.cr.sx-3, m_g.cr.ey-14,         sZero, m_cblack, 1);

      // ------------------------------------------------------------
      GdBaseMonitor::drawIndex("user",    &dt->user);
      GdBaseMonitor::drawIndex("system",  &dt->syst);
      GdBaseMonitor::drawIndex("nice",    &dt->nice);
      GdBaseMonitor::drawIndex("softirq", &dt->soft);
      GdBaseMonitor::drawIndex("irq",     &dt->irq);
      GdBaseMonitor::drawIndex("iowait",  &dt->iowa);
      GdBaseMonitor::drawIndex("total",   &dt->total);
      GdBaseMonitor::drawIndex("idle",    &dt->idle);
  }

}
#endif

