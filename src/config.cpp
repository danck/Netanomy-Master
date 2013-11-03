#include "config.hpp"

using namespace nty;

Config::Config() {};
Config::~Config() noexcept {};

int 
Config::number_io_threads()
{
	// currently just a random number, needs proper implementation
	return 4;
}