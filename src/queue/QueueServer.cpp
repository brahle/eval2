/**
 * TODO add licence
 */

#include <protocol/TBinaryProtocol.h>
#include <server/TThreadedServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "QueueService.h"

using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

namespace eval { 

namespace queue {


class QueueServiceHandler : virtual public QueueServiceIf {

 public:
 
  QueueServiceHandler() {
    // XXX add synced queue
  }

  bool ping() {
    // XXX write this async
    return 1;
  }

  void addTask(const int32_t taskId) {
    // XXX add to non-existing queue
  }

  void freeWorkerThread(const int32_t workerId) {
    // XXX 
  }

};

class QueueServer {

 public:
 
  QueueServer(const int &port) {
      shared_ptr<QueueServiceHandler> handler(new QueueServiceHandler());
      shared_ptr<TProcessor> processor(new QueueServiceProcessor(handler));
      shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
      shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
      shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

      server_ = new TThreadedServer(processor, serverTransport, transportFactory, protocolFactory);
  }
  
  void serve() {
    server_->serve();
  }
 
 private:
 
  TThreadedServer* server_;  
  
};

} // namespace

} // namespace

int main() {
  
    return 0;
}
