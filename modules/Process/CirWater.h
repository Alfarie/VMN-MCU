#include <Task.h>
extern TaskManager taskManager;
class CirWater : public WaterProcess
{
public:
  CirWater(callback ns) : WaterProcess(1000, ns){};

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
    // return "{ \"type\": \"waterprocess-cir\", \"data\":{ \"crt\": " + String(currentTime) + ", \"max\": "+ String(waterProcess.cirTime) +" }}" ;
    String data = "{st-water,cir," + String(currentTime) + "," + String(waterProcess.cirTime) + "}";
    return data;
  }

private:
  virtual bool OnStart()
  {
    if (!waterProcess.isCir)
    {
      nextState("cir");
      return false;
    }
    //testCom.println("cir water: start");
    currentTime = 0;
    DigitalWrite(4, HIGH);
    return true;
  }
  virtual void OnUpdate(uint32_t delta_time)
  {
    currentTime += (delta_time / 1000.0);


    if (!waterProcess.isCir)
    {
      currentTime = 0;
      DigitalWrite(4, LOW);
      nextState("cir");
      taskManager.StopTask(this);
    }
    else if (currentTime >= waterProcess.cirTime)
    {
      currentTime = 0;
      DigitalWrite(4, LOW);
      nextState("cir");
      taskManager.StopTask(this);
    }
  }
};