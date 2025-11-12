#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>
#include <header.hpp>

PZEM004Tv30 pzem(D5, D6); // Software Serial pin 11 (RX) & 12 (TX)

const char mqttpTopicVoltage[] = "home/state/voltage";
const char mqttpTopicCurrent[] = "home/state/current";
const char mqttpTopicPower[] = "home/state/power";
const char mqttpTopicFrequency[] = "home/state/frequency";
const char mqttpTopicEnergy[] = "home/state/energy";
const char mqttpTopicPf[] = "home/state/pf";

void readVoltage()
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
}