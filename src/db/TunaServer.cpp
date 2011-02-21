/**
 * Copyright 2011 Anton Grbin
 *
 */

#include <string>

#include <protocol/TBinaryProtocol.h>
#include <server/TThreadedServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include <thrift/concurrency/Mutex.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>

#include "db/gen-cpp/Tuna.h"

using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using apache::thrift::concurrency::Mutex;
using apache::thrift::concurrency::Guard;

using boost::shared_ptr;

namespace eval {

class TunaHandler : virtual public TunaIf {
 public:
  TunaHandler() {
    // Your initialization goes here
  }

  bool ping() {
    // Your implementation goes here
    printf("ping\n");

    return 1;
  }

  void simpleQuery(std::vector<int32_t> & _return, const std::string& query) {
    // Your implementation goes here
    std::vector<int> tmp;
    for (unsigned int i = 0; i < query.size(); ++i)
      tmp.push_back(i);

    _return = tmp;
    printf("simpleQuery\n");
  }

};

} // namespace

void startServer(int port) {
  shared_ptr<eval::TunaHandler> handler(
    new eval::TunaHandler());
  
  shared_ptr<TProcessor> processor(
    new TunaProcessor(handler));

  shared_ptr<TServerTransport> serverTransport(
    new TServerSocket(port));

  shared_ptr<TTransportFactory> transportFactory(
    new TBufferedTransportFactory());

  shared_ptr<TProtocolFactory> protocolFactory(
    new TBinaryProtocolFactory());

  TThreadedServer server(
			 processor, 
			 serverTransport, 
			 transportFactory, 
			 protocolFactory
			 );
  
  server.serve();
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  // TODO read server configuration

  int port = atoi(argv[1]);

  startServer(port);
  
  // TODO add the client side for worker service

  return 0;
}
