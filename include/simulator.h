#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "task.h"
#include "scheduler.h"
#include "cpu.h"

class Simulator
{
public:
    // Constructor: Initializes the simulator with a specific scheduler and set of tasks.
    Simulator(std::unique_ptr<Scheduler> sched, const std::vector<Task> &templates);

    // Runs the simulation for a given duration in time ticks.
    void run(int duration);

    // Prints a final report of the simulation results.
    void print_results() const;

private:
    // Advances the simulation by a single time tick.
    void advance_time_tick();

    // Checks the task templates and creates new task instances if they are due.
    void generate_new_tasks();

    // Checks for and records any missed deadlines among active tasks.
    void check_for_missed_deadlines();

    int system_time;
    int simulation_duration;
    std::unique_ptr<Scheduler> scheduler;
    CPU cpu;
    std::vector<Task> task_templates;
    int next_task_id;

    // A list of all tasks currently in the system (waiting, running)
    std::vector<std::shared_ptr<Task>> all_tasks_in_system;

    // Metrics
    std::map<Criticality, int> generated_tasks;
    std::map<Criticality, int> completed_tasks;
    std::map<Criticality, int> missed_deadlines;
    int cpu_busy_ticks;
};