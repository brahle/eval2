/**
 * Copyright 2011 Matija Osrecki
 *
 * This file is part of Evaluator.
 * 
 * Evaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * Evaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public
 * License along with Evaluator. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef EVAL_DISPATCHER_CLIENT_H_
#define EVAL_DISPATCHER_CLIENT_H_

#include <string>

#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <transport/TTransportUtils.h>

#include "dispatcher/gen-cpp/Dispatcher.h"

using boost::shared_ptr;

using apache::thrift::transport::TTransport;
using apache::thrift::transport::TSocket;
using apache::thrift::transport::TBufferedTransport;
using apache::thrift::protocol::TProtocol;
using apache::thrift::protocol::TBinaryProtocol;

namespace eval {

class DefaultDispatcherClient {
 public:

  DefaultDispatcherClient(const std::string &ip, const int port) {
    shared_ptr<TTransport> socket(new TSocket(ip, port));
    shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport_));
    shared_ptr<DispatcherClient> client(new DispatcherClient(protocol));
    
    this->transport_ = transport;
    this->client_ = client;
  }

  shared_ptr<TTransport> transport() {
    return transport_;
  }

  shared_ptr<DispatcherClient> client() {
    return client_;
  }

 private:
  
  shared_ptr<TTransport> transport_;

  shared_ptr<DispatcherClient> client_;
};

}  // namespace

#endif  // EVAL_DISPATCHER_CLIENT_H_
