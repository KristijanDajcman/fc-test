#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>
#include "HTMLElement.hpp"
#include <vector>

std::string readJSON(std::string path)
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

std::string complexAttribute(nlohmann::json attribute)
{
    std::string attribute_string = "";
    for (auto i : attribute.items())
    {
        std::cout << i.key() << std::endl;
        attribute_string += i.key() + "=" + i.value().dump() + ", ";
    }
    attribute_string = attribute_string.substr(0, attribute_string.size() - 2);
    return attribute_string;
}

std::vector<HTMLElement> toHTML(const nlohmann::json &data)
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

    // Build the head. Head has mostly empty elements and elements without attributes
    HTMLElement head = HTMLElement("head", false);
    if (data.contains("head"))
    {
        const nlohmann::json headObject = data["head"];
        // std::cout << headObject.dump(4) << std::endl;
        for (auto &entry : headObject.items())
        {
            const std::string key = entry.key();
            const nlohmann::json value = entry.value();

            HTMLElement headElement = HTMLElement(key, false);
            // Is the HTML element empty with attributes or not empty without attributes
            if (value.is_object())
            {
                // HTML has attributes but no content
                headElement.setEmpty(true);
                for (auto &attribute : value.items())
                {
                    if (attribute.value().is_object())
                        headElement.addAttribute(attribute.key(), complexAttribute(attribute.value()));
                    else
                        headElement.addAttribute(attribute.key(), attribute.value().dump());
                }
            }
            else
            {
                // HTML has content but no attributes
                headElement.setContent(value);
            }
            head.addChild(headElement);
        }
        html.addChild(head);

    }

    // Build the body
    if (data.contains("body"))
    {
    }
    document_elements.push_back(html);
    return document_elements;
}

int main()
{
    std::string path = "../primeri/pageNotFound.json";
    std::string json_string;
    // read the json file
    try
    {
        json_string = readJSON(path);
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

    auto holder = toHTML(json_data);

    for (auto i : holder)
        std::cout << i.getHTML() << std::endl;

    return 0;
}