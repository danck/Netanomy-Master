#include <iostream>
#include <zmq.hpp>
#include <iostream>
#include "logger.hpp"

int main(int argc, char ** argv)
{
  std::cout << "hallo\n";
  logIt * logger;
  logIt(loglevel_e::logDEBUG) << "Hallo1 \n";
  logIt(loglevel_e::logDEBUG) << "Hallo2 \n";
  logIt(loglevel_e::logDEBUG) << "Hallo3 \n";

  while (true)
  {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    std::cout << "Listening on tcp://*:11001"  << std::endl;
    socket.bind("tcp://*:11001");

    while(true)
    {
      zmq::message_t request;

      socket.recv(&request);
      std::cout << "Yay, got something. Press Key to reply..."  << std::endl;
      std::cin.ignore();
      zmq::message_t reply(5);
      memcpy((void*) reply.data(), "1234", 4);
      socket.send(reply);
      std::cout << "hab was gesendet"  << std::endl;
    }
  }
  return 0;
}

/*
void logTest(void) {
    loglevel_e loglevel_save = loglevel;

    loglevel = logDEBUG4;

    log(logINFO) << "foo " << "bar " << "baz";

    int count = 3;
    log(logDEBUG) << "A loop with "    << count << " iterations";
    for (int i = 0; i != count; ++i)
    {
        log(logDEBUG1) << "the counter i = " << i;
        log(logDEBUG2) << "the counter i = " << i;
    }

    loglevel = loglevel_save;
}
*/
