#ifndef __NTY_CONFIG__
#define __NTY_CONFIG__

namespace nty {

	class Config {
	public:
		Config();

		~Config() noexcept;

		/*
		@brief	Determines the size of ZMQ's IO thread pool
		*/
		static int number_io_threads();
	};
}

#endif