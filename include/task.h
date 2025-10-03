#pragma once
#include <string>

// Defines the three criticality levels
enum class Criticality
{
    LEVEL_1_MISSION_CRITICAL = 1, // Highest
    LEVEL_2_IMPORTANT = 2,
    LEVEL_3_NON_CRITICAL = 3 // Lowest
};

// Represents a single instance of a task to be executed
struct Task
{
    int id; // Unique identifier for this instance
    std::string name;
    Criticality level;
    int execution_time;               // Original WCET (Worst-Case Execution Time)
    int remaining_time;               // How much time is left to run
    int deadline;                     // Absolute deadline (time tick)
    int period;                       // How often the task is generated
    bool has_missed_deadline = false; // When a task misses its deadline, we'll set the flag

    // Constructor for creating new task instances
    Task(int p_id, std::string p_name, Criticality p_level, int exec_time, int p_deadline, int p_period)
        : id(p_id), name(p_name), level(p_level), execution_time(exec_time),
          remaining_time(exec_time), deadline(p_deadline), period(p_period) {}
};