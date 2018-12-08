class EEPROM_ADDR
{
public:
  static const int EC_SETPOINT = sizeof(float) * 0;
  static const int EC_DETECTING = sizeof(float) * 1;
  static const int EC_WORKING = sizeof(float) * 2;
  static const int EC_CAL = sizeof(float) * 3;

  static const int PH_SETPOINT = sizeof(float) * 4;
  static const int PH_DETECTING = sizeof(float) * 5;
  static const int PH_WORKING = sizeof(float) * 6;
  static const int PH_CAL = sizeof(float) * 7;

  static const int CO2_SETPOINT = sizeof(float) * 8;
  
  static const int ISINIT_EEPROM = 50 + sizeof(int) * 0;
  static const int CO2_CONTROL = ISINIT_EEPROM + sizeof(int) * 1;
  static const int PH_CONTROL = ISINIT_EEPROM + sizeof(int) * 2;
  static const int EC_CONTROL = ISINIT_EEPROM + sizeof(int) * 3;
  static const int CIRCULATING = ISINIT_EEPROM + sizeof(int) * 4;
  static const int FILL_WATER = ISINIT_EEPROM + sizeof(int) * 5;
  static const int COOKING = ISINIT_EEPROM + sizeof(int) * 6;

  static const int LED_TIMER_ADDR_ST = 100;
  static const int LED_TIMER_SIZE = LED_TIMER_ADDR_ST - sizeof(int);
  static const int LED_MODE = LED_TIMER_ADDR_ST - sizeof(int) * 2;   //led mode [1]timer  [2]manual
  static const int LED_SW = LED_TIMER_ADDR_ST - sizeof(int) * 3; //led status [1]CH_ON [2] CH_OFF

  static const int PUMP_PERIOD = 210;
  static const int PUMP_WORKING = PUMP_PERIOD - sizeof(int);
  static const int PUMP_MODE = PUMP_PERIOD - sizeof(int) * 2;   //PUMP mode [1]timer  [2]manual
  static const int PUMP_STATUS = PUMP_PERIOD - sizeof(int) * 3; //led status [1]CH_ON [2] CH_OFF
};