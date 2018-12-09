#include <Task.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = "vmnnetwork";
const char *password = "raspberry";

WiFiServer server(80);

extern TaskManager taskManager;
class VmnServer : public Task
{
  public:
    static VmnServer *s_instance;
    VmnServer() : Task(MsToTaskTime(10))
    {
        //mpuCom.println();
        //mpuCom.println("Configuring access point...");

        // You can remove the password parameter if you want the AP to be open.

        WiFi.softAP(ssid, password);
        IPAddress myIP = WiFi.softAPIP();
        //mpuCom.print("AP IP address: ");
        //mpuCom.println(myIP);
        server.begin();
        //mpuCom.println("VmnServer started");
    }
    static VmnServer *instance()
    {
        if (!s_instance)
            s_instance = new VmnServer;
        return s_instance;
    }

  private:
    String cmdStr;
    char res[100];
    int size, cmdNumber, cmdSize;
    virtual bool OnStart() { return true; }
    virtual void OnUpdate(uint32_t delta_time)
    {
        WiFiClient client = server.available(); // listen for incoming clients
        if (client)
        { // if you get a client,
            // debugCom.println("New Client connected"); // print a message out the //mpuCom port
            String currentLine = ""; // make a String to hold incoming data from the client
            while (client.connected())
            { // loop while the client's connected
                if (client.available())
                {

                    if (client.read() == '{')
                    {
                        int size = 0;
                        while (true)
                        {
                            if (client.available())
                            {
                                char ch = client.read();
                                if (ch == '}')
                                {
                                    client.print("success");
                                    client.stop();
                                    res[size] = '\0';
                                    String strRes = res;
                                    debugCom.println(strRes);
                                    float data[3];
                                    ExtractDataFloat(data, 3, strRes);
                                    int st = (int)data[0];
                                    Nodes::nodes[st].setValue(data[1], data[2]);
                                    debugCom.print("Recieved data from station: " + String(st));
                                    debugCom.println(", ec:" + String(data[1]) + "," + String(data[2]));
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
            
        }
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
VmnServer *VmnServer::s_instance = 0;