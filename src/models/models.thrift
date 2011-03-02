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

/*
 * Just some structures to test thrift-to-psql script. 
 */

namespace cpp eval.model

struct Task {         // tasks
 1: i32 id,           // PRIMARY KEY
 2: i32 type,
 3: string filePath   // varchar(150)
}

struct TestCase {     // test_cases
 1: i32 id,           // PRIMARY KEY
 2: i32 taskId,       // REFERENCES tasks(id)
 3: string filePath   // varchar(100)
}

struct Submission {   // submissions
 1: i32 id,           // PRIMARY KEY
 2: i32 taskId,       // REFERENCES tasks(id)
 3: string codePath   // varchar(150)
}

struct Job {          // jobs
 1: i32 id,           // PRIMARY KEY
 2: i32 test_case_id, // REFERENCES test_cases(id)
 3: bool finished
}
