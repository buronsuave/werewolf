#include "common.h"

void write_log(const char *text)
{
    std::time_t current_time = std::time(nullptr);
    std::tm* local_time = std::localtime(&current_time);

    // Format the date
    char filename[50]; // Assuming filename won't exceed 20 characters
    std::strftime(filename, sizeof(filename), "logs/%d-%m-%Y.log", local_time);

    // Open the log file
    std::ofstream logfile(filename, std::ios_base::app);
    if (!logfile) 
    {
        std::cout << "Error: Unable to open log file: " << filename;
        return;
    }
    // Write message to the log file
    // Format the date for logging
    char datetime[30]; // Adjust the size according to your needs
    std::strftime(datetime, sizeof(datetime), "[%d-%m-%Y %H:%M:%S] ", local_time);
    logfile << datetime << text;

    // Close the log file
    logfile.close();
}

// Function to replace printf for logging
void write_formatted_log(const char* format, ...) 
{
    // Create a char array to store the formatted message
    char buffer[1024]; // Adjust the size according to your needs
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Log the message to file
    write_log(buffer);
}