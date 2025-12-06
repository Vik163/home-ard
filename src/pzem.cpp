#include <PZEM004Tv30.h>
#include <TimeLib.h> // Для работы с временем (установите через Library Manager)
#include <SoftwareSerial.h>

#include <header.hpp>

PZEM004Tv30 pzem(D1, D2); // (RX,TX)connect to TX,RX of PZEM
// если использовать не один блок
// PZEM004Tv30 pzem2(D5, D6);  // (RX,TX) connect to TX,RX of PZEM
// PZEM004Tv30 pzem3(D7, D8);  // (RX,TX) connect to TX,RX of PZEM

const char mqttpTopicVoltage[] = "home/state/voltage";
const char mqttpTopicCurrent[] = "home/state/current";
const char mqttpTopicPower[] = "home/state/power";
const char mqttpTopicFrequency[] = "home/state/frequency";
const char mqttpTopicEnergy[] = "home/state/energy";
const char mqttpTopicPf[] = "home/state/pf";

void resetEnergy()
{
   pzem.resetEnergy();
}

int readVoltage()
{
   float voltage = pzem.voltage();
   mqttPublish("Voltage: ", voltage, "V", mqttpTopicVoltage);

   float current = pzem.current();
   mqttPublish("Current: ", current, "A", mqttpTopicCurrent);

   float power = pzem.power();
   mqttPublish("Power: ", power, "W", mqttpTopicPower);

   float energy = pzem.energy();
   mqttPublish("Energy: ", energy, "kWh", mqttpTopicEnergy);

   float frequency = pzem.frequency();
   mqttPublish("Frequency: ", frequency, "Hz", mqttpTopicFrequency);

   float pf = pzem.pf();
   mqttPublish("PowerFactor: ", pf, "", mqttpTopicPf);

   return (int)voltage;
}
