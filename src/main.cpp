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
        
    std::string clean_string = "";
    for (char c : attribute_string)
    {
        if (c != '"')
            clean_string += c;
    }
    clean_string = clean_string.substr(0, clean_string.size() - 2);
    return '"' + clean_string + '"';
}

std::string complexAttributeBody(nlohmann::json attribute)
{
    std::string attribute_string = "";
    for (auto i : attribute.items())
        attribute_string += i.key() + ":" + i.value().dump() + ";";

    // Remove " from string
    std::string clean_string = "";
    for (char c : attribute_string)
    {
        if (c != '"')
        {
            clean_string += c;
        }
    }
    clean_string = clean_string.substr(0, clean_string.size() - 1);
    return '"' + clean_string + '"';
}

void bodyToHTML(nlohmann::json jsonObject, HTMLElement &parent)
{
    for (const auto &entry : jsonObject.items())
    {
        const std::string key = entry.key();
        const nlohmann::json value = entry.value();
        if (key == "attributes")
        {
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

void metaExceptional(nlohmann::json values, HTMLElement& parent)
{
    for (auto& value : values.items())
    {
        HTMLElement meta = HTMLElement("meta", true);
        if (value.value().is_object())
        {
            meta.addAttribute("name", value.key());
            meta.addAttribute("content", complexAttributeHead(value.value()));
        }
        else
        {
            meta.addAttribute("name", value.key());
            meta.addAttribute("content", value.value());
        }
        parent.addChild(meta);
    }
}

void headToHTML(nlohmann::json jsonObject, HTMLElement &parent)
{
    for (auto &entry : jsonObject.items())
    {
        const std::string key = entry.key();
        const nlohmann::json value = entry.value();

        if (key == "meta")
        {
            metaExceptional(value, parent);
            continue;;
        }
        
        // Is the HTML element empty with attributes or not empty without attributes
        if (value.is_object())
        {
            HTMLElement child = HTMLElement(key, true);
            // HTML has attributes but no content
            for (auto &attribute : value.items())
            {
                if (attribute.value().is_object())
                    child.addAttribute(attribute.key(), complexAttributeHead(attribute.value()));
                else
                    child.addAttribute(attribute.key(), attribute.value().dump());
            }
            parent.addChild(child);
        }
        else
        {
            if (value.is_array())
            {
                // Handle array elements
                for (const auto &arrayElement : value)
                {
                    if (arrayElement.is_object())
                    {
                        HTMLElement arrayChild = HTMLElement(key, true);

                        for (auto &attribute : arrayElement.items())
                        {
                            if (attribute.value().is_object())
                                arrayChild.addAttribute(attribute.key(), complexAttributeHead(attribute.value()));
                            else
                                arrayChild.addAttribute(attribute.key(), attribute.value().dump());
                        }

                        parent.addChild(arrayChild);
                    }
                    else
                    {
                        HTMLElement arrayChild = HTMLElement(key);
                        arrayChild.setContent(arrayElement);
                        parent.addChild(arrayChild);
                    }
                }
            }
            else
            {
                HTMLElement child = HTMLElement(key);
                // HTML has content but no attributes
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
    if (data.contains("head"))
    {
        HTMLElement head = HTMLElement("head");
        headToHTML(data["head"], head);
        html.addChild(head);
    }

    
    // Build the body
    if (data.contains("body"))
    {
        HTMLElement body = HTMLElement("body");
        bodyToHTML(data["body"], body);
        html.addChild(body);
    }
    document_elements.push_back(html);
    return document_elements;
}

void writeHTML(std::string name, std::string data)
{
    std::ofstream outputFile(name + ".html");

    if (outputFile.is_open()) {
        outputFile << data;

        outputFile.close();

        std::cout << "JSON has been converted to HTML." << std::endl;
    } else {
        std::cerr << "Unable to open the file for writing." << std::endl;
    }
}

int main()
{
    std::string path;
    std:: cout << "Enter path: ";
    std::cin >> path;

    // std::string path = "../primeri/pageNotFound.json";
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

    std::string html_string;
    for (auto i : holder)
        html_string += i.getHTML();


    std::string out;
    std:: cout << "Enter output path: ";
    std::cin >> out;
    
    writeHTML(out, html_string);

    return 0;
}