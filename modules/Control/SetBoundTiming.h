#include <Task.h>
extern TaskManager taskManager;
class SetBoundTiming : public Control
{
  public:
    SetBoundTiming(int ch) : Control(1000, ch)
    {
        state = 0;
        current_time = 0;
        UpdateFromRom();
    }

  private:
    float upper, lower, detecting, working, current_time;
    int state, sensor;
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
            current_time += (delta_time / 1000);
            if (state == 0)
            {
                DigitalWrite(channel - 1, CH_OFF);
                if (current_time > detecting)
                {
                    bool working_condition = sensor > lower && sensor > upper;
                    if (working_condition)
                        state = 2;

                    current_time = 0;
                }
            }
            else if (state == 1)
            {
                DigitalWrite(channel - 1, CH_OFF);
                if (current_time > detecting)
                {
                    bool working_condition = sensor > lower;
                    if (working_condition)
                        state = 2;
                    else
                    {
                        state = 0;
                    }
                    current_time = 0;
                }
            }
            else if (state == 2)
            {
                DigitalWrite(channel - 1, CH_ON);
                if (current_time > working)
                {
                    state = 1;
                    current_time = 0;
                }
            }
        }

        else
        {
            current_time += (delta_time / 1000);
            if (state == 0)
            {
                DigitalWrite(channel - 1, CH_OFF);
                if (current_time > detecting)
                {
                    bool working_condition = sensor < lower && sensor < upper;
                    if (working_condition)
                        state = 2;
                    current_time = 0;
                }
            }
            else if (state == 1)
            {
                DigitalWrite(channel - 1, CH_OFF);
                if (current_time > detecting)
                {
                    bool working_condition = sensor < upper;
                    if (working_condition)
                        state = 2;
                    else
                    {
                        state = 0;
                    }
                    current_time = 0;
                }
            }
            else if (state == 2)
            {
                DigitalWrite(channel - 1, CH_ON);
                if (current_time > working)
                {
                    state = 1;
                    current_time = 0;
                }
            }
        }
    }

    void UpdateFromRom()
    {
        upper = rom_channel[channel - 1].setboundtiming.upper;
        lower = rom_channel[channel - 1].setboundtiming.lower;
        detecting = rom_channel[channel - 1].setboundtiming.detecting;
        working = rom_channel[channel - 1].setboundtiming.working;
    }

    void SaveToRom()
    {
        rom_channel[channel - 1].setboundtiming.upper = upper;
        rom_channel[channel - 1].setboundtiming.lower = lower;
        rom_channel[channel - 1].setboundtiming.detecting = detecting;
        rom_channel[channel - 1].setboundtiming.working = working;
        EEPROM_Manager::Update(channel);
    }
};