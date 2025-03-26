#include <chrono>
#include <mutex>
#include <random>
#include <ratio>
#include <thread>
#include <vector>

class DiningPhilosophersProblem
{
public:
    DiningPhilosophersProblem(int size);
    void run();
    void stop();
    void rerun();
    void print_stats();
    
private:
    int size;
    bool running;
    std::mutex cout_mutex;
    std::vector<std::mutex> chopsticks;
    std::vector<std::thread> philosophers;
    std::vector<double> waiting_times;
    std::mt19937 rd;

    void thinking(int index);
    void eating(int index);
    void pick_chops(int index, int first, int second);
    void put_chops(int index, int first, int second);
    void philosopher(int index);

};