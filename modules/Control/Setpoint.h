#include <Task.h>
extern TaskManager taskManager;
class Setpoint : public Control
{
public:
  Setpoint(int ch) : Control(1000, ch)
  {
    UpdateFromRom();
  };

  void SetValue(float sp, float w, float d, int s)
  {
    setpoint = sp;
    working = w;
    detecting = d;
    sensor = s;
    SaveToRom();
  }

private:
  float setpoint, working, detecting;
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
    if (sensor == 0)
    {
      if (state == 0)
      {
        currentTime += (delta_time / 1000);
        if (currentTime >= detecting)
        {
          if (sensorValue > setpoint)
          {
            if(ChannelStatus[4] == CH_ON || ChannelStatus[5] == CH_ON){
              DigitalWrite(channel - 1, CH_OFF);
            }
            else{
              DigitalWrite(channel - 1, CH_ON);
            }
            state = 1;
            currentTime = 0;
          }
          else
          {
            currentTime = 0;
            state = 0;
          }
        }
      }

      if (state == 1)
      {
        currentTime += (delta_time / 1000);
        if (currentTime >= working)
        {
          DigitalWrite(channel - 1, CH_OFF);
          state = 0;
          currentTime = 0;
        }
      }
    }
    else
    {
      if (state == 0)
      {
        currentTime += (delta_time / 1000);
        if (currentTime >= detecting)
        {
          if (sensorValue < setpoint)
          {

            if(ChannelStatus[4] == CH_ON || ChannelStatus[5] == CH_ON){
              DigitalWrite(channel - 1, CH_OFF);
            }
            else{
              DigitalWrite(channel - 1, CH_ON);
            }

            state = 1;
            currentTime = 0;
          }
          else
          {
            currentTime = 0;
            state = 0;
          }
        }
      }

      if (state == 1)
      {
        currentTime += (delta_time / 1000);
        if (currentTime >= working)
        {
          DigitalWrite(channel - 1, CH_OFF);
          state = 0;
          currentTime = 0;
        }
      }
    }
    String str = "Setpoint is running " +
                    String(channel) + " " +
                    String(working) + " " +
                    String(detecting) + " " +
                    String(sensorValue) + "/" +
                    String(setpoint) + " " +
                    "st" + String(state) + " " +
                    "ct" + String(currentTime) + " " +
                    "ss" + String(sensor);

    //debugCom.println(str);
  }
  
  void UpdateFromRom()
  {
    working = rom_channel[channel - 1].setpoint.working;
    detecting = rom_channel[channel - 1].setpoint.detecting;
    setpoint = rom_channel[channel - 1].setpoint.setpoint;
    sensor = rom_channel[channel - 1].sensor;
  }

  void SaveToRom()
  {
    rom_channel[channel - 1].setpoint.working = working;
    rom_channel[channel - 1].setpoint.detecting = detecting;
    rom_channel[channel - 1].setpoint.setpoint = setpoint;
    rom_channel[channel - 1].sensor = sensor;
    EEPROM_Manager::Update(channel);
  }
};