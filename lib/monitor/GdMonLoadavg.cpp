#include <dframework/monitor/GdMonLoadavg.h>
#include <dframework/gd/GDImage.h>
#include <dframework/util/Time.h>
#include <dframework/util/Util.h>
#include <math.h>

namespace dframework {

  GdMonLoadavg::GdMonLoadavg(uint64_t sec)
          : MonLoadavg(sec)
  {
  }

  GdMonLoadavg::~GdMonLoadavg(){
  }

  void GdMonLoadavg::getMax(struct max_st* max, sp<info>& info, int limit){
      int mk, mc = 0;
      max->a = 0;
      for(mk=info->m_aLists.size(); (mk>0 && mc < limit); mk--, mc++){
        sp<GdMonLoadavg> e = info->m_aLists.get(mk-1);
        if( max->a < e->m_1 )
            max->a = e->m_1;
        if( max->a < e->m_5 )
            max->a = e->m_5;
        if( max->a < e->m_15 )
            max->a = e->m_15;
      }
  }

  void GdMonLoadavg::setData(struct data_st* dt)
  {
    ::memset(dt, 0, sizeof(struct data_st));

    int alpha  = 127/2;
    int green  = m_img->alpha(0x00, 0xFF, 0x00, alpha);
    int blue   = m_img->alpha(0x00, 0x00, 0xFF, alpha);
    int red    = m_img->alpha(0xFF, 0x00, 0x00, alpha);

#define SET_GD(x, c, cl, datum) \
    Graph::SetGraphData(&dt->x, c, cl, 0,0, datum, 1, 0, 2)

#define SET_GD2_U(x, c, datum, t) \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 1, 0, t)

#define SET_GD2_D(x, c, datum, t) \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 2, 0, t)

    SET_GD2_U(m1,  red,   m_g.cr.ey, 2);
    SET_GD2_U(m5,  blue,  m_g.cr.ey, 2);
    SET_GD2_U(m15, green, m_g.cr.ey, 2);
  }

  void GdMonLoadavg::procData(int dx, struct max_st* max, struct data_st* dt
                         , sp<GdMonLoadavg>& p, sp<GdMonLoadavg>& pp)
  {
      DFW_UNUSED(dx);
      DFW_UNUSED(pp);
      bool is = pp.has();

      DGD_GRAPH_DATA_V( dt->m1, p->m_1,             m_g.cs.h, max->a);
      DGD_GRAPH_DATA_PV(dt->m1, is, (is?pp->m_1:0), m_g.cs.h, max->a);
      DGD_GRAPH_DATA_V( dt->m5, p->m_5,             m_g.cs.h, max->a);
      DGD_GRAPH_DATA_PV(dt->m5, is, (is?pp->m_5:0), m_g.cs.h, max->a);
      DGD_GRAPH_DATA_V( dt->m15, p->m_15,             m_g.cs.h, max->a);
      DGD_GRAPH_DATA_PV(dt->m15, is, (is?pp->m_15:0), m_g.cs.h, max->a);
  }

  void GdMonLoadavg::drawData(int dx, data_st* dt)
  {
      DFW_UNUSED(dx);

      m_img->alphaBlending(1);
      drawDatumLine(dx, &dt->m15);
      drawDescLine(dx, &dt->m15);
      drawDescLine(dx, &dt->m5);
      drawDescLine(dx, &dt->m1);
      m_img->alphaBlending(0);
  }

  void GdMonLoadavg::drawBottom(sp<MonBase>& dest 
          , struct max_st* max, struct data_st* dt)
  {
      DFW_UNUSED(dest);

      String sText;
      String sZero = "0";

      sText = String::format("%.02f", ((float)max->a/100.f));
      m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
      m_img->small(m_g.cr.sx-3, m_g.cr.ey-14, sZero, m_cblack, 1);

      GdBaseMonitor::drawIndex("1 minute", &dt->m1);
      GdBaseMonitor::drawIndex("5 minute", &dt->m5);
      GdBaseMonitor::drawIndex("15 minute", &dt->m15);
  }

  sp<Retval> GdMonLoadavg::draw(int num, sp<info>& info, sp<MonBase>& dest
                        , const char* path)
  {
      sp<Retval> retval;
      if( DFW_RET(retval, MonLoadavg::draw(num, info, dest, path)) )
          return DFW_RETVAL_D(retval);
      if( DFW_RET(retval, baseInit()) )
          return DFW_RETVAL_D(retval);

      struct max_st  max;
      struct data_st dt;
      getMax(&max, info, m_g.cs.w);
      setData(&dt);

      drawHead(dest->m_sec, info->m_sec);

      uint64_t first_sec;
      uint64_t last_sec = dest->m_sec;
      for(int k=info->m_aLists.size(), dx=m_g.cr.ex; ((k>0)&&(dx>m_g.cr.sx));)
      {
          sp<GdMonLoadavg> p  = info->m_aLists.get(k-1);
          sp<GdMonLoadavg> pp = info->m_aLists.get(k-2);
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
      return NULL;
  }

}

