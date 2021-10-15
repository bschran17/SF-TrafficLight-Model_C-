#include "controller.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <queue>

TrafficLight::TrafficLight(std::string CNN1, std::string color1, std::string streetName1, int cycleLength1, std::string point1)
{
  CNN = CNN1;
  color = color1;
  streetName = streetName1;
  cycleLength = cycleLength1;
  point = point1;
}

std::string TrafficLight::getCNN()
{
  return CNN;
}

std::string TrafficLight::getColor()
{
  return color;
}

std::string TrafficLight::getStreetName()
{
  return streetName;
}

std::string TrafficLight::getPoint()
{
  return point;
}

int TrafficLight::getCycleLength()
{
  return cycleLength;
}

void TrafficLight::setColor(std::string color1)
{
  color = color1;
}

TrafficController::TrafficController(int numStreets1, std::vector<int> trafficLightIndices1, std::string CNN1)
{
  numStreets = numStreets1;
  trafficLightIndices = trafficLightIndices1;
  CNN = CNN1;
}

int TrafficController::getNumStreets()
{
  return numStreets;
}

std::vector<TrafficLight> TrafficController::performEvent(Event e, std::vector<TrafficLight> trafficLights)
{
  TrafficLight tl = trafficLights.at(trafficLightIndices.at(e.streetIndex));
  tl.setColor(e.color);
  int index = trafficLightIndices.at(e.streetIndex);
  trafficLights.insert(trafficLights.begin() + index, tl);
  trafficLights.erase(trafficLights.begin() + index + 1);
  return trafficLights;
}

Event TrafficController::initializeEvent(int index)
{
  return Event(index, 0, "GREEN", 0);
}

Event TrafficController::calculateNextEvent(Event e, std::vector<TrafficLight> trafficLights)
{
  if (e.color == "RED")
  {
    return Event(e.trafficControllerIndex, (e.streetIndex + 1) % (numStreets - 1), "GREEN", e.time);
  }
  else if (e.color == "GREEN")
  {
    return Event(e.trafficControllerIndex, e.streetIndex, "YELLOW", e.time + trafficLights.at(trafficLightIndices.at(e.streetIndex)).getCycleLength());
  }
  else
  {
    return Event(e.trafficControllerIndex, e.streetIndex, "RED", e.time + 10);
  }
}

Event::Event(int trafficControllerIndex1, int streetIndex1, std::string color1, int time1)
{
  trafficControllerIndex = trafficControllerIndex1;
  streetIndex = streetIndex1;
  color = color1;
  time = time1;
}

std::vector<TrafficLight> createTrafficLights(std::vector<std::vector<std::string>> parsedCSV)
{
  std::vector<TrafficLight> TrafficLights;
  int k = 0;
  for (std::vector<std::string> row : parsedCSV)
  {
    int index = 1;
    std::string CNN1 = row.at(0);
    std::string color1 = "RED";
    std::string streetName1 = row.at(index);
    int cycleLength1;
    while (streetName1 != "NONE" && streetName1 != "GPS")
    {
      if (k % 25 == 0)
      {
        cycleLength1 = 90;
      }
      else if (k % 25 < 15)
      {
        cycleLength1 = 60;
      }
      else
      {
        cycleLength1 = 30;
      }
      TrafficLight tl = TrafficLight(CNN1, color1, streetName1, cycleLength1, row.at(6));
      TrafficLights.push_back(tl);
      k = k + 1;
      index = index + 1;
      streetName1 = row.at(index);
    }
  }
  return TrafficLights;
}

std::vector<TrafficController> createTrafficControllers(std::vector<std::vector<std::string>> parsedCSV)
{
  int index = 0;
  std::vector<TrafficController> trafficControllers;
  for (std::vector<std::string> row : parsedCSV)
  {
    int k = 2;
    std::vector<int> trafficLights1;
    trafficLights1.push_back(index);
    trafficLights1.push_back(index + 1);
    index = index + 2;
    if (row.at(3) != "NONE")
    {
      trafficLights1.push_back(index);
      index = index + 1;
      k = k + 1;
      if (row.at(4) != "NONE")
      {
        trafficLights1.push_back(index);
        index = index + 1;
        k = k + 1;
      }
    }
    TrafficController tc = TrafficController(k, trafficLights1, row.at(0));
    trafficControllers.push_back(tc);
  }
  return trafficControllers;
}

std::vector<std::string> getCSVLines()
{
  std::vector<std::string> vec;
  std::ifstream myFile;
  myFile.open("Traffic_Signals_SF.csv");

  while (myFile.good())
  {
    std::string line;
    getline(myFile, line, '\n');
    vec.push_back(line);
  }
  return vec;
}

std::vector<std::string> parseCSVLine(std::string line)
{
  std::string delimiter = ",";
  std::vector<std::string> vec;

  size_t pos = 0;
  std::string token;
  int index = 0;
  while ((pos = line.find(delimiter)) != std::string::npos)
  {
    token = line.substr(0, pos);
    if (index == 0 || index == 3 || index == 4 || index == 6 || index == 8 || index == 12 || index == 34)
    {
      if (token == "")
      {
        vec.push_back("NONE");
      }
      else
      {
        vec.push_back(token);
      }
    }
    line.erase(0, pos + delimiter.length());
    index = index + 1;
  }
  vec.push_back(line);
  return vec;
}

std::vector<std::vector<std::string>> parseCSV()
{
  std::vector<std::vector<std::string>> parsedCSV;
  std::vector<std::string> csvLines = getCSVLines();
  for (std::string line : csvLines)
  {
    std::vector<std::string> words = parseCSVLine(line);
    if (words.at(5) == "GPS")
    {
      parsedCSV.push_back(words);
    }
  }
  return parsedCSV;
}

class Compare
{
public:
  bool operator()(Event e1, Event e2)
  {
    return e1.time > e2.time;
  }
};

std::priority_queue<Event, std::vector<Event>, Compare> createPriorityQueue(std::vector<TrafficController> trafficControllers)
{
  std::priority_queue<Event, std::vector<Event>, Compare> pq;
  int x = 0;
  for (TrafficController tc : trafficControllers)
  {
    Event e = tc.initializeEvent(x);
    pq.push(e);
    x = x + 1;
  }
  return pq;
}

void writeCSV(std::vector<TrafficLight> trafficLights)
{
  std::ofstream myfile("myfile.csv");
  myfile << "CNN,StreetName,Color" << std::endl;
  for (TrafficLight tl : trafficLights)
  {
    std::string CNN = tl.getCNN();
    std::string streetName = tl.getStreetName();
    std::string color = tl.getColor();
    myfile << CNN << "," << streetName << "," << color << std::endl;
  }
}

std::vector<std::string> createFileNames()
{
  std::vector<std::string> fileNames;
  std::string fileName = "";
  std::string w = "";
  std::string x = "";
  std::string y = "";
  std::string z = "";
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      switch (i)
      {
      case 0:
        w = "r";
        break;
      case 1:
        w = "g";
        break;
      case 2:
        w = "y";
        break;
      }
      switch (j)
      {
      case 0:
        x = "r";
        break;
      case 1:
        x = "g";
        break;
      case 2:
        x = "y";
        break;
      }
      fileName = "i2" + w + x + ".png";
      fileNames.push_back(fileName);
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          y = "r";
          break;
        case 1:
          y = "g";
          break;
        case 2:
          y = "y";
          break;
        }
        fileName = "i3" + w + x + y + ".png";
        fileNames.push_back(fileName);
        for (int l = 0; l < 3; l++)
        {
          switch (l)
          {
          case 0:
            z = "r";
            break;
          case 1:
            z = "g";
            break;
          case 2:
            z = "y";
            break;
          }
          fileName = "i4" + w + x + y + z + ".png";
          fileNames.push_back(fileName);
        }
      }
    }
  }
  return fileNames;
}

void createkml(std::vector<std::string> fileNames, std::vector<TrafficController> trafficControllers, std::vector<TrafficLight> trafficLights)
{
  std::ofstream myfile("myfile.kml");
  myfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  myfile << "<kml xmlns=\"http://www.opengis.net/kml/2.2\">" << std::endl;
  myfile << "<Document>" << std::endl;
  for (std::string fileName : fileNames)
  {
    std::string woPNG = fileName.substr(0, fileName.length() - 4);
    myfile << "  <Style id=\"" << woPNG << "\">" << std::endl;
    myfile << "    <IconStyle id=\"" << woPNG << "\"" << std::endl;
    myfile << "      <Icon>" << std::endl;
    myfile << "        <href>files/" << fileName << "</href>" << std::endl;
    myfile << "        <scale>1.0</scale>" << std::endl;
    myfile << "      </Icon>" << std::endl;
    myfile << "    </IconStyle>" << std::endl;
    myfile << "  </Style>" << std::endl;
  }
  for (TrafficController tc : trafficControllers)
  {
    int numStreets = tc.getNumStreets();
    if (numStreets == 2)
    {
      std::string CNN = tc.CNN;
      std::string color1 = trafficLights.at(tc.trafficLightIndices.at(0)).getColor();
      std::string color2 = trafficLights.at(tc.trafficLightIndices.at(1)).getColor();
      std::string streetName1 = trafficLights.at(tc.trafficLightIndices.at(0)).getStreetName();
      std::string streetName2 = trafficLights.at(tc.trafficLightIndices.at(1)).getStreetName();
      std::string coordinates = trafficLights.at(tc.trafficLightIndices.at(0)).getPoint();
      coordinates = coordinates.substr(7, coordinates.length() - 8);
      int loc = coordinates.find(" ");
      std::string point1 = coordinates.substr(0, loc);
      std::string point2 = coordinates.substr(loc + 1, coordinates.length());
      if (color1 == "RED")
      {
        color1 = "r";
      }
      else if (color1 == "GREEN")
      {
        color1 = "g";
      }
      else
      {
        color1 = "y";
      }
      if (color2 == "RED")
      {
        color2 = "r";
      }
      else if (color2 == "GREEN")
      {
        color2 = "g";
      }
      else
      {
        color2 = "y";
      }
      myfile << "  <Placemark>" << std::endl;
      myfile << "    <name>" << CNN << "</name>" << std::endl;
      myfile << "    <description>" << streetName1 << " and " << streetName2 << "/description>" << std::endl;
      myfile << "    <styleUrl>#i2" << color1 << color2 << "</styleUrl>" << std::endl;
      myfile << "    <Point>" << std::endl;
      myfile << "      <coordinates>" << point1 << "," << point2 << "</coordinates>" << std::endl;
      myfile << "    </Point>" << std::endl;
      myfile << "  </Placemark>" << std::endl;
    }
    else if (numStreets == 3)
    {
      std::string CNN = tc.CNN;
      std::string color1 = trafficLights.at(tc.trafficLightIndices.at(0)).getColor();
      std::string color2 = trafficLights.at(tc.trafficLightIndices.at(1)).getColor();
      std::string color3 = trafficLights.at(tc.trafficLightIndices.at(2)).getColor();
      std::string streetName1 = trafficLights.at(tc.trafficLightIndices.at(0)).getStreetName();
      std::string streetName2 = trafficLights.at(tc.trafficLightIndices.at(1)).getStreetName();
      std::string streetName3 = trafficLights.at(tc.trafficLightIndices.at(2)).getStreetName();
      std::string coordinates = trafficLights.at(tc.trafficLightIndices.at(0)).getPoint();
      coordinates = coordinates.substr(7, coordinates.length() - 8);
      int loc = coordinates.find(" ");
      std::string point1 = coordinates.substr(0, loc);
      std::string point2 = coordinates.substr(loc + 1, coordinates.length());
      if (color1 == "RED")
      {
        color1 = "r";
      }
      else if (color1 == "GREEN")
      {
        color1 = "g";
      }
      else
      {
        color1 = "y";
      }
      if (color2 == "RED")
      {
        color2 = "r";
      }
      else if (color2 == "GREEN")
      {
        color2 = "g";
      }
      else
      {
        color2 = "y";
      }
      if (color3 == "RED")
      {
        color3 = "r";
      }
      else if (color3 == "GREEN")
      {
        color3 = "g";
      }
      else
      {
        color3 = "y";
      }
      myfile << "  <Placemark>" << std::endl;
      myfile << "    <name>" << CNN << "</name>" << std::endl;
      myfile << "    <description>" << streetName1 << ", " << streetName2 << ", and " << streetName3 << "/description>" << std::endl;
      myfile << "    <styleUrl>#i3" << color1 << color2 << color3 << "</styleUrl>" << std::endl;
      myfile << "    <Point>" << std::endl;
      myfile << "      <coordinates>" << point1 << "," << point2 << "</coordinates>" << std::endl;
      myfile << "    </Point>" << std::endl;
      myfile << "  </Placemark>" << std::endl;
    }
    else
    {
      std::string CNN = tc.CNN;
      std::string color1 = trafficLights.at(tc.trafficLightIndices.at(0)).getColor();
      std::string color2 = trafficLights.at(tc.trafficLightIndices.at(1)).getColor();
      std::string color3 = trafficLights.at(tc.trafficLightIndices.at(2)).getColor();
      std::string color4 = trafficLights.at(tc.trafficLightIndices.at(3)).getColor();
      std::string streetName1 = trafficLights.at(tc.trafficLightIndices.at(0)).getStreetName();
      std::string streetName2 = trafficLights.at(tc.trafficLightIndices.at(1)).getStreetName();
      std::string streetName3 = trafficLights.at(tc.trafficLightIndices.at(2)).getStreetName();
      std::string streetName4 = trafficLights.at(tc.trafficLightIndices.at(3)).getStreetName();
      std::string coordinates = trafficLights.at(tc.trafficLightIndices.at(0)).getPoint();
      coordinates = coordinates.substr(7, coordinates.length() - 8);
      int loc = coordinates.find(" ");
      std::string point1 = coordinates.substr(0, loc);
      std::string point2 = coordinates.substr(loc + 1, coordinates.length());
      if (color1 == "RED")
      {
        color1 = "r";
      }
      else if (color1 == "GREEN")
      {
        color1 = "g";
      }
      else
      {
        color1 = "y";
      }
      if (color2 == "RED")
      {
        color2 = "r";
      }
      else if (color2 == "GREEN")
      {
        color2 = "g";
      }
      else
      {
        color2 = "y";
      }
      if (color3 == "RED")
      {
        color3 = "r";
      }
      else if (color3 == "GREEN")
      {
        color3 = "g";
      }
      else
      {
        color3 = "y";
      }
      if (color4 == "RED")
      {
        color4 = "r";
      }
      else if (color4 == "GREEN")
      {
        color4 = "g";
      }
      else
      {
        color4 = "y";
      }
      myfile << "  <Placemark>" << std::endl;
      myfile << "    <name>" << CNN << "</name>" << std::endl;
      myfile << "    <description>" << streetName1 << ", " << streetName2 << ", " << streetName3 << ", and " << streetName4 << "/description>" << std::endl;
      myfile << "    <styleUrl>#i4" << color1 << color2 << color3 << color4 << "</styleUrl>" << std::endl;
      myfile << "    <Point>" << std::endl;
      myfile << "      <coordinates>" << point1 << "," << point2 << "</coordinates>" << std::endl;
      myfile << "    </Point>" << std::endl;
      myfile << "  </Placemark>" << std::endl;
    }
  }
}

int main(int argc, char **argv)
{
  try
  {
    std::string str(argv[1]);
    str = str.substr(3, strlen(argv[1]) - 3);
    int n = std::stoi(str);
    if (n < 0)
    {
      std::cout << "-t must equal an integer greater than -1" << std::endl;
      std::cout << "exiting program with code 1" << std::endl;
      return 1;
    }
    std::vector<std::vector<std::string>> parsedCSV = parseCSV();
    std::vector<TrafficLight> trafficLights = createTrafficLights(parsedCSV);
    std::vector<TrafficController> trafficControllers = createTrafficControllers(parsedCSV);
    std::priority_queue<Event, std::vector<Event>, Compare> pq = createPriorityQueue(trafficControllers);
    while (!(pq.empty()))
    {
      Event e = pq.top();
      TrafficController tc = trafficControllers.at(e.trafficControllerIndex);
      trafficLights = tc.performEvent(e, trafficLights);
      Event nextE = tc.calculateNextEvent(e, trafficLights);
      pq.pop();
      if (nextE.time <= n)
      {
        pq.push(nextE);
      }
    }
    writeCSV(trafficLights);
    std::vector<std::string> fileNames = createFileNames();
    TrafficController tc = trafficControllers.at(0);
    std::string CNN = tc.CNN;
    std::string color1 = trafficLights.at(tc.trafficLightIndices.at(0)).getColor();
    std::string color2 = trafficLights.at(tc.trafficLightIndices.at(1)).getColor();
    std::string streetName1 = trafficLights.at(tc.trafficLightIndices.at(0)).getStreetName();
    std::string streetName2 = trafficLights.at(tc.trafficLightIndices.at(1)).getStreetName();
    createkml(fileNames, trafficControllers, trafficLights);
    return 0;
  }
  catch (...)
  {
    std::cout << "You must supply an argument with the form \"-t=n\" where n is an integer greater than -1" << std::endl;
    std::cout << "exiting program with code 1" << std::endl;
    return 1;
  }
}