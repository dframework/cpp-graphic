#ifndef DFRAMEWORK_GRAPH_GRAPH_H
#define DFRAMEWORK_GRAPH_GRAPH_H

#include <dframework/base/Retval.h>
#include <dframework/graph/types.h>
#include <dframework/gd/GDImage.h>

namespace dframework {

  class Graph : public Object
  {
  public:
      static void SetGraphData(dgd_graph_data_t* d, int color);
      static void SetGraphData(dgd_graph_data_t* d
                            , int color, int color_line);
      static void SetGraphData(dgd_graph_data_t* d
                            , int color, int color_line
                            , int value, int prev_value);
      static void SetGraphData(dgd_graph_data_t* d
                            , int c, int cl, int v, int pv
                            , int datum, int vector, int off, int type=2);

      static sp<Retval> SetGraphData(dgd_graph_data_t** d, int color);
      static sp<Retval> SetGraphData(dgd_graph_data_t** d
                            , int color, int color_line);
      static sp<Retval> SetGraphData(dgd_graph_data_t** d
                            , int color, int color_line
                            , int value, int prev_value);
      static sp<Retval> SetGraphData(dgd_graph_data_t** out
                            , int c, int cl, int v, int pv
                            , int datum, int vector, int off);
      static sp<Retval> SetGraphData(dgd_graph_data_t** out
                            , int c, int cl, int v, int pv
                            , int datum, int vector, int off, int type=2);

      static void SetGraphInfo(dgd_graph2_t* g
                            , int w, int h, int sx, int sy, int ex, int ey);

      inline static void drawDatumLine(int dx
                            , sp<GDImage>& im, dgd_graph_data_t* data
                            , int datum, int off){
          im->line(dx, data->v, dx, datum+off, data->c);
      }
      inline static void drawDatumLine(int dx
                            , sp<GDImage>& im, dgd_graph_data_t* data
                            , int off=0){
          im->line(dx, data->v, dx, data->datum+off, data->c);
      }

      inline static void drawDescLine(int dx
                            , sp<GDImage>& im, dgd_graph_data_t* data){
          im->line(dx-1, data->pv, dx, data->v, data->cl);
      }

      inline static void drawAscLine(int dx
                            , sp<GDImage>& im, dgd_graph_data_t* data){
          im->line(dx, data->v, dx+1, data->pv, data->cl);
      }

  public:
      Graph();
      virtual ~Graph();

  };

};

#endif /*  DFRAMEWORK_GRAPH_GRAPH_H */

