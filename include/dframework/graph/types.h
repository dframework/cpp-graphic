#ifndef DFRAMEWORK_GRAPHIC_TYPES_H
#define DFRAMEWORK_GRAPHIC_TYPES_H


#define DGD_GRAPH_DATA_V(d, x, p, m)                        \
    (d.v = ( m                                              \
               ? (((d.vector==1) || (d.vector==3))          \
                  ? (d.datum - ceil((x*p)/m)+d.offset)      \
                  : (d.datum + ceil((x*p)/m)+d.offset))     \
               : (d.datum+d.offset)))

#define DGD_GRAPH_DATA_VD(d, a, x, p, m)                    \
    (d.v = ( m                                              \
               ? (((d.vector==1) || (d.vector==3))          \
                  ? ((d.datum=a) - ceil((x*p)/m)+d.offset)  \
                  : ((d.datum=a) + ceil((x*p)/m)+d.offset)) \
               : ((d.datum=a)+d.offset)))

#define DGD_GRAPH_DATA_VP(d, x, p, m)                       \
    (d->v = ( m                                             \
                 ? (((d->vector==1) || (d->vector==3))      \
                    ? (d->datum - ceil((x*p)/m)+d->offset)  \
                    : (d->datum + ceil((x*p)/m)+d->offset)) \
                 : (d->datum+d->offset)))

#define DGD_GRAPH_DATA_VPD(d, a, x, p, m)                   \
    (d->v = ( m                                             \
             ? (((d->vector==1) || (d->vector==3))          \
                ? ((d->datum=a) - ceil((x*p)/m)+d->offset)  \
                : ((d->datum=a) + ceil((x*p)/m)+d->offset)) \
             : ((d->datum=a)+d->offset)))

#define DGD_GRAPH_DATA_PV(d, b, x, p, m)                    \
    (d.pv = ( (b && m)                                      \
                 ? (((d.vector==1) || (d.vector==3))        \
                    ? (d.datum - ceil((x*p)/m)+d.offset)    \
                    : (d.datum + ceil((x*p)/m)+d.offset))   \
                 : d.v ) )

#define DGD_GRAPH_DATA_PVD(d, a, b, x, p, m)                  \
    (d.pv = ( (b && m)                                        \
                 ? (((d.vector==1) || (d.vector==3))          \
                    ? ((d.datum=a) - ceil((x*p)/m)+d.offset)  \
                    : ((d.datum=a) + ceil((x*p)/m)+d.offset)) \
                 : d.v ) )

#define DGD_GRAPH_DATA_PVP(d, b, x, p, m)                   \
    (d->pv = ( (b && m)                                     \
                 ? (((d->vector==1) || (d->vector==3))      \
                    ? (d->datum - ceil((x*p)/m)+d->offset)  \
                    : (d->datum + ceil((x*p)/m)+d->offset)) \
                 : d->v ) )


namespace dframework {

  typedef struct {
      int x;
      int y;
  } dgd_point_t;

  typedef struct {
      int w;
      int h;
  } dgd_size_t;

  typedef struct {
      int sx;
      int sy;
      int ex;
      int ey;
  } dgd_rect_t;

  typedef struct {
      int x;
      int y;
      int w;
      int h;
  } dgd_area_t;

  typedef struct {
      dgd_size_t  asize;  // all space size(width x height).
      dgd_rect_t  gspace; // draw graph space.
      dgd_size_t  gsize;  // draw graph space size.
  } dgd_graph_t;

  typedef struct {
      // ----------------------------------
      // |      all space                 |
      // |      ------------------------  |
      // |      |   draw canvas space  |  |
      // |      |                      |  |
      // |      |          +           |  |  <- '+' is draw space middle point
      // |      |                      |  |
      // |      |                      |  |
      // |      ------------------------  |
      // ----------------------------------
      dgd_size_t  as;  // all space size(width x height).
      dgd_rect_t  cr;  // draw canvas space rect.
      dgd_size_t  cs;  // draw canvas space size.
      dgd_area_t  m;   // draw canvas space middle info.
  } dgd_graph2_t;

  typedef struct {
      int c;      // default color.
      int cl;     // line color.
      int v;      // cur value.
      int pv;     // prev value.
      int datum;
      int vector; // up(1), down(2), left(3), right(4) at datum.
      int offset;
      int type;   // point(1), fill(2), line(3), fill-line(4)
  } dgd_graph_data_t;

};

#endif /* DFRAMEWORK_GRAPHIC_TYPES_H */

