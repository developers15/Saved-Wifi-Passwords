
// Author: Imran Memon
// License: MIT
// Email: javascriptisthebestlanguage@gmail.com
// I tortured myself just so you can have a wifi password Retrival tool, MF, AND I SPENT LIKE 3 HOURS HELP
// Get libraries
#include <iostream> // Includes basic functions, including int main to call the system to run main functions
#include <string>   // For access to strings, we'll be needing these
#include <sstream>  // For being able to string split and do string manipulation
#include <fstream>

#define popen _popen // Because in some cases popen might not be valid so we need to define just in case

using namespace std; // So I don't have to keep freaking put :: all over the place

// Make the function to execute netsh on the system
// This function and function parameter will help us later to execute commands on the system
string executeCommand(const char* cmd) {
    char buffer[256]; // This string stores the command
    FILE* commandFile = _popen(cmd, "r"); // Opens a pipe to the command and creates a file
    string input; // Convert the buffer command into a string to be executed
    if (fgets(buffer, sizeof(buffer), commandFile) != NULL) {
        return input += buffer; // We literally have to return this, I hate C++ syntax, anyways, appends each line of the output to the string
    }
}

// The SSID is a string, mfs
// Yea MFS, I told you this is for Windows!!!! This is literally using netsh!!!!!
string getSsid() {
    stringstream ss(executeCommand("netsh wlan show interfaces | findstr SSID").data()); // Make a string stream variable for manipulation that stores the finding SSID command
    string input; // Input string to help get the SSID, using the getline() command and with the stringstream
 // Find the separator between SSID and SSID:, usually in the middle
    while (getline(ss, input)) { // Get the SSID while those variables exist
        // This will help separate unwanted stuff, like the SSID text (context: By SSID text, I mean like it says SSID: then the SSID) when running netsh
    
         if (input.find("SSID") != string::npos) { // Make sure SSID is not a NULL string, and see why we used pos? we got to separate that sh
               size_t pos = input.find(":");
           return input.substr(pos + 2); // Go by 4 spaces, that's where the SSID (Wi-Fi name) is located
            // Why does C++ have to make string return a value ):, this is torture
        }
    }
}

// Yay, I'm done torturing my hands with the SSID

// Why am I doing this for you
// Im not explaing the rest of that function, good luck MFS
// We need to retrieve the SSID so netsh will know what to get the password for, so that's why we are referencing this SSID variable, along with a parameter to call later.
std::string getPassword(const std::string& ssid) {
    std::string command = "netsh wlan show profile \"" + ssid + "\" key=clear";
    std::stringstream ss(executeCommand(command.c_str()));
    std::string input;
    while (std::getline(ss, input)) {
        if (input.find("Key Content") != std::string::npos) {
            return input.substr(29, input.length());
        }
    }
    return "< NULL >";
}

int main() {
    // First, get the SSID
    string ssid = getSsid(); // In case you're dumb as hell, THE SSID IS A STRING
    cout << "SSID: " << ssid << endl;

    // Now, get the password
    string password = getPassword(ssid);
    cout << "Password: " << password << endl;

    return 0;
}
