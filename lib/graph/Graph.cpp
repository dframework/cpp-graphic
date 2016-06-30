#include <dframework/graph/Graph.h>

namespace dframework {

  void Graph::SetGraphData(dgd_graph_data_t* d, int c){
      d->c  = c;
      d->cl = c;
      d->v  = 0;
      d->pv = 0;
      d->datum = 0;
      d->vector = 0;
      d->offset = 0;
      d->type = 2; // default : fill
  }

  void Graph::SetGraphData(dgd_graph_data_t* d, int c, int cl){
      d->c  = c;
      d->cl = cl;
      d->v  = 0;
      d->pv = 0;
      d->datum = 0;
      d->vector = 0;
      d->offset = 0;
      d->type = 2; // default : fill
  }

  void Graph::SetGraphData(dgd_graph_data_t* d, int c, int cl, int v, int pv){
      d->c  = c;
      d->cl = cl;
      d->v  = v;
      d->pv = pv;
      d->datum = 0;
      d->vector = 0;
      d->offset = 0;
      d->type = 2; // default : fill
  }

  void Graph::SetGraphData(dgd_graph_data_t* d, int c, int cl, int v, int pv
                                              , int datum, int vector, int off
                                              , int type)
  {
      d->c  = c;
      d->cl = cl;
      d->v  = v;
      d->pv = pv;
      d->datum = datum;
      d->vector = vector;
      d->offset = off;
      d->type = type;
  }

  sp<Retval> Graph::SetGraphData(dgd_graph_data_t** out, int c){
      if( !(*out = (dgd_graph_data_t*)::malloc( sizeof(dgd_graph_data_t) )) )
          return DFW_RETVAL_NEW(DFW_E_NOMEM, ENOMEM);
      SetGraphData(*out, c);
      return NULL;
  }

  sp<Retval> Graph::SetGraphData(dgd_graph_data_t** out, int c, int cl){
      if( !(*out = (dgd_graph_data_t*)::malloc( sizeof(dgd_graph_data_t) )) )
          return DFW_RETVAL_NEW(DFW_E_NOMEM, ENOMEM);
      SetGraphData(*out, c, cl);
      return NULL;
  }

  sp<Retval> Graph::SetGraphData(dgd_graph_data_t** out
                               , int c, int cl, int v, int pv){
      if( !(*out = (dgd_graph_data_t*)::malloc( sizeof(dgd_graph_data_t) )) )
          return DFW_RETVAL_NEW(DFW_E_NOMEM, ENOMEM);
      SetGraphData(*out, c, cl, v, pv);
      return NULL;
  }

  sp<Retval> Graph::SetGraphData(dgd_graph_data_t** out
                               , int c, int cl, int v, int pv
                               , int datum, int vector, int off
                               , int type)
  {
      if( !(*out = (dgd_graph_data_t*)::malloc( sizeof(dgd_graph_data_t) )) )
          return DFW_RETVAL_NEW(DFW_E_NOMEM, ENOMEM);
      SetGraphData(*out, c, cl, v, pv, datum, vector, off, type);
      return NULL;
  }

  void Graph::SetGraphInfo(dgd_graph2_t* g
                         , int w, int h, int sx, int sy, int ex, int ey)
  {
      g->as.w  = w;
      g->as.h  = h;
      g->cr.sx = sx;
      g->cr.sy = sy;
      g->cr.ex = ex;
      g->cr.ey = ey;
      g->cs.w  = ex - sx;
      g->cs.h  = ey - sy;
      g->m.x   = ((ex - sx) / 2) + sx;
      g->m.y   = ((ey - sy) / 2) + sy;
      g->m.w   = g->cs.w / 2;
      g->m.h   = g->cs.h / 2;
  }

#if 0
  void Graph::DrawDatumFillLine(int dx, sp<GDImage>& im, dgd_graph_data_t* data
                     , int off)
  {
      im->line(dx, data->v, dx, data->datum+off, data->c);
  }
#endif

  Graph::Graph(){
  }

  Graph::~Graph(){
  }

};

