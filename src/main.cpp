/** @mainpage Netanomy: Master Component
*   @author Yavuz Arslan <yavuz.arslan@haw-hamburg.de>
*   @author Kirchner <daniel.kirchner1@haw-hamburg.de>
*   @version 0.0.1
*   @par Description:
*   - Orchestrates test runs
*   - Interfaces database, slaves and GUI
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
   Publishes a message on the socket.

   @param[in]     _inArg1 Pointer to zmq context.
   @param[in]     _inArg2 Smart-pointer to logger.
   @return void.
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

/**
   Collects incoming results from the socket.

   @param[in]     _inArg1 Pointer to zmq context.
   @param[in]     _inArg2 Smart-pointer to logger.
   @return void.
 */
// 
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

/**
   Starts the master server.
 */
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