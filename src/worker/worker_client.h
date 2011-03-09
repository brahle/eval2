#ifndef EVAL_WORKER_CLIENT_H_
#define EVAL_WORKER_CLIENT_H_

#include <string>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TTransportUtils.h>
#include <boost/shared_ptr.hpp>

#include "gen-cpp/Worker.h"

namespace eval { 

class WorkerClient {
 public:

  WorkerClient(const std::string &ip, const int port) {
    boost::shared_ptr<apache::thrift::transport::TTransport> socket(
      new apache::thrift::transport::TSocket(ip, port));

    transport_ = boost::shared_ptr<apache::thrift::transport::TTransport>(
      new apache::thrift::transport::TBufferedTransport(socket));

    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol(
      new apache::thrift::protocol::TBinaryProtocol(transport_));

    client_ = boost::shared_ptr< ::WorkerClient>(
      new ::WorkerClient(protocol));

    try {
      transport_->open();
    } catch(const apache::thrift::TException &tx) {
      // TODO(someone): log this
    }
  }

  ~WorkerClient() {
    try {
      transport_->close();
    } catch(const apache::thrift::TException &tx) {
      // TODO(someone): log this
    }
  }

  void ping() {
    client_->ping();
  }

  void giveJob(const int jobId) {
    client_->giveJob(jobId);
  }

 private:
  boost::shared_ptr< ::WorkerClient> client_;
  boost::shared_ptr<apache::thrift::transport::TTransport> transport_;

};

}  // namespace

#endif  // EVAL_WORKER_CLIENT_H_
