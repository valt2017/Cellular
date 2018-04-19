#include "stdafx.h"
#include "traffic.h"
#include <iostream>
#include <time.h>
#include <cmath>
#include <sstream>

using namespace std;

int timeNewCar;
int numCars;
int gap;
double intensity;
double avgSpeed;
double sumSpeed;
double density;
double lane;

int Lfire;            // [m]
int TrafficStop;      // [min]
int TrafficGo;        // [min]

Vehicle road[CNT_CELLS][NUM_LANES];
char speedMax[CNT_CELLS][NUM_LANES];

// std random, uniform distribution, ...
double rand01(void)
{
  return((double)rand()/(double)RAND_MAX);
}

int RunProbab(double Probability, int Experiments)
{
  int Success = 0;
  for(int i=0; i<Experiments; i++)
    if(rand01() < Probability)
	    Success++;
  return(Success);
}
// Lane preference according to traffic intensity
double LanePref(double Inten)
{
  return(1 - 0.00035*Inten + 0.00000005*Inten*Inten);
}

// new car arrival - random type 
char CarType()
{
  double CarType = rand01();

  if(CarType < TRUCKS_PER)
    return(TRUCK);
  if(CarType < BUS_PER + TRUCKS_PER)
    return(BUS);
  if(CarType < BUS_PER + TRUCKS_PER + VANS_PER)
    return(VAN);
  return(CAR);
}
// gap forward
int findgapF(int index, int lane)
{
  for(int k=1; k<=road[index][lane].Speed; k++)
    if(index+k < CNT_CELLS && road[index+k][lane].Speed != EMPTY_CELL)
      return k-1;
  return MAX_SPEED;
}
// gap backward
int findgapB(int index, int lane)
{
  for(int k=1; k<=road[index][lane].Speed; k++)
    if(index-k >= 0 && road[index-k][lane].Speed != EMPTY_CELL)
      return k-1;
  return MAX_SPEED;
}

void InitTraffic()
{
  srand(time(NULL));
  timeNewCar = 0;
  numCars = 0;
  gap = 0;
  intensity = 0;
  avgSpeed = 0;
  sumSpeed = 0;
  density = 0;
  lane = 0;
  Lfire = 500; // m
  TrafficStop = 5; // 5 min - traffic stop simulation
  TrafficGo = 10; // 10 min - traffic go simulation

  for(int j=0; j<NUM_LANES; j++)
    for(int i=0; i<CNT_CELLS; i++) 
    {
      road[i][j].Speed = EMPTY_CELL;
      road[i][j].VehType = 0;
    }
  memset(speedMax, MAX_SPEED, sizeof(speedMax));
}

void ExitTraffic()
{
   //... 
}
// exponential 
double exponential(double Lambda)
{
  return(-log(rand01())/Lambda);
}
// new car - Exponential / Bernoulli arrival
int NewCar(int Intensity)
{
  double TimeCarReal;
  // < 600 veh/h exponential
  if(Intensity < 600)
    TimeCarReal = exponential((double)(Intensity)/(double)(MIN_HOD*MIN_HOD));
  else
  { // Bernoulli arrival
    if(rand01() < (double)(Intensity)/(double)(MIN_HOD*MIN_HOD)) 			   
	    TimeCarReal = 0;
	  else
	    TimeCarReal = (double)(MIN_HOD*MIN_HOD)/(double)(Intensity);
  }
  return((int)floor(TimeCarReal + 0.5));	  
}
// simulation step
void TrafficSimStep(int time, string & s)
{
  stringstream ss;
  // Average speed
  sumSpeed = 0;
  numCars = 0;
  // Traffic stop - jam/fire 
  if(time == TrafficStop * MIN_SECS)
  { // change maximum speed in specified area
    for(int i=int(Lfire/LNG_CELL)-1; i<int(Lfire/LNG_CELL) + MAX_SPEED; i++)
    {
      speedMax[i][0] = 0;
      speedMax[i][1] = 0;
    }
  }
  // Traffic go - remove speed limits
  if (time == TrafficGo * MIN_SECS)
  { // change maximum speed in specified area
	  for (int i = int(Lfire / LNG_CELL) - 1; i<int(Lfire / LNG_CELL) + MAX_SPEED; i++)
	  {
		  speedMax[i][0] = MAX_SPEED;
		  speedMax[i][1] = MAX_SPEED;
	  }
  }
  // Basic CA rules
  for(int j=0; j<NUM_LANES; j++)
		for(int i=0; i<CNT_CELLS; i++)  
      if(road[i][j].Speed != EMPTY_CELL)
      { 
        numCars++;
        sumSpeed += road[i][j].Speed;    
        // Acceleration: Maximum speed? No -> speed+1
        if(road[i][j].Speed < speedMax[i][j])
          road[i][j].Speed++; 
        // Decelaration: Gap==speed? No-> speed=gap
        gap = findgapF(i,j);
        if(gap < road[i][j].Speed)
          road[i][j].Speed = gap;
        // Randomization - slowing down
        if(road[i][j].Speed > 1 && rand01() < SLOW_DOWN) //road[i][j] != 0
            road[i][j].Speed--;
        // Decelaration: over maximum speed? Yes-> speed-1
        for(int k=1; k<=road[i][j].Speed; k++)
          if(i+k < CNT_CELLS && road[i][j].Speed > speedMax[i+k][j])
          { 
            road[i][j].Speed--;
            break;
          }
      }
  // Traffic parameters
  if(numCars != 0)
    avgSpeed = sumSpeed / numCars;
  density = (double)numCars*1000/(CNT_CELLS*LNG_CELL);
  intensity = avgSpeed*LNG_CELL*density*MS_TO_KMH;
  lane = LanePref(intensity);
  // Ouput string
  ss << "Time:" << time <<" s            "<< endl;
  ss << "Time new car:" << timeNewCar <<" s            "<< endl;
  ss << "Num cars:" << numCars <<" "<< endl;
  ss << "Average speed:" << avgSpeed*LNG_CELL*MS_TO_KMH << " km/h       " << endl;
  ss << "Density:" << density <<" veh/km      "<< endl;
  ss << "Intensity:" << intensity <<" veh/h      "<< endl;
  ss << "Right lane pref.:" << lane*100 <<" %      "<< endl;
  // Move to new positions
  for(int j=0; j<NUM_LANES; j++)
	for(int i=CNT_CELLS-1; i>=0; i--)  
    {
	  if(road[i][j].Speed != EMPTY_CELL && road[i][j].Speed != 0)
      {
        if(i + road[i][j].Speed < CNT_CELLS)
          road[i + road[i][j].Speed][j] = road[i][j];
        else 
          numCars--;
        road[i][j].Speed = EMPTY_CELL;
        road[i][j].VehType = 0;
      }
    }
  // New car entry
  if(timeNewCar == 0)
  { 
    timeNewCar = NewCar(INTENSITY);
    numCars++;
    double LeftRight = rand01();
    if(LeftRight < lane)
    {
      if(road[0][1].Speed == EMPTY_CELL)
      {
        road[0][1].Speed = MAX_SPEED; 
        road[0][1].VehType = CarType();
      }
    }
    else 
    {
      if(road[0][0].Speed == EMPTY_CELL)
      {
        road[0][0].Speed = MAX_SPEED; 
        road[0][0].VehType = CarType();
      }
    }
  }
  else
    timeNewCar--;
  // Switch lane ?
  for(int j=0; j<NUM_LANES; j++)
  {
    int nextlane = (j+1) % 2;
	  for(int i=0; i<CNT_CELLS; i++)  
      if(road[i][j].Speed != EMPTY_CELL)
      {
        if((findgapF(i,j) < road[i][j].Speed && speedMax[i][j] == speedMax[i][nextlane])  // 6. max speed in the other lane is higher
          || speedMax[i][j] < speedMax[i][nextlane])
        //if(findgapF(i,j) < road[i][j].Speed)                     // 1. distance ahead in current lane is smaller than current speed
          if(findgapF(i,nextlane) > road[i][j].Speed)            //>= 2. distance ahead in another lane is larger than in current lane
            if(road[i][nextlane].Speed == EMPTY_CELL)            // 3. there is empty cell right in another lane
              if(findgapB(i,nextlane) >= road[i][j].Speed)       // 4. The distance ahead of the following vehicle in another lane is larger than the speed of the following vehicle *
                if(rand01() < CHANGE_LANE)               // 5. change the lane with the probability
                {
                  road[i][nextlane] = road[i][j];    // Switch lane
                  road[i][j].Speed = EMPTY_CELL;
                  road[i][j].VehType = 0;
                }
      }
  }
  // Print lanes
  for(int j=0; j<NUM_LANES; j++) 
  {
    for(int i=0; i<CNT_CELLS; i++) 
    {
      if(road[i][j].Speed != EMPTY_CELL)
      {
          ss << (int)road[i][j].Speed;
          //ss << (int) road[i][j].NumPers % 10;
          //ss << road[i][j].VehType;
      }
      else
        ss << '_';
    }
    ss << endl;
  }
  s = ss.str();
}