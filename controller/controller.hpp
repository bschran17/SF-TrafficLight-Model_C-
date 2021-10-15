#include <string>
#include <string.h>
#include <vector>

class TrafficLight
{
public:
  TrafficLight(std::string, std::string, std::string, int, std::string);
  std::string getColor();
  std::string getCNN();
  std::string getStreetName();
  std::string getPoint();
  int getCycleLength();
  void setColor(std::string);

private:
  std::string color;
  std::string CNN;
  std::string streetName;
  std::string point;
  int cycleLength;
};

class Event;

class TrafficController
{
public:
  TrafficController(int, std::vector<int>, std::string);
  int getNumStreets();
  std::vector<TrafficLight> performEvent(Event, std::vector<TrafficLight>);
  Event initializeEvent(int);
  Event calculateNextEvent(Event, std::vector<TrafficLight>);
  std::string CNN;
  std::vector<int> trafficLightIndices;

private:
  int numStreets;
};

class Event
{
public:
  Event(int, int, std::string, int);
  int trafficControllerIndex;
  int streetIndex;
  std::string color;
  int time;

private:
};