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
service Dispatcher {
  
  bool ping(),
  
  /**
   * Adds task to the queue.
   */
  void addTask(1:i32 taskId),
  
  /**
   * Set the worker with the given ID as free and notifies all worker threads.
   */
  void freeWorker(1:i32 workerId),

  /**
   * Registers the worker. Returns 0 if everything went OK.
   */
  i32 registerWorker(1:i32 workerId, 2:string ip, 3:i32 port)

}
