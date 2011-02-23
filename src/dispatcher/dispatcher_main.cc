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

#include <protocol/TBinaryProtocol.h>
#include <server/TThreadedServer.h>
#include <thrift/concurrency/Mutex.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

#include "dispatcher/dispatcher_handler.h"

using boost::shared_ptr;

int startServer(int port) {
    shared_ptr<eval::DispatcherHandler> handler(
      new eval::DispatcherHandler());

    shared_ptr<apache::thrift::TProcessor> processor(
      new DispatcherProcessor(handler));

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

  return 0;
}

