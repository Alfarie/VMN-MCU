#include <Task.h>
extern TaskManager taskManager;
#include "./CirWater.h"
#include "./Waiting.h"
#include "./FillWater.h"
#include "./BeforeStateChanged.h"
class WaterProcessControl : public Task
{
public:
  static WaterProcessControl *s_instance;
  static int state;

  static String GetControl()
  {
    String data = "{ct-water," + String(waterProcess.isCir) +
                  "," + String(waterProcess.isFill) +
                  "," + String(waterProcess.cirTime) +
                  "," + String(waterProcess.waitingTime) + "}";
    return data;
    /*
      String str = "{\"type\": \"water-control\", \"data\":{ \"isCir\": "  + String(waterProcess.isCir) + 
                    ",\"isFill\": " + String(waterProcess.isFill) + 
                    ",\"cirTime\": " + String(waterProcess.cirTime) + 
                    ",\"waitTime\": " + String(waterProcess.waitingTime) + "}}";
      return str;
      */
  }

  static void NextState(String message)
  {
    // testCom.println("Call from:" + message);
    // testCom.println("stop state:" + String(state));
    state++;
    if (state > 5)
    {
      state = 0;
    }
    //testCom.println("start state:" + String(state));
    taskManager.StartTask(wt_process[state]);
  }

  WaterProcessControl() : Task(MsToTaskTime(100))
  {
    s_fillWater = new FillWater(WaterProcessControl::NextState);
    s_cirWater = new CirWater(WaterProcessControl::NextState);
    s_waiting = new Waiting(WaterProcessControl::NextState);
    s_bch = new BeforeStateChanged(WaterProcessControl::NextState);
    s_bch2 = new BeforeStateChanged(WaterProcessControl::NextState);

    wt_process[0] = s_fillWater;
    wt_process[1] = s_cirWater;
    wt_process[2] = s_bch;
    wt_process[3] = s_fillWater;
    wt_process[4] = s_waiting;
    wt_process[5] = s_bch2;
  };

  static WaterProcessControl *instance()
  {
    if (!s_instance)
      s_instance = new WaterProcessControl;
    return s_instance;
  }

  static String GetStatus()
  {
    return wt_process[state]->GetStatus();
  }

private:
  FillWater *s_fillWater;
  CirWater *s_cirWater;
  Waiting *s_waiting;
  BeforeStateChanged *s_bch, *s_bch2;

  virtual bool OnStart()
  {
    taskManager.StartTask(wt_process[0]);
    return true;
  }
  virtual void OnUpdate(uint32_t delta_time)
  {
    // //testCom.println(WaterProcessControl::GetStatus());
    // for(int i = 0 ; i < CHANNEL_NUMBER ;i++){
    //   testCom.print(" " + String(ChannelStatus[i]) + " " );
    // }
    // //testCom.println();
    // //testCom.println(state);
  }
};
WaterProcessControl *WaterProcessControl::s_instance = 0;
int WaterProcessControl::state = 0;