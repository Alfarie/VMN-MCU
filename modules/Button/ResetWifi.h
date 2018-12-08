#include <Task.h>
extern TaskManager taskManager;
#include "./ledRed.h"
class ResetWifi : public Task
{
  public:
    static ResetWifi *s_instance;
    ResetWifi() : Task(MsToTaskTime(1000)) {}
    static ResetWifi *instance()
    {
        if (!s_instance)
            s_instance = new ResetWifi;
        return s_instance;
    }

  private:
    int wifiResetPin = 34, ledRed = 4;
    int currentTime = 0;
    int state = 0;
    virtual bool OnStart()
    {
        pinMode(wifiResetPin, INPUT);

        pinMode(ledRed, OUTPUT);
        digitalWrite(ledRed, LOW);
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
        int bt = digitalRead(wifiResetPin);

        if (bt == 0)
        {
            if (state == 0)
            {
                currentTime += delta_time;
                if (currentTime > 5000)
                {
                    taskManager.StartTask(LedRed::instance());
                    mpuCom.println("WIFI-RST");
                    currentTime = 0;
                }
            }
        }
    }
};
ResetWifi *ResetWifi::s_instance = 0;