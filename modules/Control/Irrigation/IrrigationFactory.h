#include "./irrigation.h"
#include "./ai-irrigation.h"
#include "./pure-irrigation.h"
#include "./soil-par-irrigation.h"
class IrrigationFactory {
    public:
        static Control  * instance(String control, int ch){
            if(control == "ai"){
                new AIIrrigation(ch);
            }
            else if(control == "pure"){
                return new PureIrrigation(ch);
            }
            else if(control == "soil-par"){
                return new SoilParIrrigation(ch);
            }
        }
};