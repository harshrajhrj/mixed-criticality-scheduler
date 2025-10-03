#pragma once
#include <memory>
#include "task.h"

class CPU
{
public:
    // Constructor
    CPU();

    // Assigns a task to the CPU. This could be a new task or a preempted one.
    void assign_task(std::shared_ptr<Task> task);

    // Executes the current task for one time tick.
    void execute_tick();

    // Returns a pointer to the currently running task.
    std::shared_ptr<Task> get_current_task() const;

    // Checks if the CPU is currently idle (no task assigned).
    bool is_idle() const;

private:
    std::shared_ptr<Task> current_task;
};