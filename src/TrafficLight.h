#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <random>
#include <chrono>

#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum class TrafficLightPhase {
    red,
    green,
};


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

template <class T>
class MessageQueue
{
public:
    T receive();
    void send(T &&msg);

private:
    std::mutex _mutex;
    std::condition_variable _condition;
    std::deque<T> _queue;
};

// A simple helper class to get random numbers intervals
template <typename T>
class Random
{
public:
    Random():
      _eng{static_cast<unsigned long>(std::chrono::high_resolution_clock::now().time_since_epoch().count())}
    {

    }

    T generateBetween(T min, T max)
    {
      return std::uniform_int_distribution<T>(min,max)(_eng);
    }

private:
  std::mt19937 _eng;
};

class TrafficLight: public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    virtual ~TrafficLight () = default;

    // getters / setters
    TrafficLightPhase getCurrentPhase();


    // typical behaviour methods
    void waitForGreen();
    void simulate() override;

private:
    // typical behaviour methods
    void cycleThroughPhases();

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling
    // send in conjunction with move semantics.

    MessageQueue<TrafficLightPhase> _queue;
    TrafficLightPhase _currentPhase;
    Random<int> _randomNumber;
};

#endif