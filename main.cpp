#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define popen _popen
#define pclose _pclose

// Author: Imran Memon
// License: MIT
// Email: javascriptisthebestlanguage@gmail.com
// I tortured myself just so you can have a wifi password Retrieval tool, MF, AND I SPENT LIKE 3 HOURS HELP

// Function to execute a system command and return the output as a string
std::string executeCommand(const char* cmd) {
    char buffer[128]; // This string stores the command
    std::string result = "";
    FILE* cmdFile = _popen(cmd, "r"); // Opens a pipe to the command and creates a file
    std::string input; // Convert the buffer command into a string to be executed
    if (cmdFile) {
        // Read the command output into the result string
        while (fgets(buffer, sizeof(buffer), cmdFile) != NULL) {
            result += buffer;
        }
        _pclose(cmdFile);
    }
    return result;
}

// Why am I doing this for you
// I'm not explaining the rest of that function, good luck MFS, just look down for better explanations
// We need to retrieve the SSID so netsh will know what to get the password for, so that's why we are referencing this SSID variable, along with a parameter to call later.
std::string getPassword(const std::string& ssid) {
    // Create the netsh command to show the profile key for the specified SSID
    std::string command = "netsh wlan show profile \"" + ssid + "\" key=clear";
    std::stringstream ss(executeCommand(command.c_str()));
    std::string input;
    while (std::getline(ss, input)) {
        // Check if the line contains "Key Content"
        if (input.find("Key Content") != std::string::npos) {
            // Return the password substring starting from position 29
            return input.substr(29, input.length());
        }
    }
    return "< NULL >"; // Return a placeholder if the password is not found
}

// The SSID is a string, mfs
// Yea MFS, I told you this is for Windows!!!! This is literally using netsh!!!!!
std::string getSsid() {
    std::stringstream ss(executeCommand("netsh wlan show interfaces | findstr SSID").data()); // Make a string stream variable for manipulation that stores the finding SSID command
    std::string input; // Input string to help get the SSID, using the getline() command and with the stringstream
    // Find the separator between SSID and SSID:, usually in the middle
    while (std::getline(ss, input)) { // Get the SSID while those variables exist
        // This will help separate unwanted stuff, like the SSID text (context: By SSID text, I mean like it says SSID: then the SSID) when running netsh

        if (input.find("SSID") != std::string::npos) { // Make sure SSID is not a NULL string, and see why we used pos? we got to separate that sh
            size_t pos = input.find(":");
            return input.substr(pos + 2); // Go by 4 spaces, that's where the SSID (Wi-Fi name) is located
            // Why does C++ have to make string return a value ):, this is torture
        }
    }
}

// Yay, I'm done torturing my hands with the SSID

int main() {
    // First, get the SSID
    std::string ssid = getSsid(); // In case you're dumb as hell, THE SSID IS A STRING
    std::cout << "SSID: " << ssid << std::endl;

    if (!ssid.empty()) {


        // Get and print the password for the SSID
        std::string password = getPassword(ssid);

        if (!password.empty()) {
            std::cout << "Password: " << password << std::endl;
        }
        else {
            std::cerr << "Failed to retrieve password." << std::endl;
        }
    }
    else {
        std::cerr << "Failed to retrieve SSID." << std::endl;
    }

    return 0;
}

