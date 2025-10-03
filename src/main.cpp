#include <iostream>
#include <vector>
#include <memory>
#include "task.h"
#include "simulator.h"
#include "scheduler.h"

void run_simulation(const std::string &scheduler_name, std::unique_ptr<Scheduler> scheduler, const std::vector<Task> &task_templates)
{
    std::cout << "=================================================\n";
    std::cout << "  RUNNING SIMULATION WITH: " << scheduler_name << "\n";
    std::cout << "=================================================\n";

    Simulator sim(std::move(scheduler), task_templates);
    sim.run(200); // Run for 200 time ticks
    sim.print_results();
}

int main()
{
    // --- Define the task set for our system ---
    // Note: deadline is relative here (same as period), simulator will make it absolute.
    std::vector<Task> task_set = {
        // Level 1: Mission-Critical
        Task(0, "BrakeControl", Criticality::LEVEL_1_MISSION_CRITICAL, 5, 20, 20),
        // Level 2: Important
        Task(0, "GPS_Update", Criticality::LEVEL_2_IMPORTANT, 25, 50, 50),
        // Level 3: Non-Critical - THIS IS THE "STORM"
        Task(0, "PlayMusic", Criticality::LEVEL_3_NON_CRITICAL, 10, 40, 40),
        Task(0, "WeatherApp", Criticality::LEVEL_3_NON_CRITICAL, 8, 45, 45)};

    // Run with standard EDF (will likely fail)
    run_simulation("Classic EDF Scheduler", std::make_unique<EDFSched>(), task_set);

    // Run with our Mixed-Criticality Scheduler (should succeed for critical tasks)
    run_simulation("Mixed-Criticality Scheduler", std::make_unique<MCSched>(), task_set);

    return 0;
}