#include "stdafx.h"
#include "TimeTracer.hpp"
#include "rs/Timer.hpp"

rd::perf::TimeTracer::TimeTracer(const char * _msg){
	msg = _msg; 
	if(!rd::String::endsWith(msg.c_str(),' '))
		msg += " ";
	t = rs::Timer::getTimeStamp();
}

rd::perf::TimeTracer::~TimeTracer(){
	auto end = rs::Timer::getTimeStamp();
	auto total = end - t;
	trace(msg + std::to_string(total));
}
