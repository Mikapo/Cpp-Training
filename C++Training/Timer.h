#pragma once

#include <chrono>

namespace mikalib
{
    
    class timer
    {
    
     

    public:
        
        void start();
        void stop();
        double get_current_time();

    private:

        using clock = std::chrono::high_resolution_clock;
        using time_point = std::chrono::time_point<clock>;
        using microseconds = std::chrono::microseconds;

        time_point m_start_point;
        time_point m_end_point;
        bool has_stopped = false;
    };
} // namespace mikalib
