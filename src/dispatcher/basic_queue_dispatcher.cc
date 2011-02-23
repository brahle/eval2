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

#include <queue>
#include <string>
#include <map>

#include "dispatcher/dispatcher_handler.h"
#include "dispatcher/worker_data.h"
#include "utility/synced_queue.h"

using apache::thrift::concurrency::Mutex;
using apache::thrift::concurrency::Guard;

using boost::shared_ptr;

namespace eval {

bool DispatcherHandler::ping() {
  printf("Queue server. Piiinged.\n");
  return 1;
}


void DispatcherHandler::addTask(const int taskId) {
  this->taskQueue_.push(taskId);
  divideTasks();
}


void DispatcherHandler::freeWorker(const int workerId) {
  this->workerQueue_.push(workerId);
  divideTasks();
}

int DispatcherHandler::registerWorker(const int workerId,
  const std::string& ip, const int port) {

  {
    Guard g(this->registerMutex_);

    if (this->workers_.find(workerId) != this->workers_.end()) {
      return 1;
    }

    this->workers_[workerId] = Worker(ip, port);

    printf("Worker %d registered. Ip '%s', port %d.\n",
      workerId, ip.c_str(), port);
  }

  this->workerQueue_.push(workerId);

  return 0;
}

void DispatcherHandler::divideTasks() {
  int worker;
  int task;

  {
    // need to make sure that only one thread tries to take all
    // resources at one moment
    Guard g(this->divideMutex_);

    try {
      worker = this->workerQueue_.pop();
    } catch(int) {
      return;
    }

    try {
      task = this->taskQueue_.pop();
    } catch(int) {
      return;
    }
  }

  // TODO(prasko): send task to worker
}

}  // namespace
