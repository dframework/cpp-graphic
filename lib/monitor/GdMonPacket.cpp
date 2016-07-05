#include <dframework/monitor/GdMonPacket.h>
#include <dframework/gd/GDImage.h>
#include <dframework/util/Time.h>
#include <dframework/util/Util.h>
#include <math.h>

namespace dframework {

  GdMonPacket::GdMonPacket(uint64_t sec)
          : MonPacket(sec)
  {
  }

  GdMonPacket::~GdMonPacket(){
  }

  void GdMonPacket::getMax(struct max_st* max, sp<info>& info, int limit){
      int mk, mc = 0;
      max->b = max->p = 0;

      for(mk=info->m_aLists.size(); (mk>0 && mc < limit); mk--, mc++){
        sp<GdMonPacket> e = info->m_aLists.get(mk-1);

        if( max->b < e->m_rbytes )
            max->b = e->m_rbytes;
        if( max->b < e->m_tbytes )
            max->b = e->m_tbytes;

        if( max->p < e->m_rpackets )
            max->p = e->m_rpackets;
        if( max->p < e->m_tpackets )
            max->p = e->m_tpackets;
      }
  }

  void GdMonPacket::setData(struct data_st* dt)
  {
    ::memset(dt, 0, sizeof(struct data_st));

    int alpha    = 127/2;
    // http://www.hipenpal.com/tool/html_color_charts_rgb_color_table_in_korean.php
    // http://www.n2n.pe.kr/lev-1/color.htm
    int green    = m_img->alpha(0x00, 0xFF, 0x00, alpha);
    int blue     = m_img->alpha(0x00, 0x00, 0xFF, alpha);

#define SET_GD(x, c, cl, datum) \
    Graph::SetGraphData(&dt->x, c, cl, 0,0, datum, 1, 0, 2)

#define SET_GD2_U(x, c, datum, t) \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 1, 0, t)

#define SET_GD2_D(x, c, datum, t) \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 2, 0, t)

    SET_GD2_U(rbytes,   green, m_g.cr.ey, 2);
    SET_GD2_U(tbytes,   blue,  m_g.cr.ey, 3);

    //SET_GD2_D(rpackets, green, m_g.m.y, 3);
    //SET_GD2_D(tpackets, blue,  m_g.m.y, 3);
  }

  void GdMonPacket::procData(int dx, struct max_st* max, struct data_st* dt
                         , sp<GdMonPacket>& p, sp<GdMonPacket>& pp)
  {
      DFW_UNUSED(dx);
      bool is = pp.has();

      DGD_GRAPH_DATA_V( dt->rbytes,   p->m_rbytes,           m_g.cs.h, max->b);
      DGD_GRAPH_DATA_PV(dt->rbytes, is, (is?pp->m_rbytes:0), m_g.cs.h, max->b);
      DGD_GRAPH_DATA_V( dt->tbytes,   p->m_tbytes,           m_g.cs.h, max->b);
      DGD_GRAPH_DATA_PV(dt->tbytes, is, (is?pp->m_tbytes:0), m_g.cs.h, max->b);

      //DGD_GRAPH_DATA_V( dt->rpackets, p->m_rpackets, m_g.m.h, max->p);
      //DGD_GRAPH_DATA_PV(dt->rpackets, is, (is?pp->m_rpackets:0), m_g.m.h, max->p);
      //DGD_GRAPH_DATA_V( dt->tpackets, p->m_tpackets, m_g.m.h, max->p);
      //DGD_GRAPH_DATA_PV(dt->tpackets, is, (is?pp->m_tpackets:0), m_g.m.h, max->p);
  }

  void GdMonPacket::drawData(int dx, data_st* dt)
  {
      DFW_UNUSED(dx);

      m_img->alphaBlending(1);
      drawDatumLine(dx, &dt->rbytes);
      drawDescLine(dx, &dt->tbytes);
      m_img->alphaBlending(0);

      //drawDescLine(dx, &dt->rpackets);
      //drawDescLine(dx, &dt->tpackets);

      //drawDatumLine(dx, &dt->rbytes,   0);
      //drawDatumLine(dx, &dt->tbytes,   0);

      //drawDatumLine(dx, &dt->rpackets, 0);
      //drawDatumLine(dx, &dt->tpackets, 0);
  }

  void GdMonPacket::drawBottom(sp<MonBase>& dest 
          , struct max_st* max, struct data_st* dt)
  {
      DFW_UNUSED(dest);

      String sText;
      String sZero = "0";

      sText = Util::SizeToString(max->b); //String::format("%lu", max->b);
      m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
      m_img->small(m_g.cr.sx-3, m_g.cr.ey-14, sZero, m_cblack, 1);

      // ------------------------------------------------------------
      GdBaseMonitor::drawIndex("rbytes",   &dt->rbytes);
      GdBaseMonitor::drawIndex("tbytes",   &dt->tbytes);
      //GdBaseMonitor::drawIndex("rpackets", &dt->rpackets);
      //GdBaseMonitor::drawIndex("tpackets", &dt->tpackets);
  }

  sp<Retval> GdMonPacket::draw(int num, sp<info>& info, sp<MonBase>& dest
                       , const char* path)
  {
      sp<Retval> retval;
      if( DFW_RET(retval, MonPacket::draw(num, info, dest, path)) )
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
          sp<GdMonPacket> p  = info->m_aLists.get(k-1);
          sp<GdMonPacket> pp = info->m_aLists.get(k-2);
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

