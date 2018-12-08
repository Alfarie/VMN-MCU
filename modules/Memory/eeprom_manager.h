#include <EEPROM.h>
#include "./address.h"

struct data_manual_s
{
    byte status;
};
struct data_timer_s
{
    byte size;
    byte mode;
    timer_s timer_list[8];
};
// struct data_setpoint_s
// {
//     float setpoint;
//     float detecting;
//     float working;
// };
// struct data_setbound_s
// {
//     float upper;
//     float lower;
// };
// struct data_setboundtiming_s
// {
//     float upper;
//     float lower;
//     float detecting;
//     float working;
// };
struct data_irr_s
{
    float soil_upper;
    float soil_lower;
    float soil_detecting;
    float soil_working;

    float descent_rate;
    float limit_time;

    float par_soil_setpoint;
    float par_detecting;
    float par_working;
    float par_acc;
    byte mode;
};

struct data_advcond
{
    timer_s timer_list[3];
    int timer_size;
    bool timer_flag;

    int sensor_condition;
    float sensor_setpoint;
    bool sensor_flag;
    bool sensor_direction;

    int sensor;
    bool direction;
    float setpoint;
    float working;
    float detecting;
};
struct data_advsbt
{
    timer_s timer_list[3];
    int timer_size;
    bool timer_flag;

    int sensor_condition;
    float sensor_setpoint;
    bool sensor_flag;
    bool sensor_direction;

    int sensor;
    bool direction;
    float upper;
    float lower;
    float detecting;
    float working;
};

struct data_advsb
{
    timer_s timer_list[3];
    int timer_size;
    bool timer_flag;

    int sensor_condition;
    float sensor_setpoint;
    bool sensor_flag;
    bool sensor_direction;

    int sensor;
    bool direction;
    float upper;
    float lower;
};

struct data_table_s
{
    byte mode;
    byte gpio_status;
    byte sensor;
    data_timer_s timer;
    data_manual_s manual;
    data_irr_s irrigation;
    data_advcond advcond;
    data_advsbt advsbt;
    data_advsb advsb;
};

struct data_water_process
{
    byte isCir;
    int cirTime;
    int waitingTime;
    byte isFill;
};

struct data_calibration
{
    float ecCal;
    float phCal;
};
data_table_s rom_channel[CHANNEL_NUMBER];
data_water_process waterProcess;
data_calibration calibrationData;

class EEPROM_Manager
{
  public:
    EEPROM_Manager(){};
    static int init_byte;
    static int water_process_byte;
    static int calibration_byte;
    static void InitEEPROM()
    {

#if !defined(ARDUINO_ARCH_AVR)
        // EEPROM.begin(EEPROM_SIZE);
        EEPROM.begin(2048);
#endif
        byte init;
        // EEPROM.put(init_byte, 1);
        // Commit();
        EEPROM.get(init_byte, init);
        mpuCom.println("INFO-MEMCHSIZE :" + String(sizeof(data_table_s)));
        if (init == 2)
        {
            //debugCom.println("Initialize eeprom GetAll data");
            //testCom.println("Initialize eeprom GetAll data");
            mpuCom.println("INFO-GETMEMORY");
            GetAll();
        }
        else
        {
            mpuCom.println("INFO-CREATEMEMORY");
            data_table_s dt = InitData();
            data_water_process wt = InitWaterProcess();
            data_calibration cal = {1.1, 1.1};

            for (int i = 0; i < 4; i++)
            {
                EEPROM.put(channel_list[i], dt);
            }

            EEPROM.put(water_process_byte, wt);
            EEPROM.put(calibration_byte, cal);

            EEPROM.put(init_byte, 2);
            Commit();
            GetAll();
        }
    }
    static void Update(int channel)
    {
        DisplayLog::PrintInfo(" Update to Channel " + String(channel) + " " + String(channel_list[channel - 1]));

        // Serial.println("size: " + String(rom_channel[channel - 1].timer_size));
        // for(int i = 0 ; i < rom_channel[channel - 1].timer_size; i++){
        //     Serial.print( String(rom_channel[channel - 1].timer_list[i].st) + "-" + String(rom_channel[channel - 1].timer_list[i].en) + " ");
        // }
        // Serial.println();
        EEPROM.put(channel_list[channel - 1], rom_channel[channel - 1]);
        Commit();
    }
    static void UpdateWaterProcess()
    {
        EEPROM.put(water_process_byte, waterProcess);
        //testCom.println("WaterProcess Updated");
        Commit();
    }

    static void UpdateCalibration()
    {
        EEPROM.put(calibration_byte, calibrationData);
        //testCom.println("WaterProcess Updated");
        Commit();
    }
    static data_water_process InitWaterProcess()
    {
        data_water_process water_process;
        water_process = {0};
        water_process.isCir = true;
        water_process.cirTime = 900;
        water_process.waitingTime = 900;
        water_process.isFill = true;

        return water_process;
    }
    static data_table_s InitData()
    {
        data_table_s initData;
        initData = {0};
        initData.mode = 0;
        initData.sensor = 1;
        initData.manual.status = 0;

        //{timer,3,1,300-480,540-720,780-960,1020-1200,1260-1439}
        timer_s timer_list[8];
        initData.timer.timer_list[0] = {300, 480};
        initData.timer.timer_list[1] = {540, 720};
        initData.timer.timer_list[2] = {780, 960};
        initData.timer.timer_list[3] = {1020, 1200};
        initData.timer.timer_list[4] = {1260, 1439};
        initData.timer.size = 5;
        initData.timer.mode = 0;

        // initData.setpoint.setpoint = 40.0;
        // initData.setpoint.detecting = 30;
        // initData.setpoint.working = 30;

        // initData.setbound.upper = 60;
        // initData.setbound.lower = 40;

        initData.irrigation.soil_upper = 60;
        initData.irrigation.soil_lower = 40;
        initData.irrigation.soil_detecting = 30;
        initData.irrigation.soil_working = 15;

        initData.irrigation.par_soil_setpoint = 50;
        initData.irrigation.par_detecting = 30;
        initData.irrigation.par_working = 15;
        initData.irrigation.par_acc = 1.5;
        initData.irrigation.mode = 0;

        initData.irrigation.limit_time = 3;
        initData.irrigation.descent_rate = 0.2;

        // initData.setboundtiming.upper = 60;
        // initData.setboundtiming.lower = 40;
        // initData.setboundtiming.detecting = 30;
        // initData.setboundtiming.working = 15;

        initData.advcond.timer_list[0] = {480, 1080};
        initData.advcond.timer_size = 1;
        initData.advcond.timer_flag = true;

        // temp = 3
        initData.advcond.sensor_condition = 3;
        initData.advcond.sensor_direction = 0;
        initData.advcond.sensor_setpoint = 30;
        initData.advcond.sensor_flag = true;
        //co2 = 5
        initData.advcond.sensor = 5;
        initData.advcond.direction = 0;
        initData.advcond.setpoint = 600;
        initData.advcond.working = 15;
        initData.advcond.detecting = 30;

        initData.advsbt.timer_list[0] = {480, 1080};
        initData.advsbt.timer_size = 1;
        initData.advsbt.timer_flag = true;
        // temp = 3
        initData.advsbt.sensor_condition = 3;
        initData.advsbt.sensor_direction = 0;
        initData.advsbt.sensor_setpoint = 30;
        initData.advsbt.sensor_flag = true;
        //co2 = 5
        initData.advsbt.sensor = 5;
        initData.advsbt.direction = 0;
        initData.advsbt.upper = 1000;
        initData.advsbt.lower = 800;
        initData.advsbt.working = 15;
        initData.advsbt.detecting = 30;

        initData.advsb.timer_list[0] = {480, 1080};
        initData.advsb.timer_size = 1;
        initData.advsb.timer_flag = true;
        // temp = 3
        initData.advsb.sensor_condition = 3;
        initData.advsb.sensor_direction = 0;
        initData.advsb.sensor_setpoint = 30;
        initData.advsb.sensor_flag = true;
        //co2 = 5
        initData.advsb.sensor = 5;
        initData.advsb.direction = 0;
        initData.advsb.upper = 1000;
        initData.advsb.lower = 800;

        return initData;
    }

    static void GetAll()
    {
        for (int i = 0; i < CHANNEL_NUMBER; i++)
        {
            EEPROM.get(channel_list[i], rom_channel[i]);
        }
        // EEPROM.get(water_process_byte, waterProcess);
        // EEPROM.get(calibration_byte, calibrationData);
    }

    static void Commit()
    {
#if !defined(ARDUINO_ARCH_AVR)
        EEPROM.commit();
#endif
    }

  private:
    static int channel_list[CHANNEL_NUMBER];
};
int EEPROM_Manager::init_byte = 0;
int EEPROM_Manager::water_process_byte = 50;
int EEPROM_Manager::calibration_byte = 20;
int EEPROM_Manager::channel_list[CHANNEL_NUMBER] = {100, 500, 1000, 1500};