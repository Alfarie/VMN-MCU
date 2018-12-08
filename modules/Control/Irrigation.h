#include <Task.h>
extern TaskManager taskManager;
class Irrigation : public Control
{
  public:
    static float channel_paracc[4];
    Irrigation(int ch) : Control(1000, ch)
    {
        soil = 0;
        par = 0;
        max_par = 0;
        UpdateFromRom();
    };
    
    float GetCurrentPar()
    {
        return current_par_acc;
    }

  private:
    float soil_upper, soil_lower, soil_working, soil_detecting;
    float par_soil_setpoint, par_working, par_detecting, par_acc;
    float current_par_acc;
    float current_time = 0, current_time_c2 = 0;
    float soil, par;
    double max_par;
    int mode, state, state_c2;

    virtual bool OnStart()
    {
        DigitalWrite(channel - 1, CH_OFF);
        state = 0;
        state_c2 = 0;
        current_par_acc = 0;
        max_par = (par_acc * (1000000.00));
        return true;
    }
// {irrigation,4,1,60,40,30,15,50,15,30,0.1}
// {manual,1,0}
    virtual void OnUpdate(uint32_t delta_time)
    {
        par = Sensor::instance()->GetSensor(2);
        soil = Sensor::instance()->GetSensor(1);
        Irrigation::channel_paracc[channel - 1] = current_par_acc;
        // mpuCom.println(String(channel) + " " + current_par_acc + " " + String(state) + " " + String(mode));
        // only soil
        if (mode == 0)
        {
            current_time += (delta_time / 1000);
            if (state == 0)
            {
                DigitalWrite(channel - 1, CH_OFF);
                if (current_time > soil_detecting)
                {
                    bool working_condition = soil < soil_lower && soil < soil_upper;
                    if (working_condition)
                        state = 2;

                    current_time = 0;
                }
            }
            else if (state == 1)
            {
                DigitalWrite(channel - 1, CH_OFF);
                if (current_time > soil_detecting)
                {
                    bool working_condition = soil < soil_upper;
                    if (working_condition)
                        state = 2;
                    else{
                        state = 0;
                    }
                    current_time = 0;
                }
            }
            else if (state == 2)
            {
                DigitalWrite(channel - 1, CH_ON);
                if (current_time > soil_working)
                {
                    state = 1;
                    current_time = 0;
                }
            }
        }
        //only par
        
        else if (mode == 1)
        {
            if (state == 0)
            {
                current_par_acc += par;
                if (current_par_acc > max_par)
                {
                    bool working_condition = soil < par_soil_setpoint;
                    if (working_condition){
                        state = 2;
                    }
                    else{
                        state = 1;
                    }
                }
            }
            else if (state == 1)
            {
                current_time += (delta_time / 1000);
                if (current_time > par_detecting)
                {/*  */
                    bool working_condition = soil < par_soil_setpoint;
                    if (working_condition)
                        state = 2;
                    else{
                        state = 0;
                        par = 0;
                        current_par_acc = 0;
                    }
                        
                    current_time = 0;
                }
            }
            else if (state == 2)
            {
                current_time += (delta_time / 1000);
                DigitalWrite(channel - 1, CH_ON);
                if (current_time > par_working)
                {
                    state = 1;
                    current_time = 0;
                }
            }

            current_time_c2 += (delta_time / 1000);
            if (state_c2 == 0)
            {
                DigitalWrite(channel - 1, CH_OFF);
                if (current_time_c2 > soil_detecting)
                {
                    bool working_condition = soil < soil_lower;
                    if (working_condition)
                        state_c2 = 1;

                    current_time_c2 = 0;
                }
            }
            else if (state_c2 == 1)
            {
                DigitalWrite(channel - 1, CH_ON);
                if (current_time_c2 > soil_working)
                {
                    state_c2 = 0;
                    current_time_c2 = 0;
                }
            }

        }
    }

    void UpdateFromRom()
    {
        soil_upper = rom_channel[channel - 1].irrigation.soil_upper;
        soil_lower = rom_channel[channel - 1].irrigation.soil_lower;
        soil_detecting = rom_channel[channel - 1].irrigation.soil_detecting;
        soil_working = rom_channel[channel - 1].irrigation.soil_working;

        par_soil_setpoint = rom_channel[channel - 1].irrigation.par_soil_setpoint;
        par_working = rom_channel[channel - 1].irrigation.par_working;
        par_detecting = rom_channel[channel - 1].irrigation.par_detecting;
        par_acc = rom_channel[channel - 1].irrigation.par_acc;
        mode = rom_channel[channel - 1].irrigation.mode;
    }
    void SaveToRom()
    {
        rom_channel[channel - 1].irrigation.soil_upper = soil_upper;
        rom_channel[channel - 1].irrigation.soil_lower = soil_lower;
        rom_channel[channel - 1].irrigation.soil_detecting = soil_detecting;
        rom_channel[channel - 1].irrigation.soil_working = soil_working;

        rom_channel[channel - 1].irrigation.par_soil_setpoint = par_soil_setpoint;
        rom_channel[channel - 1].irrigation.par_working = par_working;
        rom_channel[channel - 1].irrigation.par_detecting = par_detecting;
        rom_channel[channel - 1].irrigation.par_acc = par_acc;
        rom_channel[channel - 1].irrigation.mode = mode;
        EEPROM_Manager::Update(channel);
    }
};

float Irrigation::channel_paracc[4] = {0, 0, 0, 0};