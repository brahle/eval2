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
#include "queue/ThreadSafeQueue.h"

using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace apache::thrift::concurrency;

using boost::shared_ptr;

namespace eval { 

namespace queue {

class Worker {
  
 public:
  
  Worker(std::string ip, int port) : ip_(ip), port_(port) {}
    
  inline std::string ip() {
    return ip_;
  }
  
  inline int port() {
    return port_;
  }
  
 private:
  
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

  void addTask(int taskId) {
    taskQueue_.push(taskId);
    taskQueue_.size();
  }

  void freeWorkerThread(const int workerId) {
    
  }

 private:
 
  SyncedQueue<int> taskQueue_;

  SyncedQueue<int> workerQueue_;

  std::vector<Worker> workers_;
  
  Mutex queueMutex_;
  
};

} // namespace

} // namespace

int main() {
  int port = 9090;

  shared_ptr<eval::queue::QueueServiceHandler> handler(
    new eval::queue::QueueServiceHandler());
  
  shared_ptr<TProcessor> processor(
    new QueueServiceProcessor(handler));

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
  
  return 0;
}
