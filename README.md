# Dining Philosophers Problem
## Brief Problem Description
The **Dining Philosophers Problem** is a classic **synchronization problem**, which requires a concurrent algorithm design to resolve. In this issue philosophers sit at the table, spending their time either thinking or eating.

The problem is that there are as many chopsticks as philosophers. However each philosopher needs two chopstick to eat. In order to do that, the chopsticks are shared between adjacent philosophers, as a single chopstick might be used only by one philosopher by the time. Also when the philosopher is eating, he cannot be interrupted, until he's done. Another problem is the fact, that philosopher can't pick up two chopsticks simultaneously.

This is a typical computer science and operating system problem, when resources such as for example CPU, memory or I/O devices needs to be shared between processes. The Dinig Philosophers Problem also needs to deal with similar problem as typical operationg system, which is **deadlock**. Deadlock is a situation where all resources are allocated, so each process holds one resource while waiting for another. 

In this case it would happen for example if every philosopher would pick up their left chopstick, then no one could pick up their right chopstick. 

To prevent such situations in this code, the algorithm is projected in such way, that philosophers with even index number pick up their left chopstick first and philosophers with odd index number pick up their right chopstick first. This ensures no circular wait, as adjacent philosophers decide to choose different chopsticks.

## Threads & Their Roles
### Each philosopher is represented by a single thread
- `std::thread philosopher(i)` represents a single philosopher with index `i`, each of them is stored in `std::vector<std::thread> philosophers`
- thread continuously iterates in a loop performing **four actions**:
  - **thinking** 
    - for a random time between 1 and 2 seconds
  - **picking up chopsticks**
    - while chopsticks are represented as `std::vector<std::mutex> chopsticks` that can be either unlocked or locked, so philospher might need to wait for them
  - **eating** 
    - for a random time between 1 and 2 seconds
  - **puting down chopsticks**
    - unlocking both chopsticks
### Additional thread is used for printing the staticstics of how long each philosopher has been waiting for chopsticks
- during `std::thread philosopher(i)` time is measured for each philosopher before the start of picking up chopsticks and after their are picked up, the results are added to `std::vector<double> waiting_times`
- every 5 seconds the data collected in `std::vector<double> waiting_times` is printed, to show the results

## Critical Sections & Solutions
### Critical Sections in this program:
1. **Shared chopsticks**:
   - Philosopher needs to pick up two chopsticks before he can start eating
2. **Shared console output**:
   - Information about the current state of each philosopher and the stats of program are continuously printed
### Solutions of  problems
1. Each chopstick is represented as a single mutex in `std::vector<std::mutex> chopsticks`