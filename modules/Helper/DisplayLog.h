#include <Task.h>
extern TaskManager taskManager;
class DisplayLog
{
  public:
    static DisplayLog *s_instance;
    static DisplayLog *instance()
    {
      if (!s_instance)
      s_instance = new DisplayLog;
      return s_instance;
    }
    
    static void PrintError(String msg)
    {
        //debugCom.println("[Error] " + msg);
    }
    static void PrintInfo(String msg)
    {
        //debugCom.println("[Info] " + msg);
    }
};
DisplayLog *DisplayLog::s_instance = 0;