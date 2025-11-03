#include <Arduino.h>
#include <MMFS.h>
#include <LANDEDstate.h>


Sensor *sensors[] = {};
NewState landed_state(sensors, 0, nullptr);
MMFSConfig config = MMFSConfig()
                        .withState(&landed_state);


// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}