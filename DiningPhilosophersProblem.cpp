#include "DiningPhilosophersProblem.h"

#include <chrono>
#include <codecvt>
#include <functional>
#include <iostream>
#include <mutex>
#include <random>

DiningPhilosophersProblem::DiningPhilosophersProblem(int size) : 
    size(size), chopsticks(size), rd(std::random_device{}()), waiting_times(size, 0)
    {}

void DiningPhilosophersProblem::thinking(int index)
{
    {
        std::lock_guard<std::mutex> thinking_lock(cout_mutex);
        std::cout << "Philosopher nr " << index << " is thinking...\n";
    }

    std::uniform_int_distribution<int> dist(1000, 2000);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
}

void DiningPhilosophersProblem::eating(int index)
{
    {
        std::lock_guard<std::mutex> eating_lock(cout_mutex);
        std::cout << "Philosopher nr " << index << " is eating...\n";
    }

    std::uniform_int_distribution<int> dist(1000, 2000);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
}

void DiningPhilosophersProblem::pick_chops(int index, int first, int second)
{
    // philosophers with odd and even indexes pick up chopsticks in different order to avoid deadlocks
    if(index % 2 == 0)
    {
        chopsticks[first].lock();
        {
            std::lock_guard<std::mutex> chopstick_lock(cout_mutex);
            std::cout << "Philosopher nr " << index << " picked up first chopstick...\n";
        }
        chopsticks[second].lock();
        {
            std::lock_guard<std::mutex> chopstick_lock(cout_mutex);
            std::cout << "Philosopher nr " << index << " picked up second chopstick...\n";   
        }
    }
    else 
    {
        chopsticks[second].lock();
        {
            std::lock_guard<std::mutex> chopstick_lock(cout_mutex);
            std::cout << "Philosopher nr " << index << " picked up first chopstick...\n";
        }
        chopsticks[first].lock();
        {
            std::lock_guard<std::mutex> chopstick_lock(cout_mutex);
            std::cout << "Philosopher nr " << index << " picked up second chopstick...\n";   
        }
    }
}

void DiningPhilosophersProblem::put_chops(int index, int first, int second)
{
    chopsticks[first].unlock();
    chopsticks[second].unlock();
    {
            std::lock_guard<std::mutex> chopstick_lock(cout_mutex);
            std::cout << "Philosopher nr " << index << " put down chopsticks...\n";
    }
}

void DiningPhilosophersProblem::philosopher(int index)
{
    int leftChopstick = index % size;
    int rightChopstick = (index + 1) % size;

    while(running)
    {
        thinking(index);

        auto start = std::chrono::high_resolution_clock::now();
        pick_chops(index, leftChopstick, rightChopstick);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> waiting = end - start;
        waiting_times[index] += waiting.count();
        print_stats();

        eating(index);

        put_chops(index, leftChopstick, rightChopstick);
    }
}

void DiningPhilosophersProblem::run()
{
    running = true;
    for(int i = 0; i < size; i++)
    {
        philosophers.emplace_back(&DiningPhilosophersProblem::philosopher, this, i);
    }


    for(auto& philosopher : philosophers)
    {
        philosopher.join();
    }
}

void DiningPhilosophersProblem::stop()
{
    running = false;
}

void DiningPhilosophersProblem::rerun()
{
    running = false;
}

void DiningPhilosophersProblem::print_stats()
{
    std::lock_guard<std::mutex> printing_lock(cout_mutex);
    std::cout << "Waiting times for each philosopher:\n";
    for(int i = 0; i < waiting_times.size(); i++)
    {
        std::cout << "Philosopher nr " << i << " - " << waiting_times[i] << " s\n";
    }
}