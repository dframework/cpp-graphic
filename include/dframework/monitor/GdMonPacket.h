#ifndef DFRAMEWORK_MONITOR_GDMONPACKET_H
#define DFRAMEWORK_MONITOR_GDMONPACKET_H

#include <dframework/monitor/GdBaseMonitor.h>
#include <dframework/monitor/MonPacket.h>

namespace dframework {

  class GdMonPacket : public MonPacket
                  , public GdBaseMonitor
  {
  public:
      struct max_st {
          uint64_t b; // bytes
          uint64_t p; // packets
      };

      struct data_st{
          dgd_graph_data_t rbytes;
          dgd_graph_data_t rpackets;
          dgd_graph_data_t rerrs;
          dgd_graph_data_t tbytes;
          dgd_graph_data_t tpackets;
          dgd_graph_data_t terrs;
      };

  protected:
      void getMax(struct max_st*, sp<info>&, int limit_count);
      void setData(struct data_st* dt);
      void procData(int dx, struct max_st*, struct data_st*
                  , sp<GdMonPacket>& p, sp<GdMonPacket>& pp);

      void drawData(int dx, data_st*);
      void drawBottom(sp<MonBase>& base
                  , struct max_st* max
                  , struct data_st* dt);


  public:
      GdMonPacket(uint64_t sec);
      virtual ~GdMonPacket();

      inline virtual sp<MonBase> create(uint64_t sec){
          return new GdMonPacket(sec);
      }
      inline virtual const char* title(){
          return "PACKET";
      }

      virtual sp<Retval> draw(int num, sp<info>&, sp<MonBase>&, const char*);

  };

};

#endif /* DFRAMEWORK_MONITOR_GDMONPACKET_H */

