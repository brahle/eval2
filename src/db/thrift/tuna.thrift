include "../../models/models.thrift"

namespace cpp eval.tuna

exception TunaExp {
  1: i32 what,
  2: string why,
  3: string query,
  4: i64 oid
}

struct simpleQ {
  1: i32 size,
  2: list<i32> nums 
}

struct doubleQ {
  1: i32 size,
  2: list<i32> nums,
  3: list<string> strs
}

struct fullQ {
  1: i32 size,
  2: list<string> col_name,
  3: list< list<string> > data 
}

service Tuna {

  bool ping(),

  simpleQ simpleQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  doubleQ doubleQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  fullQ fullQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  simpleQ reserveFrom(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  void reserve(1:list<i32> ids)

  bool destroy(1:i32 id)

  // *** Task ***
  	
  	models.Task getTask(1: i32 id),
  	list<models.Task> getTasks(1: list<i32> ids),
  	list<models.Task> getTasksFrom(
  	  1: string qname,
  	  2: list<string> data
  	) throws(1: TunaExp e),
  	
  	bool updateTask(1: models.Task task) throws(1: TunaExp e),
  	i32 insertTask(1: models.Task task) throws (1: TunaExp e),

  // *** TestCase ***
  	
  	models.TestCase getTestCase(1: i32 id),
  	list<models.TestCase> getTestCases(1: list<i32> ids),
  	list<models.TestCase> getTestCasesFrom(
  	  1: string qname,
  	  2: list<string> data
  	) throws(1: TunaExp e),
  	
  	bool updateTestCase(1: models.TestCase test_case) throws(1: TunaExp e),
  	i32 insertTestCase(1: models.TestCase test_case) throws (1: TunaExp e),

  // *** Submission ***
  	
  	models.Submission getSubmission(1: i32 id),
  	list<models.Submission> getSubmissions(1: list<i32> ids),
  	list<models.Submission> getSubmissionsFrom(
  	  1: string qname,
  	  2: list<string> data
  	) throws(1: TunaExp e),
  	
  	bool updateSubmission(1: models.Submission submission) throws(1: TunaExp e),
  	i32 insertSubmission(1: models.Submission submission) throws (1: TunaExp e),

  // *** Job ***
  	
  	models.Job getJob(1: i32 id),
  	list<models.Job> getJobs(1: list<i32> ids),
  	list<models.Job> getJobsFrom(
  	  1: string qname,
  	  2: list<string> data
  	) throws(1: TunaExp e),
  	
  	bool updateJob(1: models.Job job) throws(1: TunaExp e),
  	i32 insertJob(1: models.Job job) throws (1: TunaExp e)
}


