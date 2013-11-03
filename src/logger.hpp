#ifndef __NTY_LOGGER__
#define __NTY_LOGGER__

#include <mutex>

namespace nty 
{
    /*
    @brief  Trivial logger
    */
    class Logger
    {
    public:
        Logger();
        ~Logger() noexcept;

        /*
        @brief  Logs the given string. UNRELIABLE.
        WARNING: To avoid deadlocks this method doesn't take action if locked.
        Appends endline to argument.
        @todo   Make reliable implementation. Implement singleton pattern.
        */
        void
        log(std::string);

    private:
        std::mutex _mutex;
    };
}

#endif