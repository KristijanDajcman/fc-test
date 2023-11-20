#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

std::string read_json_string(std::string path)
{
    std::ifstream file(path);

    if (!file.is_open())
        throw std::ios_base::failure("Error when opening file: " + path);
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json_string = buffer.str();

    // Close the file stream
    file.close();
    
    return json_string;
}

int main() {
    std::string path = "../primeri/pageNotFound.json";
    std::string json_string;
    // read the json file
    try
    {
        json_string = read_json_string(path);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    nlohmann::json json_data;
    // Deserialize JSON string into a nlohmann::json object
    try{
        json_data = nlohmann::json::parse(json_string);
    }
    // ensure the json is valid
    catch(const nlohmann::json::exception& e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }

    // Print the entire JSON object
    std::cout << json_data.dump(4) << std::endl;

    // Access specific fields dynamically
    if (json_data.contains("head")) {
        std::cout << "head: " << json_data["head"] << std::endl;
    }

    return 0;
}