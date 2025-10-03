#include "cpu.h"
#include <iostream>

CPU::CPU() : current_task(nullptr) {}

void CPU::assign_task(std::shared_ptr<Task> task)
{
    current_task = task;
}

void CPU::execute_tick()
{
    if (current_task != nullptr && current_task->remaining_time > 0)
    {
        current_task->remaining_time--;
    }
}

std::shared_ptr<Task> CPU::get_current_task() const
{
    return current_task;
}

bool CPU::is_idle() const
{
    return current_task == nullptr;
}