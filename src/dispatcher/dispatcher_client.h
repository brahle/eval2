#ifndef EVAL_DISPATCHER_CLIENT_H_
#define EVAL_DISPATCHER_CLIENT_H_

#include <string>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TTransportUtils.h>
#include <boost/shared_ptr.hpp>

#include "gen-cpp/Dispatcher.h"

namespace eval { 

class DispatcherClient {
 public:

  DispatcherClient(const std::string &ip, const int port) {
    boost::shared_ptr<apache::thrift::transport::TTransport> socket(
      new apache::thrift::transport::TSocket(ip, port));

    transport_ = boost::shared_ptr<apache::thrift::transport::TTransport>(
      new apache::thrift::transport::TBufferedTransport(socket));

    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol(
      new apache::thrift::protocol::TBinaryProtocol(transport_));

    client_ = boost::shared_ptr< ::DispatcherClient>(
      new ::DispatcherClient(protocol));

    try {
      transport_->open();
    } catch(const apache::thrift::TException &tx) {
      // TODO(someone): log this

    }
  }
  ~DispatcherClient() {
    try {
      transport_->close();
    } catch(const apache::thrift::TException &tx) {
      // TODO(someone): log this
    }
  }

  bool ping() {
    return client_->ping();
  }

  void addTask(const int taskId) {
    client_->addTask(taskId);
  }

  void freeWorker(const int workerId) {
    client_->freeWorker(workerId);
  }

  int registerWorker(const int workerId, const std::string & ip, const int port)
      {
    return client_->registerWorker(workerId, ip, port);
  }

 private:
  boost::shared_ptr< ::DispatcherClient> client_;
  boost::shared_ptr<apache::thrift::transport::TTransport> transport_;

};

}  // namespace

#endif  // EVAL_DISPATCHER_CLIENT_H_
