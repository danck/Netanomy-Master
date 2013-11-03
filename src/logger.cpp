#include <iostream>
#include "logger.hpp"

using namespace nty;

Logger::Logger()
	: _mutex()
{}
Logger::~Logger() noexcept
{}

void
Logger::log(std::string msg)
{	
	if (_mutex.try_lock())
	{
		std::cout << msg << std::endl;
		_mutex.unlock();
	} 
	else
	{
		// failed to log
	}
}