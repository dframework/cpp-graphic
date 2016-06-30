#include <dframework/monitor/GdMonDiskstats.h>
#include <dframework/gd/GDImage.h>
#include <dframework/util/Time.h>
#include <dframework/util/Util.h>
#include <math.h>

namespace dframework {

  GdMonDiskstats::GdMonDiskstats(uint64_t sec)
          : MonDiskstats(sec)
  {
  }

  GdMonDiskstats::~GdMonDiskstats(){
  }

  const char* GdMonDiskstats::title(){
      switch( m_draw_no ){
      case 0 :
          return "DISK STATS (COUNT)";
      case 1 :
          return "DISK STATS (MERGED)";
      case 2 :
          return "DISK STATS (SECTOR)";
      case 3 :
          return "DISK STATS (TIME)";
      case 4 :
          return "DISK STATS (IO)";
      default :
          return "DISK STATS (-)";
      }
  }

  const char* GdMonDiskstats::savename(){
      switch( m_draw_no ){
      case 0 :
          return "diskstats-count";
      case 1 :
          return "diskstats-merged";
      case 2 :
          return "diskstats-sector";
      case 3 :
          return "diskstats-time";
      case 4 :
          return "diskstats-io";
      default :
          return "diskstats-not";
      }
  }

  void GdMonDiskstats::getMax(struct max_st* max, sp<info>& info, int limit){
      int mk, mc = 0;
      max->rwc = max->rwm = max->rws = max->rwt = 0;
      max->ic = max->it = 0; //max->iw = 0;

      for(mk=info->m_aLists.size(); (mk>0 && mc < limit); mk--, mc++){
        sp<GdMonDiskstats> e = info->m_aLists.get(mk-1);
        if( e.has() && e->m_all.has() ){
            if( max->rwc < e->rcount() )
                max->rwc = e->rcount();
            if( max->rwc < e->wcount() )
                max->rwc = e->wcount();

            if( max->rwm < e->rmerged() )
                max->rwm = e->rmerged();
            if( max->rwm < e->wmerged() )
                max->rwm = e->wmerged();

            if( max->rws < e->rsector() )
                max->rws = e->rsector();
            if( max->rws < e->wsector() )
                max->rws = e->wsector();

            if( max->rwt < e->rtime() )
                max->rwt = e->rtime();
            if( max->rwt < e->wtime() )
                max->rwt = e->wtime();

            if( max->ic < e->iocount() )
                max->ic = e->iocount();

            if( max->it < e->iotime() )
                max->it = e->iotime();
            if( max->it < e->iowtime() )
                max->it = e->iowtime();
        }
      }
  }

  void GdMonDiskstats::setData(struct data_st* dt)
  {
    ::memset(dt, 0, sizeof(struct data_st));

    int alpha    = 127/2;
    int red      = m_img->color(0xFF, 0x00, 0x00);
    int green    = m_img->color(0x00, 0xFF, 0x00);
    int blue     = m_img->alpha(0x00, 0x00, 0xFF, alpha);

#define SET_GD2_U(x, c, datum, t) \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 1, 0, t)

#define SET_GD2_D(x, c, datum, t) \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 2, 0, t)

    SET_GD2_U(rcount,  green, m_g.cr.ey, 3);
    SET_GD2_U(wcount,  blue,  m_g.cr.ey, 3);

    SET_GD2_U(rmerged, green, m_g.cr.ey, 3);
    SET_GD2_U(wmerged, blue,  m_g.cr.ey, 3);

    SET_GD2_U(rsector, green, m_g.cr.ey, 3);
    SET_GD2_U(wsector, blue,  m_g.cr.ey, 3);

    SET_GD2_U(rtime,   green, m_g.cr.ey, 3);
    SET_GD2_U(wtime,   blue,  m_g.cr.ey, 3);

    SET_GD2_U(iotime,  blue,  m_g.m.y,   3);
    SET_GD2_U(iowtime, red,   m_g.m.y,   2);
    SET_GD2_U(iocount, green, m_g.cr.ey, 2);
  }

  void GdMonDiskstats::procData(int dx, struct max_st* max, struct data_st* dt
                         , sp<GdMonDiskstats>& p, sp<GdMonDiskstats>& pp)
  {
      DFW_UNUSED(dx);

      bool is = pp.has();
      int ha = m_g.cs.h;
      int hb = m_g.cs.h/2;

      switch( m_draw_no ){
      case 0 :
          DGD_GRAPH_DATA_V(dt->rcount,  p->rcount(),  ha, max->rwc);
          DGD_GRAPH_DATA_V(dt->wcount,  p->wcount(),  ha, max->rwc);
          DGD_GRAPH_DATA_PV(dt->rcount, is, (is?pp->rcount():0),  ha, max->rwc);
          DGD_GRAPH_DATA_PV(dt->wcount, is, (is?pp->wcount():0),  ha, max->rwc);
          break;
      case 1 :
          DGD_GRAPH_DATA_V(dt->rmerged, p->rmerged(), ha, max->rwm);
          DGD_GRAPH_DATA_V(dt->wmerged, p->wmerged(), ha, max->rwm);
          DGD_GRAPH_DATA_PV(dt->rmerged,is, (is?pp->rmerged():0), ha, max->rwm);
          DGD_GRAPH_DATA_PV(dt->wmerged,is, (is?pp->wmerged():0), ha, max->rwm);
          break;
      case 2 :
          DGD_GRAPH_DATA_V(dt->rsector, p->rsector(), ha, max->rws);
          DGD_GRAPH_DATA_V(dt->wsector, p->wsector(), ha, max->rws);
          DGD_GRAPH_DATA_PV(dt->rsector,is, (is?pp->rsector():0), ha, max->rws);
          DGD_GRAPH_DATA_PV(dt->wsector,is, (is?pp->wsector():0), ha, max->rws);
          break;
      case 3 :
          DGD_GRAPH_DATA_V(dt->rtime,   p->rtime(),   ha, max->rwt);
          DGD_GRAPH_DATA_V(dt->wtime,   p->wtime(),   ha, max->rwt);
          DGD_GRAPH_DATA_PV(dt->rtime,  is, (is?pp->rtime():0),   ha, max->rwt);
          DGD_GRAPH_DATA_PV(dt->wtime,  is, (is?pp->wtime():0),   ha, max->rwt);
          break;
      case 4 :
          DGD_GRAPH_DATA_V(dt->iotime,  p->iotime(),  hb, max->it);
          DGD_GRAPH_DATA_V(dt->iowtime, p->iowtime(), hb, max->it);
          DGD_GRAPH_DATA_V(dt->iocount, p->iocount(), hb, max->ic);

          DGD_GRAPH_DATA_PV(dt->iotime, is, (is?pp->iotime():0),  hb, max->it);
          DGD_GRAPH_DATA_PV(dt->iowtime,is, (is?pp->iowtime():0), hb, max->it);
          DGD_GRAPH_DATA_PV(dt->iocount,is, (is?pp->iocount():0), hb, max->ic);
          break;
      default :
          break;
      }
  }

  void GdMonDiskstats::drawData(int dx, data_st* dt)
  {
      switch( m_draw_no ){
      case 0 :
          m_img->alphaBlending(0);
          drawDescLine(dx, &dt->rcount);
          drawDescLine(dx, &dt->wcount);
          m_img->alphaBlending(1);
          break;
      case 1 :
          m_img->alphaBlending(0);
          drawDescLine(dx, &dt->rmerged);
          drawDescLine(dx, &dt->wmerged);
          m_img->alphaBlending(1);
          break;
      case 2 :
          m_img->alphaBlending(0);
          drawDescLine(dx, &dt->rsector);
          drawDescLine(dx, &dt->wsector);
          m_img->alphaBlending(1);
          break;
      case 3 :
          m_img->alphaBlending(0);
          drawDescLine(dx, &dt->rtime);
          drawDescLine(dx, &dt->wtime);
          m_img->alphaBlending(1);
          break;
      case 4 :
          drawDatumLine(dx,&dt->iowtime, 0);
          drawDescLine(dx, &dt->iotime);
          drawDatumLine(dx, &dt->iocount, 0);
          break;
      default :
          break;
    }

  }

  void GdMonDiskstats::drawBottom(sp<MonBase>& dest 
          , struct max_st* max, struct data_st* dt)
  {
      DFW_UNUSED(dest);

      String sText;
      String sZero = "0";

      switch( m_draw_no ){
      case 0 :
          sText = String::format("%lu", max->rwc);
          m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
          break;
      case 1 :
          sText = String::format("%lu", max->rwm);
          m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
          break;
      case 2 :
          sText = String::format("%lu", max->rws);
          m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
          break;
      case 3 :
          sText = String::format("%lu", max->rwt);
          m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
          break;
      case 4 :
          sText = String::format("%lu", max->it);
          m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
          m_img->small(m_g.cr.sx-3, m_g.m.y-14, sZero, m_cblack, 1);

          sText = String::format("%lu", max->ic);
          m_img->small(m_g.cr.sx-3, m_g.m.y, sText, m_cblack, 1);
          m_img->small(m_g.cr.sx-3, m_g.cr.ey-14, sZero, m_cblack, 1);
          break;
      default :
          break;
      }


      switch( m_draw_no ){
      case 0 :
          GdBaseMonitor::drawIndex("rcount",  &dt->rcount);
          GdBaseMonitor::drawIndex("wcount",  &dt->wcount);
          break;
      case 1 :
          GdBaseMonitor::drawIndex("rmerged", &dt->rmerged);
          GdBaseMonitor::drawIndex("wmerged", &dt->wmerged);
          break;
      case 2 :
          GdBaseMonitor::drawIndex("rsector", &dt->rsector);
          GdBaseMonitor::drawIndex("wsector", &dt->wsector);
          break;
      case 3 :
          GdBaseMonitor::drawIndex("rtime",   &dt->rtime);
          GdBaseMonitor::drawIndex("wtime",   &dt->wtime);
          break;
      case 4 :
          GdBaseMonitor::drawIndex("iotime",   &dt->iotime);
          GdBaseMonitor::drawIndex("iowtime",  &dt->iowtime);
          GdBaseMonitor::drawIndex("iocount",  &dt->iocount);
          break;
      default :
          break;
      }
  }

  sp<Retval> GdMonDiskstats::draw(int num, sp<info>& info, sp<MonBase>& dest
                          , const char* path)
  {
      sp<Retval> retval;

      if( DFW_RET(retval, MonDiskstats::draw(num, info, dest, path)) )
          return DFW_RETVAL_D(retval);

      struct max_st  max;
      struct data_st dt;

      for(int draw_no=0; draw_no<4; draw_no++){
          m_draw_no = draw_no;

          if( DFW_RET(retval, baseInit()) ){
              return DFW_RETVAL_D(retval);
          }

          if( draw_no==0 ) { getMax(&max, info, m_g.cs.w); }
          setData(&dt);

          drawHead(dest->m_sec, info->m_sec);

          uint64_t first_sec;
          uint64_t last_sec = dest->m_sec;
          for(int k=info->m_aLists.size(), dx=m_g.cr.ex; ((k>0)&&(dx>m_g.cr.sx));)
          {
              sp<GdMonDiskstats> p  = info->m_aLists.get(k-1);
              sp<GdMonDiskstats> pp = info->m_aLists.get(k-2);
              procData(dx, &max, &dt, p, pp);
              drawData(dx, &dt);
              k--;
              dx--;
              first_sec = p->m_sec;
          }

          drawBottom(dest, &max, &dt);
          drawLast(first_sec, last_sec);
          if( DFW_RET(retval, savePng(path, savename(), 0)) ){
              return DFW_RETVAL_D(retval);
          }
      }
      return NULL;
  }

}

