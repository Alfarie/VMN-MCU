#include <Task.h>
extern TaskManager taskManager;

class FillWater : public WaterProcess
{
public:
  FillWater(callback ns) : WaterProcess(1000, ns)
  {
    // pinMode(FT_SENSOR, INPUT);
  };

  String GetStatus()
  {
    /*
      {
        type: water-fill,
        data: {
          crt: currentTime,
          max: 0
        }
      }
    */
    // return "{ \"type\": \"waterprocess-fill\", \"data\":{ \"crt\": " + String(currentTime) + ", \"max\": 0 }}";
    String data = "{st-water,fill," + String(currentTime) + ",0}";
    return data;
  }

private:
  float flotingTime = 0;
  virtual bool OnStart()
  {

    int ft_state = Solution::instance()->GetFloating();
    currentTime = 0;
    flotingTime = 0;
    if (ft_state == 1)
    {
      nextState("fill water");
      return false;
    }

    //testCom.println("isFill:" + String(waterProcess.isFill));
    if (!waterProcess.isFill)
    {
      nextState("fill water");
      return false;
    }

    DigitalWrite(5, CH_ON);
    return true;
  }
  virtual void OnUpdate(uint32_t delta_time)
  {

    currentTime += (delta_time / 1000.0);
    //testCom.println(currentTime);
    
    if (!waterProcess.isFill)
    {
      DigitalWrite(5, CH_OFF);
      flotingTime = 0;
      nextState("fill water");
      taskManager.StopTask(this);
    }
    else if (Solution::instance()->GetFloating() == 1)
    {
      flotingTime += (delta_time / 1000.0);
      if (flotingTime >= 3.0)
      {
        DigitalWrite(5, CH_OFF);
        flotingTime = 0;
        nextState("fill water");
        taskManager.StopTask(this);
      }
    }
    else
    {
      flotingTime = 0;
    }
  }
};