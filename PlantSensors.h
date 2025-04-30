
#ifndef SENSOR_DRIVER
#define SENSOR_DRIVER

extern void setupSensorPins(int, int);
extern int getSoilMoisture();
extern int getWaterLevel();

extern bool readDHT();
extern double returnTemp();
extern double returnHumid();
extern void DHT_setup(int);

#endif
