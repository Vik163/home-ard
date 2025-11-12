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
const bool mqttpSensorRetained = false;

void readItem(const char *title, float value, const char *item, const char *topic)
{

   if (value != NAN)
   {
      printf("%s%.1f %s\n", title, value, item);
   }
   else
   {
      printf("Error reading: %s\n", title);
   }

   String str_temp(value);
   mqttPublish(topic, str_temp.c_str(), mqttpSensorRetained);
}

void readVoltage()
{
   float voltage = pzem.voltage();
   readItem("Voltage: ", voltage, "V", mqttpTopicVoltage);

   float current = pzem.current();
   readItem("Current: ", current, "A", mqttpTopicCurrent);

   float power = pzem.power();
   readItem("Power: ", power, "W", mqttpTopicPower);

   float energy = pzem.energy();
   readItem("Energy: ", energy, "kWh", mqttpTopicEnergy);

   float frequency = pzem.frequency();
   readItem("Frequency: ", frequency, "Hz", mqttpTopicFrequency);

   float pf = pzem.pf();
   readItem("PowerFactor: ", pf, "", mqttpTopicPf);
}