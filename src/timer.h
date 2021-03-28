#include "main.h"

#ifndef TIMER_H
#define TIMER_H
#endif

class Timer
{
    public:
        Timer(float interval);
        bool process_tick();
    private:
        float interval;
        float prev;
};