#include <Task.h>
TaskManager taskManager;
TaskManager taskManager0;
#include <Wire.h>
#include <EEPROM.h>

#define EEPROM_SIZE 1024
#define CHANNEL_NUMBER 4
#define VERSION 2.0
#define PROJECT "VMN-GROBOT"
#define UPLOADDATE String(__DATE__) + " " + String(__TIME__)
#define ledPin 2
#define co2Pin 4
#define ecPin 6
#define phPin 7
#define pumpPin 3
#define valvePin 5

bool WIFI_INCLUDED = true;

String ShowBoardInfo()
{
    String str = "INFOBOARD-VERSION" + String(VERSION) + "\r\n";
    str += "INFOPROJECT-NAME " + String(PROJECT) + "\r\n";
    str += "INFODATE-" + String(UPLOADDATE) + "\r\n";
    return str;
}

//test
/*#if !defined(ARDUINO_ARCH_AVR)
    HardwareSerial Serial1(1);
    HardwareSerial Serial2(2);          
#endif
*/

HardwareSerial &mpuCom = Serial1;
HardwareSerial &sensorCom = Serial2;
HardwareSerial &debugCom = Serial;

#include "./modules/Helper/DisplayLog.h"

int CH_ON = HIGH;
int CH_OFF = LOW;
String MCU_STATE = "NRDY";

struct timer_s
{
    uint16_t st;
    uint16_t en; // end or working depend CH_ON timer mode
};

struct sensor_s
{
    float soil;
    float vpd;
    float ec;
    float ph;
    float water;
    float temp;
    float humi;
    float co2;
    float light;
    boolean floating;
    float par;
};

#include "./modules/Control/Control.h"
Control *channel[CHANNEL_NUMBER];
//
int ChannelGpio[6] = {32, 33, 25, 26};
int ChannelStatus[6] = {LOW, LOW, LOW, LOW};

void DigitalWrite(int ch, int status)
{
    digitalWrite(ChannelGpio[ch], status);
    ChannelStatus[ch] = status;
}

//General module
#include "./modules/Memory/eeprom_manager.h"
// #include "./modules/DateTime.h"
#include "./modules/RTC.h"

#include "./modules/Sensors/Sensor.h"
#include "./modules/Sensors/ParAcc.h"
#include "./modules/Control/ControlFactory.h"
#include "./modules/ChannelHandler.h"
#include "./modules/Memory/MemoryCheck.h"

#include "./modules/Vmn/nodes.h"
#include "./modules/Communication.h"

#if !defined(WIFI_INCLUDED)
#include "./modules/Wifi/server.h"
#endif

void coreTask(void *pvParameters)
{
    String taskMessage = "Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
    while (true)
    {
        taskManager0.Loop();
    }
}

void setup()
{

#if !defined(WIFI_INCLUDED)
    taskManager0.StartTask(VmnServer::instance());
#endif

    Wire.begin();
    debugCom.begin(115200);
    mpuCom.begin(115200);
    sensorCom.begin(9600);
    debugCom.println("Initializing...");
    debugCom.println(ShowBoardInfo());
    mpuCom.println(ShowBoardInfo());
    mpuCom.println(sizeof(data_table_s));
    EEPROM_Manager::InitEEPROM();
    taskManager.StartTask(RTC::instance());
    taskManager.StartTask(Sensor::instance());
    taskManager.StartTask(ParAcc::instance());
    taskManager.StartTask(Communication::instance());

#if !defined(WIFI_INCLUDED)
    xTaskCreatePinnedToCore(
        coreTask,   /* Function to implement the task */
        "coreTask", /* Name of the task */
        10000,      /* Stack size in words */
        NULL,       /* Task input parameter */
        0,          /* Priority of the task */
        NULL,       /* Task handle. */
        0);
#endif

    //button

    ChannelHanler::instance();
    MCU_STATE = "RDY";
    mpuCom.println(MCU_STATE);
}

void loop()
{
    // Serial.println("test");
    taskManager.Loop();
}
