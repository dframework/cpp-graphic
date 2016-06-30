#ifndef DFRAMEWORK_MONITOR_GDMONCPUSTAT_H
#define DFRAMEWORK_MONITOR_GDMONCPUSTAT_H

#include <dframework/monitor/GdBaseMonitor.h>
#include <dframework/monitor/MonCpustat.h>

namespace dframework {

  class GdMonCpustat : public MonCpustat
                  , public GdBaseMonitor
  {
  public:
      struct max_st {
          uint64_t t;
          uint64_t i;
          uint64_t w;
          uint64_t p;
      };

      struct data_st{
          dgd_graph_data_t total;
          dgd_graph_data_t idle;
          dgd_graph_data_t user;
          dgd_graph_data_t syst;
          dgd_graph_data_t nice;
          dgd_graph_data_t iowa;
          dgd_graph_data_t irq;
          dgd_graph_data_t soft;
          dgd_graph_data_t proc;
          dgd_graph_data_t runn;
          dgd_graph_data_t blk;
      };

  protected:
      void getMax(struct max_st*, sp<info>&, int limit_count);
      void setData(struct data_st* dt);
      void procData(int dx, struct max_st*, struct data_st*
                  , sp<GdMonCpustat>& p, sp<GdMonCpustat>& pp);

      void drawData(int dx, data_st*);
      void drawBottom(sp<MonBase>& base
                  , struct max_st* max
                  , struct data_st* dt);


  public:
      GdMonCpustat(uint64_t sec);
      virtual ~GdMonCpustat();

      inline virtual sp<MonBase> create(uint64_t sec){
          return new GdMonCpustat(sec);
      }
      inline virtual const char* title(){
          return "CPU STAT";
      }

      virtual sp<Retval> draw(int num, sp<info>&, sp<MonBase>&, const char*);

      inline uint64_t getMaxWithoutIdle(sp<GdMonCpustat>& c){
          return (c->m_all->m_user 
              + c->m_all->m_nice
              + c->m_all->m_system
              + c->m_all->m_iowait
              + c->m_all->m_irq
              + c->m_all->m_softirq);
      }

  };

};

#endif /* DFRAMEWORK_MONITOR_GDMONCPUSTAT_H */

