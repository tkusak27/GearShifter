#include <stdio.h>
#include <math.h>

int main()
{
    
    //CAR KINEMATICS PARAMs
    const int nGears = 6;
    const double dTire = 26; // diameter of tire in inches
    const double maxRPM = 6600.0;
    const double stallRPM = 2177.0; 
    const double vMax[] = {38.88, 69.02, 103.62, 135.52, 159.78, 188.87}; // in MPH
    const double tRedline[] = {2.88, 5.12, 7.69, 10.05, 11.85, 14.01}; // in seconds
    const double GR[] = {4.110, 2.315, 1.542, 1.179, 1.000, 0.846};
    const double final_drive_rto = 3.150;
    
    const double decelr8 = 6.82; //MPH per second
    double accelr8[nGears];
    
    // Calculate acceleration per gear in MPH per sec.
    for (int i = 0; i < nGears; i++) {
      if (i == 0) {
          // In first gear, initial speed is 0
          accelr8[i] = vMax[i] / tRedline[i];
      } else {
          // In other gears, initial speed is the top speed of the previous gear
          accelr8[i] = (vMax[i] - vMax[i - 1]) / tRedline[i];
      }
    }
    
    // Calculate RPM func
    double calc_RPM(double speed, double gr) {
      double rpm = (speed * gr * final_drive_rto * 336) / dTire;
      return rpm;
    }
    
    // Calculate speed
    double calc_speed(double a, double t){
      double speed = a * t;
      return speed;
    }


}