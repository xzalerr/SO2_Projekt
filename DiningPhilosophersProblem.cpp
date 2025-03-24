#include "DiningPhilosophersProblem.h"

#include <iostream>
#include <random>

DiningPhilosophersProblem::DiningPhilosophersProblem(int size) : 
    size(size), chopsticks(size), rd(std::random_device{}()) {}

void DiningPhilosophersProblem::think(int index)
{
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Philosopher nr " << index << " is thinking...\n";
    }

    std::uniform_int_distribution<int> dist(500, 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
}

void DiningPhilosophersProblem::eat(int index)
{
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Philosopher nr " << index << " is eating...\n";
    }

    std::uniform_int_distribution<int> dist(1000, 2000);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
}

void DiningPhilosophersProblem::philosopher(int index)
{
    int leftChopstick = index % size;
    int rightChopstick = (index + 1) % size;

    while(true)
    {
        think(index);

        // even numbers and odd numbers pick up chopstick in different order, to avoid deadlocks
        if(index % 2 == 0)
        {
            chopsticks[leftChopstick].lock();
            chopsticks[rightChopstick].lock();
        }
        else
        {
            chopsticks[rightChopstick].lock();
            chopsticks[leftChopstick].lock();
        }

        eat(index);

        chopsticks[leftChopstick].unlock();
        chopsticks[rightChopstick].unlock();
    }
}

void DiningPhilosophersProblem::run()
{
    for(int i = 0; i < size; i++)
    {
        philosophers.emplace_back(&DiningPhilosophersProblem::philosopher, this, i);
    }


    for(auto& philosopher : philosophers)
    {
        philosopher.join();
    }
}