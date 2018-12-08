
class Node
{
  public:
    Node(int ch){
        channel = ch;
        ec = 0;
        drain = 0;
    }

    String getString()
    {
        return String(channel) +"-" + String(ec) + "-" +  String(drain);
    }
    
    void setValue(float pec, float pdrain){
        ec = pec;
        drain = pdrain;
    }

  private:
    int channel;
    float ec, drain;
};