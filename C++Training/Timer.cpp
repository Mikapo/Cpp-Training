#include "Timer.h"

void mikalib::timer::start()
{
    m_start_point = clock::now();
    has_stopped = false;
}

void mikalib::timer::stop()
{
    m_end_point = clock::now();
    has_stopped = true;
}

double mikalib::timer::get_current_time() 
{  
    time_point end = has_stopped ? m_end_point : clock::now();

    auto start_time = std::chrono::time_point_cast<microseconds>(m_start_point).time_since_epoch();
    auto end_time = std::chrono::time_point_cast<microseconds>(end).time_since_epoch();

    return static_cast<double>(end_time.count()- start_time.count()) * 0.001;

}
