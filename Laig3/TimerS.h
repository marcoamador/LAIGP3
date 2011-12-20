#pragma once

#include <string>
#include <iosfwd>
#include <sstream>

#ifndef Laig3_timer_h
#define Laig3_timer_h

class TimerS
{
public:
	int minutes;
	int seconds;
	TimerS(void);
	void decrement();
	void setSeconds(int sec);
	int getSeconds();
	std::string getTime();
	void increment();
    void reset(){
        minutes=0;
        seconds=0;
    }
};

#endif
