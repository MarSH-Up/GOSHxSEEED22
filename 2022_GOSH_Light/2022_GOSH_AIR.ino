#include <Arduino.h>
#include "sensirion_common.h"
#include "sgp30.h"


void AirSensor_Init(){
    s16 err;
    u32 ah = 0;
    u16 scaled_ethanol_signal, scaled_h2_signal;
    /*  Init module,Reset all baseline,The initialization takes up to around 15 seconds, during which
        all APIs measuring IAQ(Indoor air quality ) output will not change.Default value is 400(ppm) for co2,0(ppb) for tvoc*/
    while (sgp_probe() != STATUS_OK) {
        Serial.println("SGP failed");
        while (1);
    }
    /*Read H2 and Ethanol signal in the way of blocking*/
    err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal,
                                            &scaled_h2_signal);
    if (err == STATUS_OK) {
        Serial.println("get ram signal!");
    } else {
        Serial.println("error reading signals");
    }

    // Set absolute humidity to 13.000 g/m^3
    //It's just a test value
    sgp_set_absolute_humidity(13000);
    err = sgp_iaq_init();
}
float* CO2_VOC_Extraction(){
    static float VOvalues[2];
    s16 err = 0;
    u16 tvoc_ppb, co2_eq_ppm;
    err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
    VOvalues[0] = tvoc_ppb;
    VOvalues[1] = co2_eq_ppm;

    return VOvalues;
}
void setup() {

    Serial.begin(115200);
    Serial.println("serial start!!");
    AirSensor_Init();

}

void loop() {
  float* AIR = CO2_VOC_Extraction();
  Serial.print("VOC: ");
  Serial.println(AIR[0]);
  Serial.print("CO2: ");
  Serial.println(AIR[1]);
  delay(1000);
}