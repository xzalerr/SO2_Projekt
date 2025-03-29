#include <chrono>
#include <mutex>
#include <random>
#include <ratio>
#include <thread>
#include <vector>

class DiningPhilosophersProblem
{
public:
    // constructor of DiningPhilosopherProblem object, taking the size of problem as a param
    DiningPhilosophersProblem(int size);
    // function used to start the program, creating all necessary threads 
    void run();
    // function used to print the stats measured, of how long each philosopher has been waiting for chopsticks
    void print_stats();
    
private:
    // defines size of the problem - number of philosophers 
    int size; 
    // mutex used to prevent printing by multiple threads at the same time
    std::mutex cout_mutex;
    // vector of mutexes describing if specific chopstick is used or not
    std::vector<std::mutex> chopsticks;
    // vector of threads describing philosophers' behaviour
    std::vector<std::thread> philosophers;
    // vector for storing waiting time of each philosophers for chopsticks
    std::vector<double> waiting_times;
    // random number generator
    std::mt19937 rd;

    // function simulating thinking for philospher with given index
    void thinking(int index);
    // function simulating eating for philospher with given index
    void eating(int index);
    // function simulating picking up two chopsticks by philosopher when it's possible
    void pick_chops(int index, int first, int second);
    // function simulating putting down two chopsticks by philosopher
    void put_chops(int index, int first, int second);
    // function simulating a philosopher's with specific index behaviour
    void philosopher(int index);

};