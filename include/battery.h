#ifndef BATTERYMANAGER_H
#define BATTERYMANAGER_H

class BatteryManager {
public:
    BatteryManager(int pin);
    void begin();
    float getBatteryVoltage();
    void checkBattery();
    void handle();

private:
    int pin;
    int vref;
    float battery_voltage;
    unsigned long previousMillis;
};

#endif // BATTERYMANAGER_H
