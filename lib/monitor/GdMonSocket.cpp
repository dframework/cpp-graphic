#include <dframework/monitor/GdMonSocket.h>
#include <dframework/gd/GDImage.h>
#include <dframework/util/Time.h>
#include <dframework/util/Util.h>
#include <math.h>

namespace dframework {

  GdMonSocket::GdMonSocket(uint64_t sec)
          : MonSocket(sec)
  {
  }

  GdMonSocket::~GdMonSocket(){
  }

  const char* GdMonSocket::title(){
      switch( m_draw_no ){
      case 0 :
          return "SOCKET (ALL)";
      case 1 :
      case 2 :
      case 3 :
      case 4 :
      default :
          return "SOCKET (-)";
      }
  }

  const char* GdMonSocket::savename(){
      switch( m_draw_no ){
      case 0 :
          return "socket-all";
      case 1 :
      case 2 :
      case 3 :
      case 4 :
      default :
          return "socket-not";
      }
  }

  void GdMonSocket::getMax(struct max_st* max, sp<info>& info, int limit){
      int mk, mc = 0;
      max->a = max->b = 0;

      for(mk=info->m_aLists.size(); (mk>0 && mc < limit); mk--, mc++){
        sp<GdMonSocket> e = info->m_aLists.get(mk-1);
        if( e.has() && e->m_all.has() ){
            if( max->a < e->m_all->m_total )
                max->a = e->m_all->m_total;

            if( max->b < (e->m_all->m_total - e->m_all->m_est) )
                max->b = (e->m_all->m_total - e->m_all->m_est);
        }
      }
  }

  void GdMonSocket::setData(struct data_st* dt)
  {
    ::memset(dt, 0, sizeof(struct data_st));

    int alpha    = 127/2;
    int gray     = m_img->alpha(0xAA, 0xAA, 0xAA, alpha);
    int red      = m_img->color(0xFF, 0x00, 0x00);
    int green    = m_img->color(0x00, 0xFF, 0x00);
    int blue     = m_img->alpha(0x00, 0x00, 0xFF, alpha);

#define SET_GD2_U(x, c, datum, t) \
    Graph::SetGraphData(&dt->x, c, c,  0,0, datum, 1, 0, t)

    SET_GD2_U(total,  gray,  0, 2);
    SET_GD2_U(est,    green, m_g.cr.ey, 2);

    SET_GD2_U(fin,    gray,  0, 2);
    SET_GD2_U(wait,   red,   0, 2);
    SET_GD2_U(close,  blue,  0, 2);
    SET_GD2_U(others, green, m_my3_4, 2);
  }

  void GdMonSocket::procData(int dx, struct max_st* max, struct data_st* dt
                         , sp<GdMonSocket>& p, sp<GdMonSocket>& pp)
  {
      DFW_UNUSED(dx);
      DFW_UNUSED(pp);

      int ha = m_mh1_4;
      int hb = m_mh3_4;
      int left  = p->m_all->m_total - p->m_all->m_est;

      uint64_t fin = p->m_all->m_fin_wait1 + p->m_all->m_fin_wait2;
      uint64_t wait = p->m_all->m_time_wait;
      uint64_t close = p->m_all->m_close_wait;

      uint64_t others = p->m_all->m_listen 
                      + p->m_all->m_last_ack
                      + p->m_all->m_close + p->m_all->m_closing
                      + p->m_all->m_syn_sent + p->m_all->m_syn_recv;

      switch( m_draw_no ){
      case 0 :
          DGD_GRAPH_DATA_V( dt->est,   p->m_all->m_est, ha, max->a);
          DGD_GRAPH_DATA_VD(dt->total, dt->est.v, left, ha, max->a);

          DGD_GRAPH_DATA_V( dt->others,              others, hb, max->b);
          DGD_GRAPH_DATA_VD(dt->close, dt->others.v, close, hb, max->b);
          DGD_GRAPH_DATA_VD(dt->wait,  dt->close.v,  wait, hb, max->b);
          DGD_GRAPH_DATA_VD(dt->fin,   dt->wait.v,   fin, hb, max->b);
          break;

      case 1 :
          break;
      case 2 :
          break;
      case 3 :
          break;
      case 4 :
          break;
      default :
          break;
      }
  }

  void GdMonSocket::drawData(int dx, data_st* dt)
  {
      switch( m_draw_no ){
      case 0 :
          m_img->alphaBlending(0);
          drawDatumLine(dx, &dt->est, -1);
          drawDatumLine(dx, &dt->total, -1);

          drawDatumLine(dx, &dt->others, -1);
          drawDatumLine(dx, &dt->close, -1);
          drawDatumLine(dx, &dt->wait, -1);
          drawDatumLine(dx, &dt->fin, -1);
          m_img->alphaBlending(1);
          break;
      case 1 :
          break;
      case 2 :
          break;
      case 3 :
          break;
      case 4 :
          break;
      default :
          break;
    }

  }

  void GdMonSocket::drawBottom(sp<MonBase>& dest 
          , struct max_st* max, struct data_st* dt)
  {
      DFW_UNUSED(dest);
      DFW_UNUSED(max);
      DFW_UNUSED(dt);

      String sText;
      String sZero = "0";

      switch( m_draw_no ){
      case 0 :
          sText = String::format("%lu", max->b);
          m_img->small(m_g.cr.sx-3, 20, sText, m_cblack, 1);
    
          sText = String::format("%lu", max->a);
          m_img->small(m_g.cr.sx-3, m_my3_4, sText, m_cblack, 1);

          m_img->small(m_g.cr.sx-3, m_g.cr.ey-m_mh1_4-14, sZero, m_cblack, 1);
          m_img->small(m_g.cr.sx-3, m_g.cr.ey-14,         sZero, m_cblack, 1);

          GdBaseMonitor::drawIndex("fin",    &dt->fin);
          GdBaseMonitor::drawIndex("wait",   &dt->wait);
          GdBaseMonitor::drawIndex("close",  &dt->close);
          GdBaseMonitor::drawIndex("others", &dt->others);
          GdBaseMonitor::drawIndex("total",  &dt->total);
          GdBaseMonitor::drawIndex("est",    &dt->est);
          break;
      case 1 :
          break;
      case 2 :
          break;
      case 3 :
          break;
      case 4 :
          break;
      default :
          break;
      }
  }

  sp<Retval> GdMonSocket::draw(int num, sp<info>& info, sp<MonBase>& dest
                       , const char* path)
  {
      sp<Retval> retval;

      if( DFW_RET(retval, MonSocket::draw(num, info, dest, path)) )
          return DFW_RETVAL_D(retval);

      struct max_st  max;
      struct data_st dt;

      for(int draw_no=0; draw_no<1; draw_no++){
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
              sp<GdMonSocket> p  = info->m_aLists.get(k-1);
              sp<GdMonSocket> pp = info->m_aLists.get(k-2);
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
      }
      return NULL;
  }

}

