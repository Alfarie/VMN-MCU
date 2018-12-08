#include <Task.h>
extern TaskManager taskManager;
class WaterProcess : public Task
{
  public:
    typedef void (*callback)(String str);
    WaterProcess(int ms,callback ns) : Task(MsToTaskTime(ms)),
                                        nextState(ns){};
    
    virtual String GetStatus(){
      return status;
    }
  protected: 
    float currentTime = 0 ;
    String status;
    callback nextState;
    
  private:
    virtual bool OnStart()
    {
        
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
    
    }
};