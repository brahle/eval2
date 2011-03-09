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
#include <server/TSimpleServer.h>
#include <transport/TBufferTransports.h>
#include <transport/TServerSocket.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "dispatcher/dispatcher_client.h"

#include "worker/worker_handler.h"

using boost::shared_ptr;

void registerSelf(int id, int port) {
  eval::DispatcherClient client("localhost", port);
  client.ping();
  client.registerWorker(id, "localhost", port); 
  while(1) {}
}

void startServer(int port) {
  shared_ptr<eval::WorkerHandler> handler(
    new eval::WorkerHandler());

  shared_ptr<apache::thrift::TProcessor> processor(
    new WorkerProcessor(handler));

  shared_ptr<apache::thrift::server::TServerTransport> serverTransport(
    new apache::thrift::transport::TServerSocket(port));

  shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory(
    new apache::thrift::transport::TBufferedTransportFactory());

  shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory(
    new apache::thrift::protocol::TBinaryProtocolFactory());

  apache::thrift::server::TSimpleServer server(
    processor, serverTransport, transportFactory, protocolFactory);

  server.serve();
}

/**
  Arguments - worker id, dispatcher port
*/
int main(int argc, char **argv) {
  assert(argc == 3);

  /*
   TODO(prasko): read configuration: 
    - dispatcher's ip+port 
    - worker's ip+port or id
  */

  int id = atoi(argv[1]);
  int port = atoi(argv[2]);

  registerSelf(id, port);

  //  startServer(port);

  return 0;
}

