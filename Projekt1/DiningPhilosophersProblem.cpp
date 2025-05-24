#include "DiningPhilosophersProblem.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <random>

DiningPhilosophersProblem::DiningPhilosophersProblem(int size) : 
    size(size), chopsticks(size), rd(std::random_device{}()), waiting_times(size, 0)
    {}

void DiningPhilosophersProblem::thinking(int index)
{
    // print the info that philosopher with given index is thinking and disable cout for other threads 
    {
        std::lock_guard<std::mutex> thinking_lock(cout_mutex);
        std::cout << "Philosopher nr " << index << " is thinking...\n";
    }

    std::uniform_int_distribution<int> dist(1000, 2000);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rd)));
}

void DiningPhilosophersProblem::eating(int index)
{
    // print the info that philosopher with given index is eating and disable cout for other threads 
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
    // put down both chopsticks and print info about it
    chopsticks[first].unlock();
    chopsticks[second].unlock();
    {
            std::lock_guard<std::mutex> chopstick_lock(cout_mutex);
            std::cout << "Philosopher nr " << index << " put down chopsticks...\n";
    }
}

void DiningPhilosophersProblem::philosopher(int index)
{
    // calculating left and right chopstick basing on philosopher's index
    int leftChopstick = index % size;
    int rightChopstick = (index + 1) % size;

    // philosopher is thinking, then trying to pick up chopsticks, then eating and then putting down chopsticks 
    // picking up time is measured with chrono and then added to waiting_times vector
    while(true)
    {
        thinking(index);

        auto start = std::chrono::high_resolution_clock::now();
        pick_chops(index, leftChopstick, rightChopstick);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> waiting = end - start;
        waiting_times[index] += waiting.count();

        eating(index);

        put_chops(index, leftChopstick, rightChopstick);
    }
}

void DiningPhilosophersProblem::run()
{
    // creating threads for philosophers and adding to vector and creating thread for printing stats
    for(int i = 0; i < size; i++)
    {
        philosophers.emplace_back(&DiningPhilosophersProblem::philosopher, this, i);
    }

    std::thread stats_thread(&DiningPhilosophersProblem::print_stats, this);

    // synchronizing all threads created with parent thread - main in this case
    for(auto& philosopher : philosophers)
    {
        philosopher.join();
    }

    stats_thread.join();
}

void DiningPhilosophersProblem::print_stats()
{
    // printing stats from waiting_times vector every 5 seconds
    while(true)
    {
        {
            std::lock_guard<std::mutex> printing_lock(cout_mutex);
            std::cout << "Waiting times for each philosopher:\n";
            for(int i = 0; i < waiting_times.size(); i++)
            {
                std::cout << "Philosopher nr " << i << " - " << waiting_times[i] << " s\n";
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}