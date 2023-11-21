#ifndef HTMLElement_HPP
#define HTMLElement_HPP

#include <vector>
#include <string>

class HTMLElement
{
public:
    HTMLElement(std::string tagname, bool empty);

    std::string getOpeningTag();
    std::string getClosingTag();
    void addChild(HTMLElement child);
    void addAttribute(std::string attribute_name, std::string value);
    std::string getHTML();
    void setContent(std::string content);
    void setEmpty(bool empty);

private:
    std::vector<HTMLElement> children;
    std::string tagname;
    std::string attributes;
    std::string content;
    bool empty;
};

#endif