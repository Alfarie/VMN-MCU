#include <Task.h>
extern TaskManager taskManager;

#include "./Manual.h"
#include "./Timer.h"
// #include "./Irrigation/irrigationFactory.h"
#include "./AdvanceSetpoint.h"
#include "./AdvanceSetbound.h"
#include "./AdvanceSetboundTiming.h"

class ControlFactory 
{
  public:
    static Control * instance(String type, byte channel)
    {
        // (0)Manual, (1)timer, (2)setpoint, (3)setbound, (4)irrigation, (5)misting
        if(type == "manual")
            return new Manual(channel);
        else if(type == "timer")
            return new Timer(channel);
        else if(type == "advcond") 
            return new AdvanceSetpoint(channel);
        else if(type == "advsbt") 
            return new AdvanceSetboundTiming(channel);
        else if(type == "advsb") 
            return new AdvanceSetbound(channel);
        else
            return new Manual(channel);
    }
};