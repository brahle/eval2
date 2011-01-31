
	
service QueueService {
  
  bool ping(),
  
  /**
   * Adds task to the queue.
   */
  void addTask(1:i32 taskId),
  
  /**
   * Set the worker with the given ID as free and notifies all worker threads.
   */
  void freeWorker(1:i32 workerId)

}
