/**
 * Copyright 2011 Matija Osrecki
 * 
 * TODO copy notice
 */

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "queue/gen-cpp/QueueService.h"
#include "worker/gen-cpp/WorkerService.h"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using boost::shared_ptr;

namespace eval {

class WorkerServiceHandler : virtual public WorkerServiceIf {
 
 public:
 
  WorkerServiceHandler() {}

  bool ping() {
    // TODO implement
  }

  void giveTask(const int taskId) {
    // TODO implement work
    sleep(5);
    
    
  }

};

} // namespace

void registerSelf(int port) {
  shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  WorkerServiceClient client(protocol);

  try {
    transport->open();
    
    
    
    transport->close();
  } catch (TException &tx) {
    printf("ERROR: %s\n", tx.what());
  }
}

void startServer(int port) {
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
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("Program takes 1 argument: port");
    return 1;
  }

  // TODO read configuration: queue's ip+port, worker's ip+port or id

  int port = atoi(argv[1]);

  registerSelf(port);

  startServer(port);

  return 0;
}

