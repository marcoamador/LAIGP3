#include "TimerS.h"


TimerS::TimerS(void)
{
	minutes=0;
	seconds=0;
}

void TimerS::increment(){
    if(seconds++ >= 60){
        seconds=0;
        minutes++;
	}
}

std::string TimerS::getTime(){
    std::stringstream ss;
    if(minutes<10) ss<<"0";
		ss << minutes << ":";
    if(seconds<10) ss<<"0";
		ss << seconds;
    return ss.str();
}

int TimerS::getSeconds(){
    return seconds;
}

void TimerS::setSeconds(int sec){
    seconds = sec;
}

void TimerS::decrement(){
    --seconds;
}



