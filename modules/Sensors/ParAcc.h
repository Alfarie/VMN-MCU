
#include <Task.h>
extern TaskManager taskManager;
class ParAcc : public Task
{
  public:
    static double parChAcc[4];
    static ParAcc *s_instance;
    ParAcc() : Task(MsToTaskTime(1000)){
        parAcc = 0;
        maxPar = 1000000.0;
        currentPar = 0;
    };
    static ParAcc *instance()
    {
      if (!s_instance)
      s_instance = new ParAcc;
      return s_instance;
    }
    float GetParAcc(){
        return parAcc;
    }
  private:
    float parAcc;
    float maxPar;
    float currentPar;
    virtual bool OnStart()
    {
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
        // par : watt/sqm
        currentPar = Sensor::instance()->GetSensor(2);
        parAcc += currentPar;
        if(parAcc >= maxPar ){
            parAcc = 0;
        }
        // String str = "ParAcc: " + String (currentPar) + ":" + String(parAcc) + "/" + String(maxPar);
        // mpuCom.println(str);
    }
};
ParAcc *ParAcc::s_instance = 0;
double ParAcc::parChAcc[4] = {0,0,0,0};