#include "mbed.h"
#include "EthernetInterface.h"
#include "root_ca_cert.h"

#define HOSTNAME "ifconfig.io"
#define PORT 443 

int main()
{
    // Initialize network interface:
    EthernetInterface eth;

    // Try to connect:
    nsapi_error_t status = eth.connect();
    if (status != 0) {
        printf("Could not configure ethernet - error code: %d\n", status);
        return 0;
    }

    printf("Interface configured successfully\n");

    // Create socket & open (allocate resources)
    TLSSocket sock;
    status = sock.open(&eth);
    if (status != 0) {
        printf("Could not open socket - error code: %d\n", status);
        return 1;
    }

    printf("socket opened successfully\n");

    // Set certificate
    status = sock.set_root_ca_cert(root_ca_cert);
    if (status == NSAPI_ERROR_PARAMETER) {
        printf("Error: _socket.set_root_ca_cert() returned %d\n", status);
        return 1; 
    }
    sock.set_hostname(HOSTNAME);

    // Connect
    SocketAddress a;
    eth.gethostbyname(HOSTNAME, &a);
    a.set_port(PORT);
    status = sock.connect(a);

    if (status != 0) {
        printf("Could not connect socket - error code: %d\n", status);
        return 1;
    }

    printf("Connected successfully\n");

    // Simple HTTP request
    char buffer[] = "GET / HTTP/1.1\r\n"
                              "Host: ifconfig.io\r\n"
                              "Connection: close\r\n"
                              "\r\n";
    nsapi_size_t bytes_to_send = strlen(buffer);
    
    status = sock.send(buffer, bytes_to_send);
    if (status < 0) {
        printf("Could not send data - error code: %d\n", status);
        return 1;
    }

    char buf[100];
    sock.recv(buf, 100);

    status = sock.close();
    if (status < 0) {
        printf("Could not receive data - error code: %d\n", status);
        return 1;
    }

    printf("%s\n", buf);

    while (true) {
       // do nothing now 
    }
}

