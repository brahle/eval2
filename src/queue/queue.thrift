
	
service QueueService {
  
  bool ping(),
  
  void addToQueue(1:i32 submissionID, 2:i32 taskType, 3:string payload)

}
