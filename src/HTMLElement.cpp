#include "HTMLElement.hpp"

HTMLElement::HTMLElement(std::string tagname, bool empty) : tagname(tagname), empty(empty) { attributes = ""; }

std::string HTMLElement::getOpeningTag()
{
    if (!empty)
        return "<" + tagname + attributes + ">";
    return "<" + tagname + attributes + "/>";
}
std::string HTMLElement::getClosingTag()
{
    if (!empty)
        return "</" + tagname + ">";
    return "";
}

void HTMLElement::addChild(HTMLElement child) { if (!empty) children.push_back(child); }

void HTMLElement::addAttribute(std::string attribute_name, std::string value) { attributes += " " + attribute_name + "=" + value; }

std::string HTMLElement::getHTML()
{
    std::string html = getOpeningTag();
    for (auto child : children)
        html += child.getHTML();
    html += getClosingTag();
    return html;
}