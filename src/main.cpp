/*
complie using
$ g++ -g -Wall src/main.cpp -lvirt -o out/main

run using
$ out/main
*/
#include <iostream>
#include <iomanip>
#include <libvirt/libvirt.h>

#define COL_SIZE_ID 6
#define COL_SIZE_NAME 31
#define COL_SIZE_STATE 15

int main() {
    // Create a connection
    virConnectPtr conn;
    conn = virConnectOpen("qemu:///system");
    
    // Check connection status
    if (conn == NULL) {
        std::cerr << "Connection failed\n";
        exit(1);
    }

    // Print title
    // std::cout << "\nHost name: " << virConnectGetHostname(conn) << "\n\n";

    // Get domains and number of domains
    virDomainPtr * domains;
    int numOfDomains = virConnectListAllDomains(conn,
					 &domains,
					 0); // No filters

    // Print table header
    std::cout << " "
        << std::left << std::setw(COL_SIZE_ID) << "Id"
        << std::left << std::setw(COL_SIZE_NAME) << "Name"
        << std::left << std::setw(COL_SIZE_STATE) << "State"
        << std::setw(0) << "\n";

    // Print separator
    for (int i = 0; i < COL_SIZE_ID + COL_SIZE_NAME + COL_SIZE_STATE; i++) {
        std::cout << std::setw(0) << "-";
    }
    std::cout << "\n";

    // Print domain list
    for (int i = 0; i < numOfDomains; i++) {
        int id;
        const char* name;
        const char *state;
        int stateId, reason;

        // Get parameters of interst
        name = virDomainGetName(domains[i]);
        id = virDomainGetID(domains[i]);
        virDomainGetState(domains[i], &stateId, &reason, 0);

        // Convert state value into a string
        switch (stateId) {
        case 0:
            state = "no state";
            break;
        case 1:
            state = "running";
            break;
        case 2:
            state = "blocked";
            break;
        case 3:
            state = "paused";
            break;
        case 4:
            state = "shutting down";
            break;
        case 5:
            state = "shut off";
            break;
        case 6:
            state = "crashed";
            break;
        case 7:
            state = "suspended";
            break;
        default:
            state = "unrecognized";
            break;
        }

        // Print result
        std::cout << " "
            << std::left << std::setw(COL_SIZE_ID) << id
            << std::left << std::setw(COL_SIZE_NAME) << name
            << std::left << std::setw(COL_SIZE_STATE) << state << std::endl;
        
        // Free memory
        virDomainFree(domains[i]);
    }
    std::cout << std::endl;

    // Free memory
    free(domains);

    // Close connection
    virConnectClose(conn);
    
    return 0;
}