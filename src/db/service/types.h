#include <string>
#include <result.hxx>
#include <model/models.h>

namespace eval { namespace tuna { 

template <class T>
shared_ptr<T> convertTo(const tuple &t) throw(int) {
  throw 1;
}

template<>
shared_ptr<Task> convertTo(const tuple &t) throw(int) {
  shared_ptr<Task> task(new Task());

  task.id = t[0].as<int>();
  task.title = t[1].as<std::string>();
  task.text = t[2].as<std::string>();

  return task;
}

template<>
shared_ptr<User> convertTo(const tuple &t) throw(int) {
  shared_ptr<User> user(new User());

  user.id = t[0].as<int>();
  user.ime = t[1].as<std::string>();
  user.prezime = t[2].as<std::string>();

  return user;
}

template<>
shared_ptr<Solution> convertTo(const tuple &t) throw(int) {
  shared_ptr<Solution> solution(new Solution());

  solution.id = t[0].as<int>();
  solution.taskId = t[1].as<int>();
  solution.userId = t[2].as<int>();
  solution.programingLanguageId = t[3].as<int>();
  solution.source = t[4].as<std::string>();
  solution.timeSubmited = t[5].as<int>();

  return solution;
}

} }  // namespace
