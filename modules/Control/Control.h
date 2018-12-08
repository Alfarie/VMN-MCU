#include <Task.h>
extern TaskManager taskManager;
class Control : public Task
{
  public:
    Control(int ms,int ch) : Task(MsToTaskTime(ms))
      {
        channel = ch;
        currentTime = 0;
      };

  protected:
    int channel;
    float currentTime;
    virtual void SetValue()
    {
      
    }
  private:
    
    virtual bool OnStart()
    {
      return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
      
    }
};