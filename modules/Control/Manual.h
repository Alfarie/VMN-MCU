#include <Task.h>
extern TaskManager taskManager;
class Manual : public Control
{
  public:
    Manual(int ch) : Control(1000, ch)
    {
        UpdateFromRom();
    };

    void SetValue(byte gpio_st)
    {
        gpio_status = gpio_st;
        SaveToRom();
    }
  private:
    byte gpio_status;
    virtual bool OnStart()
    {
        DigitalWrite(channel - 1, CH_OFF);
        return true;
    }
    virtual void OnUpdate(uint32_t delta_time)
    {
        //debugCom.println("Manual   is running " + String(channel) + " " + String(gpio_status));
        // digitalWrite( ChannelGpio[channel-1], gpio_status);
        // ChannelHanler::SetChannelStatus( channel - 1, gpio_status);
        DigitalWrite(channel - 1, gpio_status);
    }
    
    void UpdateFromRom()
    {
        gpio_status = rom_channel[channel - 1].manual.status;
    }
    
    void SaveToRom()
    {
        rom_channel[channel - 1].manual.status = gpio_status;
        EEPROM_Manager::Update(channel);
    }
};