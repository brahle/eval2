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

#ifndef EVAL_DISPATCHER_WORKER_H_
#define EVAL_DISPATCHER_WORKER_H_

#include <string>

  /**
   * Basic information about worker process - IP address and port.
   * 'localhost' can stand instead of the IP address.
   */
class Worker {
 public:

  /**
   * Default contructor.
   */
  Worker() {}

  /**
   * Plain setter constructor.
   */
  Worker(std::string ip, int port) : ip_(ip), port_(port) {}

  /**
   * Returns worker's IP address.
   */
  inline std::string ip() {  return ip_;  }

  /**
   * Returns worker's port.
   */
  inline int port() {  return port_;  }

 private:

  std::string ip_;
  int port_;

};

#endif  // EVAL_DISPATCHER_WORKER_H_
