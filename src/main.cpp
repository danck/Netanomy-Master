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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

using namespace nty;

/**
   Publishes a message on the publisher socket.

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

        // TODO send result to GUI-Socket
    }
}

#define SOCK_PATH "/tmp/test.sock"

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

    // TODO interacts with GUI and PUB+COLL
    // * Server Socket aufsetzen
    // * EINE Clientverbindung akzeptieren
    // * listens for commands on ze GUI socket

    int s, s2, len;
    unsigned t;
    struct sockaddr_un local, remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
    }

    for(;;) {
        int done, n;
        printf("Waiting for a connection...\n");
        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
            perror("accept");
            exit(1);
        }

        printf("Connected.\n");

        done = 0;
        do {
          n = recv(s2, str, 100, 0);

          if (n <= 0) {
              if (n < 0) perror("recv");
              done = 1;
          }
          if (!done) {
            str[n] = '\0';
            std::string msg(str);

            unsigned delm = msg.find(" ");

            std::string plugin_name = msg.substr (0, delm);
            std::string domain_name = msg.substr (delm+1, msg.size() - delm - 2 );

            logger->log(plugin_name);
            logger->log(domain_name);
            logger->log(msg);
            // printf("%s %d modified\n",str, n); 
            if (send(s2, str, n, 0) < 0) {
                perror("send");
                done = 1;
            }
          }
        } while (!done);

        close(s2);
    }


    logger->log("Main: joining publisher and collector");
    pub.join();
    col.join();
    return 0;
}