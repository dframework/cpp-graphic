#ifndef DFRAMEWORK_MONITOR_GDMONMEMORY_H
#define DFRAMEWORK_MONITOR_GDMONMEMORY_H

#include <dframework/monitor/GdBaseMonitor.h>
#include <dframework/monitor/MonMemory.h>

namespace dframework {

  class GdMonMemory : public MonMemory
                  , public GdBaseMonitor
  {
  public:
      struct max_st {
          uint64_t a;
          uint64_t b;
      };

      struct data_st{
          dgd_graph_data_t free;
          dgd_graph_data_t buffers;
          dgd_graph_data_t cached;
          dgd_graph_data_t kernel;

          dgd_graph_data_t swUsed;
          dgd_graph_data_t vmUsed;
      };

#if 0
uint64_t h = m_kernelUsed + m_cached + m_buffers + m_free;
if( m_total == h ){
        printf("ok-");
}else{
        printf("fa-");
}

uint64_t a = m_swapUsed + m_total;
uint64_t u = m_kernelUsed - m_vmallocUsed;
printf("total=%lu, kernel=%lu, cached=%lu, buffer=%lu, free=%lu"
        ", a=%lu, swapu=%lu, vmu=%lu"
        "\n"
        , m_total, m_kernelUsed, m_cached, m_buffers, m_free
        , a, m_swapUsed, m_vmallocUsed
);
#endif
  private:
      int m_draw_no;

  protected:
      void getMax(struct max_st*, sp<info>&, int limit_count);
      void setData(struct data_st* dt);
      void procData(int dx, struct max_st*, struct data_st*
                  , sp<GdMonMemory>& p, sp<GdMonMemory>& pp);

      void drawData(int dx, data_st*);
      void drawBottom(sp<MonBase>& base
                  , struct max_st* max
                  , struct data_st* dt);

  public:
      GdMonMemory(uint64_t sec);
      virtual ~GdMonMemory();

      inline virtual sp<MonBase> create(uint64_t sec){
          return new GdMonMemory(sec);
      }
      inline virtual const char* title(){
          return ((m_draw_no==0) ? "MEMORY (A)" : "MEMORY (B)");
      }
      inline virtual const char* savename(){
          return ((m_draw_no==0) ? "memory-a" : "memory-b");
      }

      virtual sp<Retval> draw(int num, sp<info>&, sp<MonBase>&, const char*);

  };

};

#endif /* DFRAMEWORK_MONITOR_GDMONMEMORY_H */

