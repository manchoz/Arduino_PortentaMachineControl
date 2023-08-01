/*
  Machine Control - Thermocouples Read Sensors

  This example reads the temperatures measured by the thermocouples
  connected to the Machine Control Carrier's temp probe inputs and prints
  them to the Serial Monitor once a second.

  The circuit:
   - Portenta H7
   - Portenta Machine Control Carrier
   - Two K Type thermocouple temperature sensors connected to
      TEMP PROBES CH0 and CH1 on the Machine Control
   - A J Type thermocouple temperature sensor connected to
     TEMP PROBES CH3 on the Machine Control

  This example code is in the public domain.
*/

#include <Arduino_MachineControl.h>

void setup() {
  Serial.begin(9600);
  // Initialize temperature probes
  MachineControl_TCTempProbe.begin();
  Serial.println("Temperature probes initialization done");
}

void loop() {
  //Set CH0, has internal 150 ms delay
  MachineControl_TCTempProbe.selectChannel(0);
  //Take CH0 measurement
  float temp_ch0 = MachineControl_TCTempProbe.readTemperature();
  Serial.print("Temperature CH0 [°C]: ");
  Serial.print(temp_ch0);
  Serial.println();

  //Set CH1, has internal 150 ms delay
  MachineControl_TCTempProbe.selectChannel(1);
  //Take CH1 measurement
  float temp_ch1 = MachineControl_TCTempProbe.readTemperature();
  Serial.print("Temperature CH1 [°C]: ");
  Serial.print(temp_ch1);
  Serial.println();
 
  //Set CH2, has internal 150 ms delay
  MachineControl_TCTempProbe.selectChannel(2);
  //Take CH2 measurement
  float temp_ch2 = MachineControl_TCTempProbe.readTemperature();
  Serial.print("Temperature CH2 [°C]: ");
  Serial.print(temp_ch2);
  Serial.println();

  Serial.println();
}
