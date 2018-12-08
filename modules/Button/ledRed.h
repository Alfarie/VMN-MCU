#include <Task.h>
extern TaskManager taskManager;
class LedRed : public Task
{
  public:
    static LedRed *s_instance;
    LedRed() : Task(MsToTaskTime(100)) {}
    static LedRed *instance()
    {
        if (!s_instance)
            s_instance = new LedRed;
        return s_instance;
    }

  private:
    int pin = 4;
    int currentTime = 0;
    int state = HIGH;
    virtual bool OnStart()
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, state);
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
        digitalWrite(pin, state = !state);
        currentTime += delta_time;
        if(currentTime >= 2000){
            digitalWrite(pin, LOW);
            currentTime = 0;
            taskManager.StopTask(this);
        }
    }
};
LedRed *LedRed::s_instance = 0;