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

#include <server/TSimpleServer.h>
#include "dispatcher/dispatcher_client.h"
#include "utility/ServerFactory.h"
#include "worker/worker_handler.h"
#include "worker/gen-cpp/Worker.h"

using apache::thrift::server::TSimpleServer;
using boost::shared_ptr;

void registerSelf(int id, int dport, int wport) {
  eval::DispatcherClient client("localhost", dport);
  client.ping();
  client.registerWorker(id, "localhost", wport); 
}

void startServer(int wport) {
  shared_ptr<TSimpleServer> server = 
    eval::util::ServerFactory<eval::WorkerHandler, 
    WorkerProcessor, TSimpleServer>::createServer(wport);

  server->serve();
}

/**
  Arguments - worker id, dispatcher port, worker port
*/
int main(int argc, char **argv) {
  assert(argc == 4);

  /*
   TODO(prasko): read configuration: 
    - dispatcher's ip+port 
    - worker's ip+port or id
  */

  int id = atoi(argv[1]);
  int dispatcher_port = atoi(argv[2]);
  int worker_port = atoi(argv[3]);

  registerSelf(id, dispatcher_port, worker_port);

  startServer(worker_port);

  return 0;
}

