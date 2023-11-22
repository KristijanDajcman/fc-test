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

std::string complexAttributeHead(nlohmann::json attribute)
{
    std::string attribute_string = "";
    for (auto i : attribute.items())
        attribute_string += i.key() + "=" + i.value().dump() + ", ";
    attribute_string = attribute_string.substr(0, attribute_string.size() - 2);
    return attribute_string;
}

std::string complexAttributeBody(nlohmann::json attribute)
{
    std::cout << "In Attributes " << attribute.dump() << std::endl;
    std::string attribute_string = "";
    for (auto i : attribute.items())
        attribute_string += i.key() + ":" + i.value().dump() + ";";
    
    // Remove " from string
    std::string clean_string = "";
    for (char c : attribute_string) {
        if (c != '"') {
            clean_string += c;
        }
    }
    clean_string = clean_string.substr(0, clean_string.size() - 1);
    return '"' + clean_string + '"';
}

void bodyToHTML(nlohmann::json jsonObject, HTMLElement &parent)
{
    std::cout << jsonObject.dump() << std::endl;
    for (const auto &entry : jsonObject.items())
    {

        const std::string key = entry.key();
        const nlohmann::json value = entry.value();
        if (key == "attributes")
        {
            std::cout << "parent: " << parent.getOpeningTag() << " -> ";
            std::cout << key << " " << value << std::endl;
            if (value.is_object())
            {
                nlohmann::json attributes = value;
                for (auto attribute : attributes.items())
                {
                    if (attribute.value().is_object())
                        parent.addAttribute(attribute.key(), complexAttributeBody(attribute.value()));
                    else
                        parent.addAttribute(attribute.key(), attribute.value().dump());
                }
            }
        }
        else
        {
            HTMLElement child = HTMLElement(key);
            std::cout << key << " -> " << value << std::endl;
            if (value.is_object())
            {
                // has nested attributes or elements
                bodyToHTML(value, child);
                parent.addChild(child);
            }
            else
            {
                // has content
                child.setContent(value);
                parent.addChild(child);
            }
        }
    }
}

std::vector<HTMLElement> toHTML(const nlohmann::json &data)
{
    std::vector<HTMLElement> document_elements;

    // Check the doctype
    if (data.contains("doctype"))
    {
        std::string dType = data["doctype"];
        HTMLElement doctype = HTMLElement("!DOCTYPE " + dType, true);
        document_elements.push_back(doctype);
    }

    HTMLElement html = HTMLElement("html");
    if (data.contains("language"))
        html.addAttribute("lang", data["language"]);

    // Build the head. Head has mostly empty elements and elements without attributes
    /*HTMLElement head = HTMLElement("head");
    if (data.contains("head"))
    {
        const nlohmann::json headObject = data["head"];
        // std::cout << headObject.dump(4) << std::endl;
        for (auto &entry : headObject.items())
        {
            const std::string key = entry.key();
            const nlohmann::json value = entry.value();

            HTMLElement headElement = HTMLElement(key);
            // Is the HTML element empty with attributes or not empty without attributes
            if (value.is_object())
            {
                // HTML has attributes but no content
                headElement.setEmpty(true);
                for (auto &attribute : value.items())
                {
                    if (attribute.value().is_object())
                        headElement.addAttribute(attribute.key(), complexAttributeHead(attribute.value()));
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
    }*/

    HTMLElement body = HTMLElement("body");
    // Build the body
    if (data.contains("body"))
    {
        bodyToHTML(data["body"], body);
        html.addChild(body);
    }
    document_elements.push_back(html);
    return document_elements;
}

int main()
{
    //std::string path = "../primeri/helloWorld.json";
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