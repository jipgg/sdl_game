#pragma once
#include <tinyxml2.h>
#include "game_components.h"
inline void AddComponentToXML(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* parentElement, Component<Game>* component) {
    // Create a new XML element for the current component
    using namespace tinyxml2;
    tinyxml2::XMLElement* element;
    if (Game_component* gc = dynamic_cast<Game_component*>(component)) {
        element = doc.NewElement("Game_component");
        XMLElement* position = element->InsertNewChildElement("position");
        position->SetAttribute("x", gc->position.x);
        position->SetAttribute("y", gc->position.y);
        XMLElement* physical_properties = element->InsertNewChildElement("physical_properties");
        Physical_properties& props = gc->physical_properties;
        physical_properties->SetAttribute("is_welded", props.is_welded);
        physical_properties->SetAttribute("is_collidable", props.is_collidable);
        physical_properties->SetAttribute("elasticity", props.elasticity);
        physical_properties->SetAttribute("mass", props.mass);
    } else {
        element = doc.NewElement("Component");
    }
    element->SetAttribute("name", component->name.c_str());
    element->SetAttribute("type_hash", component->type_hash);

    // Add children elements
    for (const auto& child : component->children) {
        AddComponentToXML(doc, element, child.second.get());
    }
    // Append the element to the parent element
    parentElement->InsertEndChild(element);

    tinyxml2::XMLError result = doc.SaveFile("components.xml");
    if (result != tinyxml2::XML_SUCCESS) {
    }

}

inline void save_to_file(Component<Game>* game) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Components");
    doc.InsertFirstChild(root);

    AddComponentToXML(doc, root, game);
}

