#include <Task.h>
extern TaskManager taskManager;
extern int  __bss_end;
extern int  *__brkval;
class MemoryCheck : public Task
{
public:
  static MemoryCheck *s_instance;
  MemoryCheck() : Task(MsToTaskTime(3000)){};
  static MemoryCheck *instance()
  {
    if (!s_instance)
      s_instance = new MemoryCheck;
    return s_instance;
  }

  int GetFreeMemory(){
    return mem;
  }

private:
  int mem;
  virtual bool OnStart()
  {
    #if defined(ARDUINO_ARCH_AVR)
      return true;
    #else
      return false;
    #endif
  }
  virtual void OnUpdate(uint32_t delta_time)
  {
    #if defined(ARDUINO_ARCH_AVR)
      mem = freemem();
      //debugCom.println("[PlantLab] Free Memory: " + String(mem));
    #endif
  }

  #if defined(ARDUINO_ARCH_AVR)
  int freemem()
  {
    int free_memory;
    if ((int)__brkval == 0)
      free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
  }
  #endif

};
MemoryCheck *MemoryCheck::s_instance = 0;