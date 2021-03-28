#include "timer.h"

Timer::Timer(float interval)
{
    this -> interval = interval;
    this -> prev = 0;
}

bool Timer::process_tick()
{
    float current = glfwGetTime();
    if(current - this -> prev >= this -> interval)
    {
        this -> prev = current;
        return true;
    }
    else return false;
}