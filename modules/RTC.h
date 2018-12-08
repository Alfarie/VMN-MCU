#include <Task.h>
#include <Wire.h>
extern TaskManager taskManager;

#include "RTClib.h"


RTC_DS1307 hrtc;
RTC_Millis srtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

class RTC : public Task
{
  public:
    static RTC *s_instance;
    RTC() : Task(MsToTaskTime(1000))
    {
        
    };
    
    static RTC *instance()
    {
        if (!s_instance)
            s_instance = new RTC;
        return s_instance;
    }

    int GetCurrentMin()
    {
        int min = (now.hour() * 60) + now.minute();
        return min;
    }

    String GetDateString()
    {

        String str = String(now.year()) + "-" +
                     AddZero(now.month()) + String(now.month()) + "-" +
                     AddZero(now.day()) + String(now.day());
        return str;
    }

    String GetTimeString()
    {
        String str = AddZero(now.hour()) + String(now.hour()) + ":" +
                     AddZero(now.minute()) + String(now.minute()) + ":" +
                     AddZero(now.second()) + String(now.second());
        return str;
    }

    String GetDateTimeString()
    {
        String data = "{st-datetime," + GetDateString() + "," + GetTimeString() + "}";
        return data;
    }

    void setDateDs1307(byte s, byte m, byte h, byte dow, byte dom, byte mo, byte y)
    {
        hrtc.adjust(DateTime(y, mo, dom, h, m, s));
        srtc.adjust(DateTime(y, mo, dom, h, m, s));
    }

  private:
    DateTime now, temp;
    virtual bool OnStart()
    {
        srtc.adjust(DateTime(2018, 1, 1, 0, 0, 0));
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
        // mpuCom.println(hrtc.isrunning());
        if(hrtc.isrunning()){
        // if(false){
            now = hrtc.now();
            // mpuCom.println(GetDateTimeString() + " HardRun");
            if(now.day() <= 31){
                SRTCUpdate();
            }
        }
        else{
            // mpuCom.println(GetDateTimeString() + " SoftRun");
            now = srtc.now();
        }
                      
    }

    void SRTCUpdate(){
        srtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()));
    }

    String AddZero(byte val)
    {
        if (val < 10)return "0";
        else return "";
    }
};
RTC *RTC::s_instance = 0;