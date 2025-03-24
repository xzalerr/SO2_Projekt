#include <mutex>
#include <random>
#include <thread>
#include <vector>

class DiningPhilosophersProblem
{
public:
    DiningPhilosophersProblem(int size);
    void run();
    
private:
    int size;
    std::mutex cout_mutex;
    std::vector<std::mutex> chopsticks;
    std::vector<std::thread> philosophers;
    std::mt19937 rd;

    void think(int index);
    void eat(int index);
    void philosopher(int index);

};