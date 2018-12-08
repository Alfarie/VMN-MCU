#include <Task.h>
extern TaskManager taskManager;
class AIIrrigation : public Irrigation
{
  public:
    AIIrrigation(int ch) : Irrigation(ch){
        currentPar = 0;
        UpdateFromRom();
    };

  private:
    float parAcc, workingTime;
    double currentPar, maxPar;

    virtual bool OnStart()
    {
        mpuCom.println("INFO-PURE-IRRIGATION-START");
        DigitalWrite(channel - 1, CH_OFF);
        maxPar = parAcc * 1000000.0;
        return true;
    }

    virtual void OnUpdate(uint32_t delta_time)
    {
        if( checkParAcc() ){
            working(delta_time);
        }

        // mpuCom.println(String(currentPar) + "," + String(maxPar)+ "," + String(currentTime));
    }

    void working(uint32_t delta_time){
        currentTime += (delta_time/1000);
         DigitalWrite(channel - 1, CH_ON);
        if(currentTime >= workingTime){
             DigitalWrite(channel - 1, CH_OFF);
             currentPar = 0;
             currentTime = 0;
        }
    }

    bool checkParAcc()
    {
        float par = Sensor::instance()->GetSensor(2);
        currentPar += par;
        accumulatePar(currentPar);
        if(currentPar >= maxPar){
            return true;
        }
        else return false;
    }

    

    void UpdateFromRom()
    {
        parAcc = rom_channel[channel - 1].irrigation.par_acc;
        workingTime = rom_channel[channel - 1].irrigation.par_working;
    }
};
