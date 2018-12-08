#include <Task.h>
extern TaskManager taskManager;
class Waiting : public WaterProcess
{
public:
  Waiting(callback ns) : WaterProcess(1000, ns){};

 String GetStatus(){
    /*
      {
        type: water-fill,
        data: {
          crt: currentTime,
          max: 0
        }
      }
    */
    
    // return "{ \"type\": \"waterprocess-wait\", \"data\":{ \"crt\": " + String(currentTime) + ", \"max\": "+ String(waterProcess.waitingTime) +" }}" ;
    String data = "{st-water,wait," + String(currentTime) + "," + String(waterProcess.waitingTime) + "}";
    return data;
  }
private:
  virtual bool OnStart()
  {
    currentTime = 0;
    // DigitalWrite(5, CH_OFF);
    // DigitalWrite(6, CH_OFF);
    return true;
  }
  virtual void OnUpdate(uint32_t delta_time)
  {
    
    currentTime += (delta_time / 1000.0);
    if (currentTime > waterProcess.waitingTime)
    {
      currentTime = 0;
      nextState("waiting");
      taskManager.StopTask(this);
    }
    
  }
};