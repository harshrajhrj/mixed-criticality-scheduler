# Real-Time Mixed-Criticality Scheduler Simulation
*An Operating System Scheduler that Thinks Like an ER Doctor*

This project is a C++ simulation of a real-time, mixed-criticality operating system scheduler. It demonstrates why standard "fair" schedulers are unsuitable for safety-critical systems (like cars or robotics) and implements a criticality-aware scheduler that can guarantee the deadlines of the most important tasks, even under heavy system load.

---

## The Big Idea: An Emergency Room Analogy

Imagine a hospital's emergency room. Three patients arrive at once:
1.  **Mission-Critical:** A person with a life-threatening injury.
2.  **Important:** A person with a broken arm.
3.  **Non-Critical:** A person with a minor scrape.

A "fair" scheduler, like the one in your laptop, might give each patient 10 minutes of a doctor's time in a round-robin fashion. This would be a disaster! The patient with the critical injury needs immediate and uninterrupted attention.

The ER doctor acts as a **Real-Time, Mixed-Criticality Scheduler**. They instantly assess the urgency (criticality) of each patient and prioritize ruthlessly. This project designs a software scheduler that thinks and acts like that ER doctor.



---

## The Problem
Standard operating systems are designed for **fairness** and **throughput**. In a real-time system, like the computer controlling a car's airbags, fairness is irrelevant. The only thing that matters is that critical tasks are completed before their strict deadlines.

* **Missing a video game deadline:** Annoying.
* **Missing an airbag deployment deadline:** Catastrophic.

A **mixed-criticality system** is one where tasks of different importance levels run on the same processor. The challenge is to design a scheduler that can absolutely guarantee the deadlines of the most critical tasks, no matter what else is happening.

---

## The Solution
This simulation implements and compares two schedulers under a high-load scenario:

1.  **Classic Earliest Deadline First (EDF):** A standard real-time algorithm that only considers a task's deadline.
2.  **Mixed-Criticality Scheduler (MCS):** A custom scheduler that implements a simple but powerful policy: **"Always run the ready task with the highest criticality level. If criticality levels are equal, run the one with the earliest deadline."**

This MCS scheduler will preempt (interrupt) low-criticality tasks to service high-criticality ones and will let non-essential tasks miss their deadlines if necessary to ensure the safety and stability of the overall system.

---

## How to Build and Run

This project uses C++17 and CMake.

### Prerequisites
* A C++17 compliant compiler (like g++)
* CMake (version 3.10 or higher)
* Make

### Steps
```bash
# 1. Clone the repository
git clone <your-repository-url>
cd mixed-criticality-scheduler

# 2. Create a build directory
mkdir build
cd build

# 3. Configure the project with CMake
cmake ..

# 4. Compile the project
make

# 5. Run the simulation
./main
```
The program will run both simulations and print the final reports to your console.

---

## Simulation Results

The simulation was run with a high-load task set designed to cause failures in a standard scheduler. The results clearly demonstrate the superiority of the Mixed-Criticality approach.

```bash
=================================================
  RUNNING SIMULATION WITH: Classic EDF Scheduler
=================================================
--- FINAL REPORT ---
Total Ticks: 200
CPU Utilization: 100.00%

--- DEADLINE ANALYSIS ---
LEVEL 1:
  - Generated: 10
  - Completed: 10
  - MISSED: 0
LEVEL 2:
  - Generated: 4
  - Completed: 0
  - MISSED: 3  <-- Correctly shows failure now!
LEVEL 3:
  - Generated: 10
  - Completed: 8
  - MISSED: 1
=================================================
  RUNNING SIMULATION WITH: Mixed-Criticality Scheduler
=================================================
--- FINAL REPORT ---
Total Ticks: 200
CPU Utilization: 100.00%

--- DEADLINE ANALYSIS ---
LEVEL 1:
  - Generated: 10
  - Completed: 10
  - MISSED: 0  <-- Mission-critical tasks are safe!
LEVEL 2:
  - Generated: 4
  - Completed: 4
  - MISSED: 0
LEVEL 3:
  - Generated: 10
  - Completed: 3
  - MISSED: 6  <-- Non-critical tasks are sacrificed, as intended.
```

| Feature / Analysis Point | Classic EDF Scheduler | Mixed-Criticality Scheduler |
| :--- | :--- | :--- |
| **Scheduling Policy** | Prioritizes the task with the **earliest deadline**, regardless of its importance. | Prioritizes by **criticality level first**, then by earliest deadline. |
| **Behavior Under Overload** | Fails unpredictably. Low-priority tasks can block and cause higher-priority tasks to miss deadlines (priority inversion). | Acts predictably. Deliberately **sacrifices (sheds) low-priority tasks** to guarantee resources for high-priority tasks. |
| **Key Log Event Example** | `T=50`: `GPS_Update` (Level 2) misses its deadline because the CPU was occupied by other tasks with earlier deadlines. | `T=40`: `PlayMusic` (Level 3) is sacrificed to run `BrakeControl` (Level 1), even though the music task had an earlier deadline. |
| **Outcome: Level 1 (Critical)** | Succeeded by chance (**0 Misses**), but offers **no guarantee** of safety. | **Guaranteed Success (0 Misses)**. All critical deadlines were met. |
| **Outcome: Level 2 (Important)** | **Failure (3 Misses)**. Important tasks were dropped due to the overload. | **Guaranteed Success (0 Misses)**. All important deadlines were met. |
| **Outcome: Level 3 (Non-Critical)** | Some tasks failed due to general system overload (**1 Miss**). | Intentionally sacrificed to ensure higher-level success (**6 Misses**). This is the desired behavior. |
| **System Safety & Predictability**| **Unsafe & Unpredictable**. Cannot be trusted for safety-critical applications. | **Safe & Predictable**. Provides the reliability needed for critical systems. |
| **ER Analogy** | A "fair" doctor who treats a patient with a paper cut before a patient with a heart attack because they arrived 30 seconds earlier. | A smart doctor who immediately triages and treats the heart attack patient, even if it means the paper cut patient has to wait. |