
class Puppet
{
  public:
    static Puppet *s_instance;
    Puppet(){
        pinMode(52,OUTPUT);
        digitalWrite(52,HIGH);

        
    };
    static Puppet *instance()
    {
      if (!s_instance)
      s_instance = new Puppet;
      return s_instance;
    }
};
Puppet *Puppet::s_instance = 0;