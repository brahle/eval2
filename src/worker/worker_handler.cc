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

#include "dispatcher/dispatcher_client.h"
#include "worker/gen-cpp/Worker.h"
#include "worker/worker_handler.h"

namespace eval {

bool WorkerHandler::ping(){
  // TODO(prasko): implement
  return true;
}

void WorkerHandler::giveTask(const int taskId) {
  // TODO(prasko): implement work
  usleep(5);
}

}  // namespace
