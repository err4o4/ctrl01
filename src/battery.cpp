#include "battery.h"
#include <Arduino.h>
#include <esp_adc_cal.h>
#include <esp_sleep.h>
#include <logger.h>

BatteryManager::BatteryManager(int pin) : pin(pin), vref(1100), battery_voltage(0), previousMillis(0) {}

void BatteryManager::begin() {
    pinMode(pin, INPUT);
    esp_adc_cal_characteristics_t adc_chars;
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, & adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        LOG_TRACE("eFuse Vref:%u mV", adc_chars.vref);
        vref = adc_chars.vref;
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        LOG_TRACE("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
    } else {
        LOG_TRACE("Default Vref: 1100mV");
    }
}

void BatteryManager::handle() {
    if (millis() - previousMillis >= 3000) {
        checkBattery();
        previousMillis = millis();
    }
}

void BatteryManager::checkBattery() {
    uint16_t v = analogRead(34);
    battery_voltage = ((float) v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
    previousMillis = millis();
}

float BatteryManager::getBatteryVoltage() {
    return battery_voltage;
}