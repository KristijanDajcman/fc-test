#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include "HTMLElement.hpp"
#include <vector>

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

HTMLElement json_to_html(const nlohmann::json &data)
{
    std::vector<HTMLElement> document_elements;

    // Check the doctype
    if (data.contains("doctype"))
    {
        HTMLElement doctype = HTMLElement("!DOCTYPE", true);
        doctype.addAttribute("", data["doctype"]);
        document_elements.push_back(doctype);
    }

    HTMLElement html = HTMLElement("html", false);
    if (data.contains("language"))
        html.addAttribute("lang", data["language"]);

    
    // Build the head
    HTMLElement head = HTMLElement("head", false);
    if (data.contains("head"))
    {


        html.addChild(head);
    }

    document_elements.push_back(html);
    return HTMLElement("foo", false);
}

int main()
{
    std::string path = "../primeri/pageNotFound.json";
    std::string json_string;
    // read the json file
    try
    {
        json_string = read_json_string(path);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    nlohmann::json json_data;
    // Deserialize JSON string into a nlohmann::json object
    try
    {
        json_data = nlohmann::json::parse(json_string);
    }
    // ensure the json is valid
    catch (const nlohmann::json::exception &e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }

    json_to_html(json_data);

    // Print the entire JSON object
    // std::cout << json_data.dump(4) << std::endl;

    // Access specific fields dynamically
    // if (json_data.contains("head")) {
    //    std::cout << "head: " << json_data["head"] << std::endl;
    //}

    return 0;
}