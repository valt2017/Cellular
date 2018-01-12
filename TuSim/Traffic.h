#ifndef TRAFFICSIM_H
#define TRAFFICSIM_H

#include <string>

typedef struct 
{
  char Speed;
  char VehType;
}Vehicle;
                                    // vehicle types
const char CAR = 'C';               // visualization letter car
const char VAN = 'V';               // visualization letter van
const char BUS = 'B';               // visualization letter bus
const char TRUCK = 'T';             // visualization letter truck
const char EMPTY_CELL = ' ';        // visualization letter empty
                                    // vehicle percentage
const double VANS_PER = 0.05;       // % vans in traffic flow
const double BUS_PER = 0.05;        // % busses in traffic flow 
const double TRUCKS_PER = 0.05;     // % trucks in traffic flow 
const double CARS_PER = 1-VANS_PER-BUS_PER-TRUCKS_PER; // % cars in traffic flow
const double LNG_CELL = 7.5;        // length of cell [m] 
const int CNT_CELLS = 100;          // number of cells in whole tunnel 100*LNG_CELL = 750m,
const double L = LNG_CELL*CNT_CELLS;// length of tunnel [m]=L*LNG_CELL
const int NUM_LANES = 2;            // traffic lanes
const char MAX_SPEED = 5;           // 5 cells - 130 km/h, 4 cells - 108km/h, 3 cells - 80 km/h
const double SLOW_DOWN = 0.25;      // slow down probability - 0.25 Chowdbury, Santen, Schadschneider (2000)
const double CHANGE_LANE = 0.7;     // change lane probability 70%
const double MS_TO_KMH = 3.6;       // m/s
const int MIN_SECS = 60;        
const int MIN_HOD = 60;
const int INTENSITY = 1200;          // veh/h
const int TIME_SIM = 15*MIN_SECS;    // [min]->[s]

extern double avgSpeed;
extern double density;
extern int numCars;
extern Vehicle road[CNT_CELLS][NUM_LANES];

void TrafficSimStep(int time, std::string & s);
void InitTraffic();
void ExitTraffic();

#endif