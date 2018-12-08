#include <Task.h>
extern TaskManager taskManager;
class BeforeStateChanged : public WaterProcess
{
  public:
    BeforeStateChanged(callback ns) : WaterProcess(1000,ns){};
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
    // return "{ \"type\": \"waterprocess-bsch\", \"data\":{ \"crt\": " + String(currentTime) + ", \"max\": "+ String(waitingTime) +" }}" ;
    String data = "{st-water,bsch," + String(currentTime) + "," + String(waitingTime) + "}";
    return data;
  }
  private:
    float waitingTime = 5;
    virtual bool OnStart()
    {
        currentTime = 0;
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
      //  //testCom.println(GetStatus());
        currentTime += (delta_time/1000.0);
        if(currentTime >= waitingTime){
            nextState("bsch");
            taskManager.StopTask(this);
        }
    }
};