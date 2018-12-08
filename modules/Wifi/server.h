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
    virtual bool OnStart() { return true; }
    virtual void OnUpdate(uint32_t delta_time)
    {

        WiFiClient client = server.available(); // listen for incoming clients

        if (client)
        {                                  // if you get a client,
            //mpuCom.println("New Client."); // print a message out the //mpuCom port
            String currentLine = "";       // make a String to hold incoming data from the client
            while (client.connected())
            { // loop while the client's connected
                if (client.available())
                {                           // if there's bytes to read from the client,
                    char c = client.read(); // read a byte, then
                    // //mpuCom.write(c);                    // print it out the //mpuCom monitor
                    if (c == '\n')
                    { // if the byte is a newline character

                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (currentLine.length() == 0)
                        {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();

                            // the content of the HTTP response follows the header:
                            client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
                            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");

                            // The HTTP response ends with another blank line:
                            client.println();
                            // break out of the while loop:
                            break;
                        }
                        else
                        { // if you got a newline, then clear currentLine:
                            currentLine = "";
                        }
                    }
                    else if (c != '\r')
                    {                     // if you got anything else but a carriage return character,
                        currentLine += c; // add it to the end of the currentLine
                    }

                    // Check to see if the client request was "GET /H" or "GET /L":
                    if (currentLine.endsWith("GET /H"))
                    {
                        digitalWrite(LED_BUILTIN, HIGH); // GET /H turns the LED on
                    }
                    if (currentLine.endsWith("GET /L"))
                    {
                        digitalWrite(LED_BUILTIN, LOW); // GET /L turns the LED off
                    }
                    // GET /val?st=0&ec=1.2&vol=300
                    if (currentLine.endsWith("HTTP/"))
                    {
                        //mpuCom.println(currentLine);
                        String queryStr = currentLine;
                        queryStr.replace("GET /vmndata?", "");
                        queryStr.replace(" HTTP/", "");
                        
                        int size = 3;
                        float data[size];
                        ExtractDataFloat(data, size, queryStr);
                        //mpuCom.println(String(data[0]) + "," + String(data[1]) + "," + String(data[2]));
                        int st = (int)data[0];
                        Nodes::nodes[st].setValue(data[1],data[2]);
                    }
                }
            }
            // close the connection:
            client.stop();
            //mpuCom.println("Client Disconnected.");
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