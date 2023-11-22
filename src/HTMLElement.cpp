#include "HTMLElement.hpp"

HTMLElement::HTMLElement() {};
HTMLElement::HTMLElement(std::string tagname) : tagname(tagname), empty(false) {}
HTMLElement::HTMLElement(std::string tagname, bool empty) : tagname(tagname), empty(empty) {}


std::string HTMLElement::getOpeningTag()
{
    if (!empty)
        return "<" + tagname + attributes + ">\n";
    return "<" + tagname + attributes + ">\n";
}
std::string HTMLElement::getClosingTag()
{
    if (!empty)
        return "\n</" + tagname + ">";
    return "";
}

void HTMLElement::addChild(HTMLElement child) { if (!empty) children.push_back(child); }

void HTMLElement::addAttribute(std::string attribute_name, std::string value) { attributes += " " + attribute_name + "=" + value; }

std::string HTMLElement::getHTML()
{
    std::string html = getOpeningTag();
    for (auto child : children)
        html += child.getHTML();
    html += content;
    html += getClosingTag();
    return html;
}

void HTMLElement::setContent(std::string content) { this->content = content; }

void HTMLElement::setEmpty(bool empty) { this->empty = empty; }