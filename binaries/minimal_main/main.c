#include "api.h"
#include "sdk.c"

// todo: this should be a example that you can use a main binary.
// the main binary handels parsing and executing the scripts inside a gme file
// this is a bunch of logic and this is a try to implement this


// CURRENTLY THIS CODE DOESN'T WORK!

// fpAkOidPara + 0x18 first used oid
// fpAkOidPara + 0x1a last used oid

void main(system_api *apiPara) {
    initTT(apiPara);
    if (api->fpAkOidPara[First_time_exec] == 0) {
        api->fpAkOidPara[First_time_exec] = 1;
        // put your init code here
        
    }
    // put your loop code here

}