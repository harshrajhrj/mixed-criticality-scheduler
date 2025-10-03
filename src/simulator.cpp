#include "simulator.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Simulator::Simulator(std::unique_ptr<Scheduler> sched, const std::vector<Task> &templates)
    : system_time(0),
      simulation_duration(0),
      scheduler(std::move(sched)),
      task_templates(templates),
      next_task_id(1),
      cpu_busy_ticks(0)
{
    // Initialize metrics maps
    for (int i = 1; i <= 3; ++i)
    {
        Criticality level = static_cast<Criticality>(i);
        generated_tasks[level] = 0;
        completed_tasks[level] = 0;
        missed_deadlines[level] = 0;
    }
}

void Simulator::run(int duration)
{
    this->simulation_duration = duration;
    std::cout << "Starting simulation for " << duration << " ticks.\n";
    for (int t = 0; t < duration; ++t)
    {
        advance_time_tick();
    }
    std::cout << "Simulation Finished!\n";
}

void Simulator::advance_time_tick()
{
    // 1. Generate new tasks that are due at the current system time
    generate_new_tasks();

    // 2. Check for any tasks that missed their deadline at the start of this tick
    check_for_missed_deadlines();

    // 3. Handle the previously running task
    std::shared_ptr<Task> last_task = cpu.get_current_task();
    if (last_task != nullptr)
    {
        if (last_task->remaining_time == 0)
        {
            // Task finished execution
            std::cout << "[T=" << std::setw(3) << system_time << "] COMPLETED: " << last_task->name << " (ID: " << last_task->id << ")\n";
            completed_tasks[last_task->level]++;
            // Remove from system
            all_tasks_in_system.erase(std::remove(all_tasks_in_system.begin(), all_tasks_in_system.end(), last_task), all_tasks_in_system.end());
        }
        else
        {
            // Task was preempted, add it back to the ready queue
            scheduler->add_task(last_task);
        }
    }

    // 4. Get the next highest priority task from the scheduler
    // ===========================CHANGES===========================
    // std::shared_ptr<Task> next_task_to_run = scheduler->get_next_task();
    std::shared_ptr<Task> next_task_to_run = nullptr;
    do
    {
        next_task_to_run = scheduler->get_next_task();
    } while (next_task_to_run != nullptr && next_task_to_run->has_missed_deadline);
    // ===========================CHANGES===========================

    // 5. Assign the new task to the CPU and execute
    cpu.assign_task(next_task_to_run);
    cpu.execute_tick();

    // 6. Log and update metrics
    if (!cpu.is_idle())
    {
        cpu_busy_ticks++;
        if (last_task == nullptr || (last_task != nullptr && last_task->id != cpu.get_current_task()->id))
        {
            std::cout << "[T=" << std::setw(3) << system_time << "] RUNNING:   " << cpu.get_current_task()->name << " (ID: " << cpu.get_current_task()->id << ")\n";
        }
    }
    else
    {
        std::cout << "[T=" << std::setw(3) << system_time << "] IDLE\n";
    }

    // 7. Increment system time
    system_time++;
}

void Simulator::generate_new_tasks()
{
    for (const auto &tmpl : task_templates)
    {
        if (system_time % tmpl.period == 0)
        {
            int deadline = system_time + tmpl.deadline;
            Task new_task(next_task_id++, tmpl.name, tmpl.level, tmpl.execution_time, deadline, tmpl.period);

            auto task_ptr = std::make_shared<Task>(new_task);
            scheduler->add_task(task_ptr);
            all_tasks_in_system.push_back(task_ptr);
            generated_tasks[tmpl.level]++;

            std::cout << "[T=" << std::setw(3) << system_time << "] GENERATED: " << new_task.name << " (ID: " << new_task.id << ") - Deadline: " << deadline << "\n";
        }
    }
}

void Simulator::check_for_missed_deadlines()
{
    auto it = all_tasks_in_system.begin();
    while (it != all_tasks_in_system.end())
    {
        if ((*it)->deadline <= system_time && (*it)->remaining_time > 0)
        {
            std::cout << "[T=" << std::setw(3) << system_time << "] MISSED DEADLINE: " << (*it)->name << " (ID: " << (*it)->id << ") !!!\n";
            missed_deadlines[(*it)->level]++;
            (*it)->has_missed_deadline = true;
            it = all_tasks_in_system.erase(it); // Remove from system
        }
        else
        {
            ++it;
        }
    }
}

void Simulator::print_results() const
{
    std::cout << "\n--- FINAL REPORT ---\n";
    std::cout << "Total Ticks: " << simulation_duration << "\n";
    double utilization = (simulation_duration > 0) ? (static_cast<double>(cpu_busy_ticks) / simulation_duration) * 100.0 : 0.0;
    std::cout << "CPU Utilization: " << std::fixed << std::setprecision(2) << utilization << "%\n\n";

    std::cout << "--- DEADLINE ANALYSIS ---\n";
    for (int i = 1; i <= 3; ++i)
    {
        Criticality level = static_cast<Criticality>(i);
        std::cout << "LEVEL " << i << ":\n";
        std::cout << "  - Generated: " << generated_tasks.at(level) << "\n";
        std::cout << "  - Completed: " << completed_tasks.at(level) << "\n";
        std::cout << "  - MISSED: " << missed_deadlines.at(level) << "\n";
    }
}