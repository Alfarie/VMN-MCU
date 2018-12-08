#include <Task.h>
extern TaskManager taskManager;




class Sensor : public Task
{
public:
  static Sensor *s_instance;
  Sensor() : Task(MsToTaskTime(200))
  {
    sensor.temp = 0;
    sensor.humi = 0;
    sensor.soil = 0;
    sensor.par = 1000;
    sensor.vpd = 0;
    sensor.co2 = 0;
  };
  static Sensor *instance()
  {
    if (!s_instance)
      s_instance = new Sensor;
    return s_instance;
  }

  sensor_s GetSensors()
  {
    return sensor;
  }
  // String GetSensorsString()
  // {
  //   String data = "{st-sensors," + String(sensor.soil) +
  //                 "," + String(sensor.temp) +
  //                 "," + String(sensor.humi) +
  //                 "," + String(sensor.vpd) +
  //                 "," + String(sensor.par) +
  //                 "," + String(sensor.co2) +
  //                 "," + String(ParAcc::instance()->GetParAcc() ) + "}";
  //   return data;
  // }

  float GetSensor(int num)
  {
    float value = 0.0f;
    switch (num)
    {
    case 0: // 0:vpd, 1:soil, 2:par, 3:temp, 4:humi, 5:co2
      value = sensor.vpd;
      break;
    case 1:
      value = sensor.soil;
      break;
    case 2:
      value = sensor.par;
      break;
    case 3:
      value = sensor.temp;
      break;
    case 4:
      value = sensor.humi;
      break;
    case 5:
      value = sensor.co2;
      break;
    default:
      value = -1;
    }
    return value;
  }

private:
  sensor_s sensor;
  String cmdStr;
  String sensorStr = "0.00,0.00,0.0,0,0";
  char res[100];
  int size, cmdNumber, cmdSize;
  virtual bool OnStart()
  {

    return true;
  }
  virtual void OnUpdate(uint32_t delta_time)
  {
    // sensor.temp = humidity.GetTemperatureC();
    // sensor.humi = humidity.GetHumidity();
    // sensor.soil = random(49, 51);
    // sensor.par = random(1000, 1100);
    // sensor.vpd = 1500;

#if defined(ARDUINO_ARCH_AVR)
    int increase = digitalRead(6);
    int decrease = digitalRead(7);

    float ival = 0.01;
    if (increase == HIGH)
    {
      sensor.temp += ival;
      sensor.humi += ival;
      sensor.soil += ival;
      sensor.par += ival;
      sensor.vpd += ival;
    }
    if (decrease == HIGH)
    {
      sensor.temp -= ival;
      sensor.humi -= ival;
      sensor.soil -= ival;
      sensor.par -= ival;
      sensor.vpd -= ival;
    }
#endif

    while (sensorCom.available())
    {
      if (sensorCom.read() == '{')
      {
        int size = 0;
        while (true)
        {
          if (sensorCom.available())
          {
            char ch = sensorCom.read();
            if (ch == '}')
            {
              res[size] = '\0';
              String resData = res;
              float s[7];
              ExtractDataFloat(s, 7, resData);
              //25.00 50.00 50.00 60.00 1583.54
              sensor.temp = s[0];
              sensor.humi = s[1];
              sensor.par = s[2];
              sensor.soil = s[3];
              sensor.vpd = s[4];
              sensor.co2 = s[5];
              debugCom.println(PrintSensor());
              break;
            }
            else
            {
              res[size] = ch;
              size++;
            }
          }
        }
      }
    }
  }

  String PrintSensor()
  {
    String str = String(sensor.temp) + " " + String(sensor.humi) + " " + String(sensor.par) + " " + String(sensor.vpd) + " " + String(sensor.soil) + " " + String(sensor.co2);
    return str;
  }

  void ExtractDataFloat(float *data, int size, String res)
  {
    int i = 0, si = 0, ei, j = 0;
    while (j < size)
    {
      int index = res.indexOf(",");
      String a = res.substring(0, index);
      data[j] = a.toFloat();
      si = index;
      res = res.substring(index + 1);
      j++;
    }
  }
};
Sensor *Sensor::s_instance = 0;