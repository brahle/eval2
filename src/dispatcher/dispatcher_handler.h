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

#ifndef EVAL_DISPATCHER_DISPATCHER_HANDLER_H_
#define EVAL_DISPATCHER_DISPATCHER_HANDLER_H_

#include <string>
#include <map>

#include <dispatcher/gen-cpp/Dispatcher.h>
#include <dispatcher/worker_data.h>
#include <utility/synced_queue.h>

namespace eval {
  /**
   *
   * Interface for the DispatcherHandler, who's job is to handle 
   * incoming requests for the server.
   *
   */
class DispatcherHandler : virtual public DispatcherIf {
 public:

  /** 
   * Default constructor.
   */
  DispatcherHandler() {}

  /**
   * It should be used for pinging the server to see it's status but
   * it's not yet implemented. 
   * 
   * Should be extended by sending some data back, like it's current load...
   */
  bool ping();

  /**
   * Adds the task to the queue or assings a worker to complete it.
   */
  void addTask(const int taskId);

  /**
   * Sets the worker status to unbusy and looks if there is any
   * unassigned task to assign to the worker.
   */
  void freeWorker(const int workerId);

  /**
   * Used by a worker to register himself to the dispatcher.
   */
  int registerWorker(const int workerId, const std::string& ip, const int port);

 private:

  /**
   * Tries to take one unassigned task and a unbusy worker and assign
   * the task to the worker.
   *
   * TODO(prasko): such test if a loop is needed.
   */
  void divideTasks();

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


#endif  // EVAL_DISPATCHER_DISPATCHER_HANDLER_H_
