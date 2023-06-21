#include "TextEntity.h"

TextEntity::TextEntity(s2d::Scene *scene, s2d::Text *text, std::string layer) : scene(scene), text(text), layer(layer)
{
}

void TextEntity::select()
{
    text->outline = true;
}

void TextEntity::deselect()
{
    text->outline = false;
}