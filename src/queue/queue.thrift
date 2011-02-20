
	
service QueueService {
  
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
