#ifndef DFRAMEWORK_MONITOR_GDBASEMONITOR_H
#define DFRAMEWORK_MONITOR_GDBASEMONITOR_H

#include <dframework/base/Retval.h>
#include <dframework/gd/GDImage.h>
#include <dframework/graph/Graph.h>

namespace dframework {

  class GdBaseMonitor
  {
  protected:
      sp<GDImage>  m_img;
      dgd_graph2_t m_g;

      // -----------------  <---- m_my0_4
      // |   |   |   |   |  <---- m_mh1_4 (1/4 height)
      // |---------------|  <---- m_my1_4
      // |   |   |   |   |
      // |---------------|
      // |   |   |   |   |
      // |---------------|  <---- m_my3_4
      // |   |   |   |   |
      // |---------------|
      //   ^ ^       ^----- m_mx3_4
      //   | `----- m_mx1_4
      //   `------- m_mw1_4 (1/4 width)
      int m_mx1_4;
      int m_mx3_4;
      int m_my1_4;
      int m_my3_4;

      int m_mh1_4; // 1/4 height
      int m_mh2_4; // 2/4 height
      int m_mh3_4; // 3/4 height (m_mh1_4 * 3)

      int m_cwhite;
      int m_cblack;
      int m_cbg;
      int m_coutline;

  private:
      int      m_index_num;
      uint64_t m_depth_sec;

      void drawVertical(uint64_t last_sec, int unit_d, int unit_a
                      , const char* fmt);
      void drawVertical2(uint64_t last_sec, int unit_d, int unit_a
                      , const char* fmt);

  public:
      GdBaseMonitor();
      virtual ~GdBaseMonitor();

      virtual const char* title() = 0;

      virtual sp<Retval> baseInit();

      virtual void drawHead(uint64_t current_sec, uint64_t depth_sec);
      virtual void drawIndex(const char* name, dgd_graph_data_t* d);
      virtual void drawDatumLine(int dx, dgd_graph_data_t* d, int offset=0);
      virtual void drawDescLine(int dx, dgd_graph_data_t* d);
      virtual void drawLast(uint64_t first_sec, uint64_t last_sec);

      virtual sp<Retval> savePng(const char* path, const char* name, unsigned);

  };

};

#endif /* DFRAMEWORK_MONITOR_GDBASEMONITOR_H */

