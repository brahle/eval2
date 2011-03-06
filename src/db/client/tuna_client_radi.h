#ifndef EVAL_TUNA_CLIENT_H_
#define EVAL_TUNA_CLIENT_H_

#include <string>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TTransportUtils.h>
#include <boost/shared_ptr.hpp>

#include "gen-cpp/Tuna.h"

using namespace eval::tuna;
using namespace eval::model;

namespace eval { 

class TunaClient {
 public:

  TunaClient(const std::string &ip, const int port) {
    boost::shared_ptr<apache::thrift::transport::TTransport> socket(
      new apache::thrift::transport::TSocket(ip, port));

    boost::shared_ptr<apache::thrift::transport::TTransport> transport(
      new apache::thrift::transport::TBufferedTransport(socket));

    boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol(
      new apache::thrift::protocol::TBinaryProtocol(transport_));

    boost::shared_ptr< ::TunaClient> client(
      new ::TunaClient(protocol));

    client_ = client;
    transport_ = transport;
    transport_->open();
  }

  ~TunaClient() {
    try {
      transport_->close();
    } catch(const apache::thrift::TException &tx) {
      // TODO(someone): log this
    }
  }

  bool ping() {
    return client_->ping();
  }

  void reserve(const std::vector< int >& ids) {
    client_->reserve(ids);
  }

  bool destroy(const int id) {
    return client_->destroy(id);
  }

  void getTasks(std::vector< Task > & return_, const std::vector< int >&
    ids) {
    client_->getTasks(return_, ids);
  }


 private:
  boost::shared_ptr< ::TunaClient> client_;
  boost::shared_ptr<apache::thrift::transport::TTransport> transport_;

};

}  // namespace

#endif  // EVAL_TUNA_CLIENT_H_
