

#include <Task.h>
extern TaskManager taskManager;
class AdvanceSetbound : public Control
{
  public:
    AdvanceSetbound(int ch) : Control(1000, ch)
    {
        UpdateFromRom();
    }

  private:
    /*
    Advance temperature parameters
        there 3 level of condition
        -> Timer level
            - timer_list, timer_size, timer_flag
        -> sensor level
            - sensor, sensor_setpoint, sensor_flag
        -> working level
            - setpoint, working, detecting, condition
  */
    timer_s timer_list[3];
    int timer_size;
    bool timer_flag;

    int sensor_condition;
    float sensor_setpoint;
    bool sensor_flag, sensor_direction;

    int sensor;
    float upper, lower;
    bool direction;

    int state;

    virtual bool OnStart()
    {
        state = 0;
        currentTime = 0;

        DigitalWrite(channel - 1, CH_OFF);
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {

        
        if (CheckTimer() && CheckSensorCondition())
        {
            CoreControl(delta_time);
        }
        else
        {
            DigitalWrite(channel - 1, CH_OFF);
        }
    }

    bool CheckTimer()
    {

        if (!timer_flag)
            return true;

        int currentMin = RTC::instance()->GetCurrentMin();
        bool flag = false;
        for (int i = 0; i < 3; i++)
        {
            if (currentMin >= timer_list[i].st && currentMin <= timer_list[i].en)
            {
                flag = true;
                break;
            }
        }
        return flag;
    }

    bool CheckSensorCondition()
    {
        if (!sensor_flag)
            return true;

        float sensor_value = Sensor::instance()->GetSensor(sensor_condition);

        if (sensor_direction == 1)
        {
            if (sensor_value > sensor_setpoint)
                return true;
            else
                return false;
        }
        else
        {
            if (sensor_value <= sensor_setpoint)
                return true;
            else
                return false;
        }
    }

    bool CoreControl(uint32_t delta_time)
    {

        float sensorValue = Sensor::instance()->GetSensor(sensor);
        String str = "d:" + String(direction) + ",s:" + String(sensorValue) + "," + String(upper) + "/" + String(lower);
        mpuCom.println(str);
        if (direction == 1)
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
    void ArrayTimerCopy(timer_s *from, timer_s *to, int size)
    {
        int i = 0;
        for (i = 0; i < size; i++)
        {
            to[i] = from[i];
        }
    }

    void UpdateFromRom()
    {
        ArrayTimerCopy(rom_channel[channel - 1].advsb.timer_list, timer_list, 3);
        timer_size = rom_channel[channel - 1].advsb.timer_size;
        timer_flag = rom_channel[channel - 1].advsb.timer_flag;

        // temp = 3
        sensor_condition = rom_channel[channel - 1].advsb.sensor_condition;
        sensor_direction = rom_channel[channel - 1].advsb.sensor_direction;
        sensor_setpoint = rom_channel[channel - 1].advsb.sensor_setpoint;
        sensor_flag = rom_channel[channel - 1].advsb.sensor_flag;

        //co2 = 5
        sensor = rom_channel[channel - 1].advsb.sensor;
        direction = rom_channel[channel - 1].advsb.direction;
        upper = rom_channel[channel - 1].advsb.upper;
        lower = rom_channel[channel - 1].advsb.lower;
    }

    void SaveToRom()
    {
        EEPROM_Manager::Update(channel);
    }
};