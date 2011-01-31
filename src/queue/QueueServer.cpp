/**
 * TODO add licence
 */

#include <queue>
#include <string>

#include <protocol/TBinaryProtocol.h>
#include <server/TThreadedServer.h>
#include <thrift/concurrency/Mutex.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "queue/QueueService.h"

using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using apache::thrift::concurrency::Guard;

using boost::shared_ptr;

namespace eval { 

namespace queue {

class Worker {
  
 public:
  
  Worker(std::string ip, int port) : busy_(false), ip_(ip), port_(port) {}
  
  inline bool busy() { 
    return busy_;
  }
  
  inline std::string ip() {
    return ip_;
  }
  
  inline int port() {
    return port_;
  }
  
 private:
  
  bool busy_;
  
  std::string ip_;
  
  int port_;
  
};

class QueueServiceHandler : virtual public QueueServiceIf {

 public:
 
  QueueServiceHandler() {}

  bool ping() {
    // TODO write this async
    return 1;
  }

  void addTask(const int taskId) {
    Guard(this->queueMutex);
    
    
  }

  void freeWorkerThread(const int workerId) {
    
  }
  
 private:
 
  std::queue<int> taskQueue;
  
  std::vector<Worker> workers;
  
  apache::thrift::concurrency::Mutex queueMutex;
  
};

} // namespace

} // namespace

int main() {
  int port = 9090;

  shared_ptr<eval::queue::QueueServiceHandler> handler(new eval::queue::QueueServiceHandler());
  
  shared_ptr<TProcessor> processor(new QueueServiceProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TThreadedServer server(processor, serverTransport, transportFactory, protocolFactory);
  
  server.serve();
  
  return 0;
}
