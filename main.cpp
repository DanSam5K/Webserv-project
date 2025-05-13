#include "includes/Webserv.hpp"


// int main(int argc, char **argv) {
//     (void)argc;
//     (void)argv;
//     std::cout << "Webserv starting..." << std::endl;
//     return 0;
// }


int main(int argc, char** argv)
{
    // Ensure the program is called with exactly one argument: the config file path
    if (argc != 2)
    {
        std::cerr << "Error: Missing configuration file.\n";
        std::cerr << "Usage: ./webserv [configuration file path]" << std::endl;
        return 1;
    }

    // Create a configuration handler instance
    ConfigParser configHandler;

    try
    {
        // Extract the file name from the command-line argument
        std::string configFilePath = argv[1];

        // Verify the provided configuration file for correctness
        configHandler.validateConfig(configFilePath);

        // Optional: parseConfig() can be called here if needed
        // config.parseConfig();

    }
    catch (const std::exception& error)
    {
        // Display a clear error message if config loading fails and exit with error code
        std::cerr << "Error: Unable to load configuration file - " << error.what() << std::endl;
        return -1;
    }
    try
    {
       configHandler.printConfig();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    // Display the loaded configuration details for debugging or confirmation
    

    // Initialize the server using the validated configuration
    Server webServer(&configHandler);

    try
    {
        // Launch the server and begin processing requests
        webServer.run();
    }
    catch (const std::exception& error)
    {
        // Handle runtime server errors gracefully
        std::cerr << "Server terminated unexpectedly: " << error.what() << std::endl;
    }

    // Normal program termination
    return 0;
}