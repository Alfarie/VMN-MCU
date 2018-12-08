#include <Task.h>
extern TaskManager taskManager;
class Irrigation : public Control
{
public:
  Irrigation(int ch) : Control(1000, ch){};

  void accumulatePar(float currentPar)
  {
   
    ParAcc::parChAcc[channel - 1] = currentPar;
    //  mpuCom.println(String(currentPar) + "," + String({Gparacc}));
  }

private:
  virtual bool OnStart() { return true; }
  virtual void OnUpdate(uint32_t delta_time)
  {
  }
};