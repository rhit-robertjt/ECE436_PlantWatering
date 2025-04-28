#ifndef PUMP_RELAY
#define PUMP_RELAY

extern void ARDUINO_ISR_ATTR onTimer();
extern void setBubblerOnTime(int time);
extern void setBubblerOffTime(int);
extern int  getBubblerOnTime();
extern int  getBubblerOffTime();
extern void setWatererOnTime(int);
extern void setWatererOffTime(int);
extern int  getWatererOnTime();
extern int  getWatererOffTime();
extern int  getBubblerRelayState();
extern int  getWatererRelayState();
extern int  getBubblerTimerTrigger();
extern int  getWatererTimerTrigger();
extern void endRelayTimer();
extern void setupRelay(int, int);

#endif