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

using boost::shared_ptr;

namespace eval {

class WorkerServiceHandler : virtual public WorkerServiceIf {
 public:

  WorkerServiceHandler() {}

  bool ping() {
    // TODO(prasko): implement
    return true;
  }

  void giveTask(const int taskId) {
    // TODO(prasko): implement work
    usleep(5);
  }
};

}  // namespace

void registerSelf(int id, int port) {
  shared_ptr<apache::thrift::transport::TTransport> socket(
    new apache::thrift::transport::TSocket("localhost", 9090));

  shared_ptr<apache::thrift::transport::TTransport> transport(
    new apache::thrift::transport::TBufferedTransport(socket));

  shared_ptr<apache::thrift::protocol::TProtocol> protocol(
    new apache::thrift::protocol::TBinaryProtocol(transport));

  QueueServiceClient client(protocol);

  try {
    transport->open();

    client.ping();

    client.registerWorker(id, "localhost", port);

    transport->close();
  } catch(const apache::thrift::TException &tx) {
    printf("ERROR: %s\n", tx.what());
  }
}

void startServer(int port) {
  shared_ptr<eval::WorkerServiceHandler> handler(
    new eval::WorkerServiceHandler());

  shared_ptr<apache::thrift::TProcessor> processor(
    new WorkerServiceProcessor(handler));

  shared_ptr<apache::thrift::server::TServerTransport> serverTransport(
    new apache::thrift::transport::TServerSocket(port));

  shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory(
    new apache::thrift::transport::TBufferedTransportFactory());

  shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory(
    new apache::thrift::protocol::TBinaryProtocolFactory());

  apache::thrift::server::TSimpleServer server(
    processor, serverTransport, transportFactory, protocolFactory);

  server.serve();
}

int main(int argc, char **argv) {
  assert(argc == 3);

  // TODO(prasko): read configuration: queue's ip+port, worker's ip+port or id

  int id = atoi(argv[1]);
  int port = atoi(argv[2]);

  registerSelf(id, port);

  startServer(port);

  return 0;
}

