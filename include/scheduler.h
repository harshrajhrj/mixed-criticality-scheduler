#pragma once
#include <vector>
#include <queue>
#include <memory>
#include "task.h"

// Base class for schedulers (Interface)
class Scheduler
{
public:
    virtual ~Scheduler() = default;
    virtual void add_task(std::shared_ptr<Task> task) = 0;
    virtual std::shared_ptr<Task> get_next_task() = 0;
    virtual bool has_tasks() const = 0;
};

// Step 2: Classic Earliest Deadline First (EDF) Scheduler
class EDFSched : public Scheduler
{
public:
    void add_task(std::shared_ptr<Task> task) override;
    std::shared_ptr<Task> get_next_task() override;
    bool has_tasks() const override;

private:
    // Custom comparator for the priority queue: smallest deadline first
    struct CompareDeadline
    {
        bool operator()(const std::shared_ptr<Task> &a, const std::shared_ptr<Task> &b) const
        {
            return a->deadline > b->deadline;
        }
    };
    std::priority_queue<std::shared_ptr<Task>, std::vector<std::shared_ptr<Task>>, CompareDeadline> ready_queue;
};

// Step 4: Mixed-Criticality Aware Scheduler
class MCSched : public Scheduler
{
public:
    void add_task(std::shared_ptr<Task> task) override;
    std::shared_ptr<Task> get_next_task() override;
    bool has_tasks() const override;

private:
    // Custom comparator: 1. By criticality level, 2. By deadline
    struct CompareMixedCriticality
    {
        bool operator()(const std::shared_ptr<Task> &a, const std::shared_ptr<Task> &b) const
        {
            if (a->level != b->level)
            {
                return static_cast<int>(a->level) > static_cast<int>(b->level);
            }
            return a->deadline > b->deadline;
        }
    };
    std::priority_queue<std::shared_ptr<Task>, std::vector<std::shared_ptr<Task>>, CompareMixedCriticality> ready_queue;
};