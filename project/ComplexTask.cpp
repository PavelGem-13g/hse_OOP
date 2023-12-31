//
// Created by Павел on 15.12.2023.
//

#include <cmath>
#include <memory>

#include "ComplexTask.h"
#include "CollectionResultsTask.h"


ComplexTask::ComplexTask() :Task() {
    splitTasks();
}

ComplexTask::ComplexTask(const std::string &name, int hours, int priority) : Task(name, hours, priority) {

}

ComplexTask::ComplexTask(const ComplexTask &complexTask):Task(complexTask) {
    this->threshold = complexTask.threshold;
    for(auto &task: complexTask.subtasks){
        this->subtasks.push_back(task);
    }
}

std::vector<std::shared_ptr<Task>> ComplexTask::getSubtasks() const{
    return subtasks;
}

TaskType ComplexTask::getType() const {
    return TaskType::TComplexTask;
}

void ComplexTask::assign(std::shared_ptr<Actor> actor) {
    actors.insert(actor);
    int count = 0;
    for(auto& task: subtasks){
        if(task->getProgress()==TaskProgressType::TComplete){
            count+=1;
        }
    }
    if(count==subtasks.size()){
        complete(actor);
    }
}

void ComplexTask::show() const {
    Task::show();
    for (auto &subtask: subtasks) {
        subtask->show();
    }
}

void ComplexTask::addToProject(int threshold) {
    this->threshold = threshold;
    splitTasks();
}


void ComplexTask::work(std::shared_ptr<Actor> actor) {

}

void ComplexTask::complete(std::shared_ptr<Actor> actor) {
    progress = TaskProgressType::TComplete;
    hours -= hours;
}

void ComplexTask::splitTasks() {
    int hours = std::floor(this->hours*0.9);
    int taskNumber = 0;
    auto complexTask = this;
    while (hours > 0){
        std::string name = this->getName()+" - "+std::to_string(taskNumber + 1);
        std::shared_ptr<Task> task;
        if(taskNumber==1){
            task = std::make_shared<Subtask>(name, std::min(threshold, hours), this->getPriority() + 1, complexTask);
        } else{
            task = std::make_shared<Subtask>(name, std::min(threshold, hours), this->getPriority(), complexTask);
        }
        subtasks.push_back(task);
        hours-=std::min(threshold, hours);
        taskNumber+=1;
    }

    std::string name = this->getName()+" - Collecting results";
    auto task = std::make_shared<CollectionResultsTask>(name, static_cast<int>(std::ceil(this->hours * 0.1)),
                                           this->getPriority() - 1, complexTask);
    subtasks.push_back(std::move(task));
}