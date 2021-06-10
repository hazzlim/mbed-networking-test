#include "mbed.h"
#include "EthernetInterface.h"

int main()
{
    // Initialize network interface:
    EthernetInterface eth;

    // Try to connect:
    nsapi_error_t status = eth.connect();

    if (status) {
        printf("Could not configure ethernet - error code: %d\n", status);
    } else {
         printf("Interface configured successfully\n");
    }

    while (true) {
       // do nothing now 
    }
}

