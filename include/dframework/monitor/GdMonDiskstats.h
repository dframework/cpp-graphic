#ifndef DFRAMEWORK_MONITOR_GDMONDISKSTATS_H
#define DFRAMEWORK_MONITOR_GDMONDISKSTATS_H

#include <dframework/monitor/GdBaseMonitor.h>
#include <dframework/monitor/MonDiskstats.h>

namespace dframework {

  class GdMonDiskstats : public MonDiskstats
                  , public GdBaseMonitor
  {
  public:
      struct max_st {
          uint64_t rwc;
          uint64_t rwm;
          uint64_t rws;
          uint64_t rwt;

          uint64_t ic;
          uint64_t it;
//          uint64_t iw;
      };

      struct data_st{
          dgd_graph_data_t rcount;
          dgd_graph_data_t rmerged;
          dgd_graph_data_t rsector;
          dgd_graph_data_t rtime;

          dgd_graph_data_t wcount;
          dgd_graph_data_t wmerged;
          dgd_graph_data_t wsector;
          dgd_graph_data_t wtime;

          dgd_graph_data_t iotime;
          dgd_graph_data_t iowtime;
          dgd_graph_data_t iocount;
      };

  private:
      int m_draw_no;

  protected:
      void getMax(struct max_st*, sp<info>&, int limit_count);
      void setData(struct data_st* dt);
      void procData(int dx, struct max_st*, struct data_st*
                  , sp<GdMonDiskstats>& p, sp<GdMonDiskstats>& pp);

      void drawData(int dx, data_st*);
      void drawBottom(sp<MonBase>& base
                  , struct max_st* max
                  , struct data_st* dt);


  public:
      GdMonDiskstats(uint64_t sec);
      virtual ~GdMonDiskstats();

      inline virtual sp<MonBase> create(uint64_t sec){
          return new GdMonDiskstats(sec);
      }
      virtual const char* savename();
      virtual const char* title();
      virtual sp<Retval> draw(int num, sp<info>&, sp<MonBase>&, const char*);

  };

};

#endif /* DFRAMEWORK_MONITOR_GDMONDISKSTATS_H */

