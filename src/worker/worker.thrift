/**
 * Copyright 2011 Matija Osrecki
 *
 * TODO copy notice
 */

service WorkerService {
	
  bool ping(), 

  /**
   * Sets the given task for computation on the worker.
   */
  oneway void giveTask(1:i32 taskId)
}