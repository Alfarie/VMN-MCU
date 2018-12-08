#include <Task.h>
#include "./node.h"
extern TaskManager taskManager;

class Nodes : public Task
{
  public:
    static Node *nodes;
    static Nodes *s_instance;
    Nodes() : Task(MsToTaskTime(1000)) {
        
    }

    static Nodes *instance()
    {
        if (!s_instance)
            s_instance = new Nodes;
        return s_instance;
    }

    static String getString(){
        String str = "{st-nodes,";
        for(int i = 0; i < 9; i++){
            if( i < 8)
                str += nodes[i].getString() + ",";
            else str += nodes[i].getString() + "}";
        }
        return str;
    }

  private:
    virtual bool OnStart() { return true; }
    virtual void OnUpdate(uint32_t delta_time) {
        
    }
};
Nodes *Nodes::s_instance = 0;
Node *Nodes::nodes = new Node[9]{{0},{1},{2},{3},{4},{5},{6},{7},{8}};