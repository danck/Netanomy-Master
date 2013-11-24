/** @mainpage ProjectX: bla, bla, bla
*   @par Description:
*   - Point 1 bla, bla, bla
*   - Point 2 bla, bla, bla
*   - Point 3 bla, bla, bla
*
*   @par Next paragraph - more information:
*    bla, bla, bla
*    bla, bla, bla
*/

#include <iostream>
#include <zmq.hpp>
#include <memory>
#include <string>
#include <thread>
#include "logger.hpp"
#include "config.hpp"
#include "zhelpers.hpp"

using namespace nty;

/**
   Write description of function here.
   The function should follow these comments.
   Use of "brief" tag is optional. (no point to it)
  
   The function arguments listed with "param" will be compared 
   to the declaration and verified.

   @param[in]     _inArg1 Description of first function argument.
   @param[out]    _outArg2 Description of second function argument.
   @param[in,out] _inoutArg3 Description of third function argument.
   @return Description of returned value.
 */
void 
publisher(zmq::context_t* context, std::shared_ptr<Logger> logger)
{
    zmq::socket_t publisher(*context, ZMQ_PUB);
    publisher.bind("tcp://*:11000");
    while (true)
    {
        std::cin.ignore();
        zhelpers::s_sendmore (publisher, "Linux");
        zhelpers::s_send (publisher, "0 Hallo Echo");
        logger->log("Publisher: sent \"0 Hallo\" to Channel \"Linux\" ");
    }
}

// Collects incoming results
void
collector(zmq::context_t* context, std::shared_ptr<Logger> logger)
{
    zmq::socket_t collector(*context, ZMQ_REP);    
    collector.bind("tcp://*:11001");
    logger->log("Collector: Listening on tcp://*:11001");
    zmq::message_t request;

    while (true)
    {
        std::string result = zhelpers::s_recv(collector);
        // collector.recv(&request);
        logger->log(result);
        zhelpers::s_send(collector, "ACK");
    }
}

int
main(int argc, char** argv)
{
    const std::shared_ptr<Logger> logger(new Logger());
    logger->log("Master starting up...");

    // Construct ZMQ context
    zmq::context_t context(nty::Config::number_io_threads());

    // starts publisher and collector
    std::thread pub (publisher, &context, logger);
    std::thread col (collector, &context, logger);

    // interacts with GUI and PUB+COLL

    logger->log("Main: joining publisher and collector");
    pub.join();
    col.join();
    return 0;
}