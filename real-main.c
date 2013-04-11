//Initializes the boards peripherals and nothing more.

//Local includes:
#include <libmaple/libmaple.h>
#include "initialize.h"



// setup() and loop():
void setup(void) {
    
}

void loop(void) {
    
}


__attribute__((constructor)) void premain() {
    init();
}

int main(void) {
    setup();
    
    while (1) {
        loop();
    }
    
    return 0;
}
