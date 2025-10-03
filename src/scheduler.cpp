#include "scheduler.h"

// --- EDF Scheduler ---
void EDFSched::add_task(std::shared_ptr<Task> task)
{
    ready_queue.push(task);
}

std::shared_ptr<Task> EDFSched::get_next_task()
{
    if (ready_queue.empty())
    {
        return nullptr;
    }
    std::shared_ptr<Task> next_task = ready_queue.top();
    ready_queue.pop();
    return next_task;
}

bool EDFSched::has_tasks() const
{
    return !ready_queue.empty();
}

// --- Mixed-Criticality Scheduler ---
void MCSched::add_task(std::shared_ptr<Task> task)
{
    ready_queue.push(task);
}

std::shared_ptr<Task> MCSched::get_next_task()
{
    if (ready_queue.empty())
    {
        return nullptr;
    }
    std::shared_ptr<Task> next_task = ready_queue.top();
    ready_queue.pop();
    return next_task;
}

bool MCSched::has_tasks() const
{
    return !ready_queue.empty();
}