#include <Task.h>
extern TaskManager taskManager;
class SetBound : public Control
{
public:
  SetBound(int ch) : Control(1000, ch)
  {
    UpdateFromRom();
  }

  void SetValue(float u, float l, int s)
  {
    upper = u;
    lower = l;
    sensor = s;

    SaveToRom();
  }

private:
  float upper, lower;
  int sensor;
  int state = 0;
  virtual bool OnStart()
  {
    DigitalWrite(channel - 1, CH_OFF);
    return true;
  }
  virtual void OnUpdate(uint32_t delta_time)
  {

    float sensorValue = Sensor::instance()->GetSensor(sensor);
    String str = "Setbound is running " +
                 String(channel) + " " +
                 String(lower) + "/" +
                 String(sensorValue) + "/" +
                 String(upper) + "/" +
                 String(state) + " " +
                 String(sensor);
    //debugCom.println(str);

    if (sensor == 0)
    {

      if (state == 0)
      {
        bool upperCheck = sensorValue >= upper;
        bool lowerCheck = sensorValue >= lower;

        if (upperCheck && lowerCheck)
        {
          DigitalWrite(channel - 1, CH_ON);
          state = 1;
        }
      }

      if (state == 1)
      {
        bool upperCheck = sensorValue <= upper;
        bool lowerCheck = sensorValue <= lower;

        if (upperCheck && lowerCheck)
        {
          DigitalWrite(channel - 1, CH_OFF);
          state = 0;
        }
      }
    }
    else
    {

      if (state == 0)
      {
        bool upperCheck = sensorValue <= upper;
        bool lowerCheck = sensorValue <= lower;

        if (upperCheck && lowerCheck)
        {
          DigitalWrite(channel - 1, CH_ON);
          state = 1;
        }
      }

      if (state == 1)
      {
        bool upperCheck = sensorValue >= upper;
        bool lowerCheck = sensorValue >= lower;

        if (upperCheck && lowerCheck)
        {
          DigitalWrite(channel - 1, CH_OFF);
          state = 0;
        }
      }
    }
  }

  void UpdateFromRom()
  {
    upper = rom_channel[channel - 1].setbound.upper;
    lower = rom_channel[channel - 1].setbound.lower;
    sensor = rom_channel[channel - 1].sensor;
  }

  void SaveToRom()
  {
    rom_channel[channel - 1].setbound.upper = upper;
    rom_channel[channel - 1].setbound.lower = lower;
    rom_channel[channel - 1].sensor = sensor;
    EEPROM_Manager::Update(channel);
  }
};