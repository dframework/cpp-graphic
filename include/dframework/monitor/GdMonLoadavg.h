#ifndef DFRAMEWORK_MONITOR_GDMONLOADAVG_H
#define DFRAMEWORK_MONITOR_GDMONLOADAVG_H

#include <dframework/monitor/GdBaseMonitor.h>
#include <dframework/monitor/MonLoadavg.h>

namespace dframework {

  class GdMonLoadavg : public MonLoadavg 
                  , public GdBaseMonitor
  {
  public:
      struct max_st {
          uint64_t a;
      };

      struct data_st{
          dgd_graph_data_t m1;
          dgd_graph_data_t m5;
          dgd_graph_data_t m15;
      };

  protected:
      void getMax(struct max_st*, sp<info>&, int limit_count);
      void setData(struct data_st* dt);
      void procData(int dx, struct max_st*, struct data_st*
                  , sp<GdMonLoadavg>& p, sp<GdMonLoadavg>& pp);

      void drawData(int dx, data_st*);
      void drawBottom(sp<MonBase>& base
                  , struct max_st* max
                  , struct data_st* dt);

  public:
      GdMonLoadavg(uint64_t sec);
      virtual ~GdMonLoadavg();

      inline virtual const char* title(){
          return "LOADAVG";
      }

      virtual sp<Retval> draw(int num, sp<info>&, sp<MonBase>&, const char*);

  };

};

#endif /* DFRAMEWORK_MONITOR_GDMONLOADAVG_H */

