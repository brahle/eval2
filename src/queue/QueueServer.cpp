/**
 * Copyright 2011 Matija Osrecki
 *
 * TODO copy notice
 */

#include <queue>
#include <string>
#include <map>

#include <protocol/TBinaryProtocol.h>
#include <server/TThreadedServer.h>
#include <thrift/concurrency/Mutex.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "queue/gen-cpp/QueueService.h"

#include "queue/SyncedQueue.hpp"
#include "queue/Worker.hpp"

using apache::thrift::concurrency::Mutex;
using apache::thrift::concurrency::Guard;

using boost::shared_ptr;

namespace eval {

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
    printf("Queue server. Piiinged.\n");

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
   * One of the RPC services.
   *
   * Registers a worker. Return 0 if everything went OK.
   */
  int registerWorker(int workerId, const std::string& ip, int port) {
    if (workers_.find(workerId) != workers_.end()) {
      return 1;
    }

    {
      Guard g(registerMutex_);
      workers_[workerId] = Worker(ip, port);
      printf("Worker %d registered. Ip '%s', port %d.\n",
        workerId, ip.c_str(), port);
    }

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

    // TODO(prasko): send task to worker
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


  /**
   * Used for registering workers.
   */
  Mutex registerMutex_;
};

}  // namespace

void startServer(int port) {
  shared_ptr<eval::QueueServiceHandler> handler(
    new eval::QueueServiceHandler());

  shared_ptr<apache::thrift::TProcessor> processor(
    new QueueServiceProcessor(handler));

  shared_ptr<apache::thrift::server::TServerTransport> serverTransport(
    new apache::thrift::transport::TServerSocket(port));

  shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory(
    new apache::thrift::transport::TBufferedTransportFactory());

  shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory(
    new apache::thrift::protocol::TBinaryProtocolFactory());

  apache::thrift::server::TThreadedServer server(
    processor, serverTransport, transportFactory, protocolFactory);

  server.serve();
}

int main(int argc, char *argv[]) {
  assert(argc == 2);

  // TODO(prasko): read server configuration

  int port = atoi(argv[1]);

  startServer(port);

  // TODO(prasko): add the client side for worker service

  return 0;
}
