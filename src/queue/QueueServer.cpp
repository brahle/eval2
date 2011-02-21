/**
 * Copyright 2011 Matija Osrecki
 *
 * TODO copy notice
 */

#include <queue>
#include <string>

#include <protocol/TBinaryProtocol.h>
#include <server/TThreadedServer.h>
#include <thrift/concurrency/Mutex.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "queue/gen-cpp/QueueService.h"

#include "queue/SyncedQueue.hpp"

using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using apache::thrift::concurrency::Mutex;
using apache::thrift::concurrency::Guard;

using boost::shared_ptr;

namespace eval { 

  /**
   * Basic information about worker process - IP address and port.
   * 'localhost' can stand instead of the IP address.
   */
class Worker { 

 public:

  /**
   * Default contructor.
   */
  Worker() {}

  /**
   * Plain setter constructor.
   */
  Worker(std::string ip, int port) : ip_(ip), port_(port) {}
    
  /**
   * Returns worker's IP address.
   */
  inline std::string ip() {  return ip_;  }
 
  /**
   * Returns worker's port.
   */
  inline int port() {  return port_;  }
  
 private:

  std::string ip_;  
  int port_;
  
};

  /**
   * This is the class which does both the queue managment and
   * listening for any new tasks. 
   *
   * Main methods are : addTask and freeWorker, which do all the job.
   *
   * Pinging the server is also available.
   *
   */
class QueueServiceHandler : virtual public QueueServiceIf {

 public:
 
  /**
   * Default constructor.
   */
  QueueServiceHandler() {}

  /**
   * One of RPC services, used to ping the server.
   */
  bool ping() {

    // TODO write this async

    return 1;
  }

  /**
   * One of the RPC services. 
   * 
   * Adds the task to the queue or assings a worker to complete it.
   */
  void addTask(int taskId) {
    taskQueue_.push(taskId);
    divideTasks();
  }

  /**
   * One of the RPC services.
   *
   * Sets the worker status as unbusy and looks if there is any
   * unassigned task to assign to the worker.
   */
  void freeWorker(int workerId) {
    workerQueue_.push(workerId);
    divideTasks();
  }

  /**
   * Registers a worker. Return 0 if everything went OK.
   */
  int registerWorker(int workerId, const std::string& ip, int port) {
    
    if(workers_.find(workerId) != workers_.end() ) {
      return 1;
    }

    workers_[workerId] = Worker(ip, port);
    workerQueue_.push(workerId);

    return 0;
  }

 private:

  /**
   * Tries to take one unassigned task and a unbusy worker and assign
   * the task to the worker.
   *
   * TODO such test if a loop is needed.
   */
  void divideTasks() {

    int worker;
    int task;

    {
      // need to make sure that only one thread tries to take all 
      // resources at one moment
      Guard g(divideMutex_);

      try {
	worker = workerQueue_.pop();
      } catch(int) {
	return;
      }

      try {
	task = taskQueue_.pop();
      } catch(int) {
	return;
      }
    }

    // TODO send task to worker
  }
  
  /**
   * Queue of tasks not yet distributed.
   */
  SyncedQueue<int> taskQueue_;

  /**
   * Queue of ready workers.
   */
  SyncedQueue<int> workerQueue_;

  /**
   * Workers' information.
   */
  std::map<int, Worker> workers_;
  
  /**
   * Mutex used in the procedure for dividing tasks among workers.
   */
  Mutex divideMutex_;
};

} // namespace

int main() {
  int port = 9090;

  shared_ptr<eval::QueueServiceHandler> handler(
    new eval::QueueServiceHandler());
  
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
  
  // TODO add the client side for worker service

  return 0;
}
