#ifndef DFRAMEWORK_MONITOR_GDMONSOCKET_H
#define DFRAMEWORK_MONITOR_GDMONSOCKET_H

#include <dframework/monitor/GdBaseMonitor.h>
#include <dframework/monitor/MonSocket.h>

namespace dframework {

  class GdMonSocket : public MonSocket
                  , public GdBaseMonitor
  {
  public:
      struct max_st {
          uint64_t a;
          uint64_t b;
      };

      struct data_st{
          dgd_graph_data_t total;
          dgd_graph_data_t est;

          dgd_graph_data_t fin;
          dgd_graph_data_t wait;
          dgd_graph_data_t close;
          dgd_graph_data_t others;
      };

  private:
      int m_draw_no;

  protected:
      void getMax(struct max_st*, sp<info>&, int limit_count);
      void setData(struct data_st* dt);
      void procData(int dx, struct max_st*, struct data_st*
                  , sp<GdMonSocket>& p, sp<GdMonSocket>& pp);

      void drawData(int dx, data_st*);
      void drawBottom(sp<MonBase>& base
                  , struct max_st* max
                  , struct data_st* dt);


  public:
      GdMonSocket(uint64_t sec);
      virtual ~GdMonSocket();

      inline virtual sp<MonBase> create(uint64_t sec){
          return new GdMonSocket(sec);
      }
      virtual const char* savename();
      virtual const char* title();
      virtual sp<Retval> draw(int num, sp<info>&, sp<MonBase>&, const char*);

  };

};

#endif /* DFRAMEWORK_MONITOR_GDMONSOCKET_H */

