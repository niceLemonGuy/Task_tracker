#include <iostream>
#include <vector>
#include <tuple>
#include <map>
using namespace std;

enum class TaskStatus {
    NEW,
    IN_PROGRESS,
    TESTING,
    DONE
};

using TasksInfo = map<TaskStatus, int>;

bool CheckTaskStatus(TasksInfo tasks, TaskStatus status) {
    if(tasks.count(status)) {
        if(tasks.at(status) > 0) {
            return true;
        }
    }
    return false;
}

TasksInfo operator + (TasksInfo& lhs, TasksInfo& rhs) {
    TasksInfo tmp;
    for(size_t i = 0; i < sizeof(TaskStatus); ++i) {
        if(CheckTaskStatus(lhs, static_cast<TaskStatus>(i)) ||
           CheckTaskStatus(rhs, static_cast<TaskStatus>(i))) {
            tmp[static_cast<TaskStatus>(i)] =
            lhs[static_cast<TaskStatus>(i)] +
            rhs[static_cast<TaskStatus>(i)];
        }
    }
    return tmp;
}

class TeamTasks {
    public:
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return personTasks.at(person);
    }
    
    void AddNewTask(const string& person) {
        ++personTasks[person][TaskStatus::NEW];
    }

    tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const string& person, int task_count) {
        TasksInfo updated, remained;
        if(!personTasks.count(person)) {
            return {updated, remained};
        }
        int tasks_num = GetTasksNum(personTasks.at(person));
        if(task_count > tasks_num) {
            task_count = tasks_num;
        }
        remained = personTasks.at(person);

        for(int i = 0; i < task_count; i++) {
            if(CheckTaskStatus(remained, TaskStatus::NEW)) {
                remained.at(TaskStatus::NEW)--;
                updated[TaskStatus::IN_PROGRESS]++;
            } else if(CheckTaskStatus(remained, TaskStatus::IN_PROGRESS)) {
                remained.at(TaskStatus::IN_PROGRESS)--;
                updated[TaskStatus::TESTING]++;
            } else if(CheckTaskStatus(remained, TaskStatus::TESTING)) {
                remained.at(TaskStatus::TESTING)--;
                updated[TaskStatus::DONE]++;
            }
        }
        personTasks.at(person) = updated + remained;
        TasksZeroClean(updated);
        TasksZeroClean(remained);
        remained.erase(TaskStatus::DONE);
        return {updated, remained};
    }

    private:
    int GetTasksNum(TasksInfo& tasks) {
        int tmp = 0;
        for(size_t i = 0; i + 1 < sizeof(TaskStatus); ++i) {
            if(CheckTaskStatus(tasks, static_cast<TaskStatus>(i))) {
                tmp += tasks[static_cast<TaskStatus>(i)];
            }
        }
        return tmp;
    }

    void TasksZeroClean(TasksInfo& tasks) {
        for(size_t i = 0; i < sizeof(TaskStatus); ++i) {
            if(CheckTaskStatus(tasks, static_cast<TaskStatus>(i)) == false) {
                tasks.erase(static_cast<TaskStatus>(i));
            }
        }
    }

    map<string, map<TaskStatus, int>> personTasks;
};

void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
    ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
    ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
    ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}
