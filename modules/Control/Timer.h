#include <Task.h>

extern TaskManager taskManager;
class Timer : public Control
{
public:
  Timer(int ch) : Control(1000, ch)
  {
    UpdateFromRom();
  };

  void SaveValue(timer_s *t, int s, int m)
  {
    ArrayTimerCopy(timer_list, t, s);
    size = s;
    mode = m;
    SaveToRom();
  }

private:
  timer_s timer_list[8];
  byte size;
  byte mode;
  byte state = 0;
  float crt = 0, working = 10, crtMin = 0;
  virtual bool OnStart()
  {
    DigitalWrite(channel - 1, CH_OFF);
    return true;
  }
  
  virtual void OnUpdate(uint32_t delta_time)
  {
    int currentMin = RTC::instance()->GetCurrentMin();
    if (mode == 0)
    {
      bool flag = false;
      for (int i = 0; i < size; i++)
      {
        if (currentMin >= timer_list[i].st && currentMin <= timer_list[i].en)
        {
          flag = true;
          break;
        }
      }
      if (flag)
      {
        DigitalWrite(channel - 1, CH_ON);
      }
      else
      {
        DigitalWrite(channel - 1, CH_OFF);
      }
    }

    else if (mode == 1)
    {
      bool flag = false;
      //not yet
      if (state == 0)
      {
        for (int i = 0; i < size; i++)
        {
          if (currentMin == timer_list[i].st)
          {
            flag = true;
            state = 1; // doing
            working = (float)timer_list[i].en;
            crtMin = (float)timer_list[i].st;
            DigitalWrite(channel - 1, CH_ON);
            break;
          }
        }
      }
      else if(state == 1){
        crt += (delta_time/1000.0);
        if(crt >= working){
          DigitalWrite(channel - 1, CH_OFF);
          state = 3;
          crt = 0;
        }
      }
      else if(state == 3){
        if(currentMin > crtMin){
          state = 0;
        }
      }
    }
  }
  void ArrayTimerCopy(timer_s *from, timer_s *to, int size)
  {
    int i = 0;
    for (i = 0; i < size; i++)
    {
      to[i] = from[i];
    }
  }

  void UpdateFromRom()
  {
    ArrayTimerCopy(rom_channel[channel - 1].timer.timer_list, timer_list, rom_channel[channel - 1].timer.size);
    mode = rom_channel[channel - 1].timer.mode;
    size = rom_channel[channel - 1].timer.size;
  }

  void SaveToRom()
  {
    ArrayTimerCopy(timer_list, rom_channel[channel - 1].timer.timer_list, rom_channel[channel - 1].timer.size);
    rom_channel[channel - 1].timer.mode = mode;
    rom_channel[channel - 1].timer.size = size;
  }
};