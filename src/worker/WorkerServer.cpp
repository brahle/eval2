/**
 * Copyright 2011 Matija Osrecki
 * 
 * TODO copy notice
 */

#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TBufferTransports.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

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
    return true;
  }

  void giveTask(const int taskId) {
    // TODO implement work
    usleep(5);    
  }

};

} // namespace

void registerSelf(int id, int port) {
  shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
  shared_ptr<TTransport> transport(new TBufferedTransport(socket));
  shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
  QueueServiceClient client(protocol);

  try {
    transport->open();
    
    client.ping();

    client.registerWorker(id, "localhost", port);
    
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
  assert(argc == 3);

  // TODO read configuration: queue's ip+port, worker's ip+port or id

  int id = atoi(argv[1]);
  int port = atoi(argv[2]);

  registerSelf(id, port);

  startServer(port);

  return 0;
}

