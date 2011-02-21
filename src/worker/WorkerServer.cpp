/**
 * Copyright 2011 Matija Osrecki
 * 
 * TODO copy notice
 */

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "WorkerService.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using boost::shared_ptr;

namespace eval {

class WorkerServiceHandler : virtual public WorkerServiceIf {
 
 public:
 
  WorkerServiceHandler() {
    // Your initialization goes here
  }

  bool ping() {
    // TODO implement
  }

  void giveTask(const int32_t taskId) {
    // TODO implement
  }

};

} // namespace

int main(int argc, char **argv) {
  // TODO read port from a conf file
  int port = 9090;

  shared_ptr<eval::WorkerServiceHandler> handler(
     new eval::WorkerServiceHandler());

  shared_ptr<TProcessor> processor(
    new WorkerServiceProcessor(handler));

  shared_ptr<TServerTransport> serverTransport(
    new TServerSocket(port));

  shared_ptr<TTransportFactory> transportFactory(
    new TBufferedTransportFactory());

  shared_ptr<TProtocolFactory> protocolFactory(
    new TBinaryProtocolFactory());

  TSimpleServer server(
		       processor, 
		       serverTransport, 
		       transportFactory, 
		       protocolFactory
		       );
  
  server.serve();

  return 0;
}

