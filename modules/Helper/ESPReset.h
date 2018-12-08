#include <Task.h>
extern TaskManager taskManager;
class ESPReset : public Task
{
  public:
    static ESPReset *s_instance;
    ESPReset() : Task(MsToTaskTime(1000)){};
    static ESPReset *instance()
    {
      if (!s_instance)
      s_instance = new ESPReset;
      return s_instance;
    }

  private:
    float currentTime;
    virtual bool OnStart()
    {
        currentTime = 0;
        return true;

    }
    virtual void OnUpdate(uint32_t delta_time)
    {
        currentTime += (delta_time/1000.0);
        debugCom.println("ResetTime : " + String(currentTime));
        if(currentTime >= 20){
            #if !defined(ARDUINO_ARCH_AVR)
                 ESP.restart();
            #endif
            currentTime = 0;
        }
    }
};
ESPReset *ESPReset::s_instance = 0;