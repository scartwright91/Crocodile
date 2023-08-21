#include "Connection.h"

Connection::Connection(
    s2d::Scene *scene,
    float width,
    s2d::Object *a,
    s2d::Object *b) : scene(scene), width(width), a(a), b(b)
{
    init();
}

Connection::~Connection()
{
    delete rect;
    scene->removeChild(rect, "canvas");
}

void Connection::update(glm::vec2 pos)
{
    if (orientation == "horizontal")
    {
        glm::vec2 current = rect->getPosition();
        rect->setPosition(glm::vec2(current.x, pos.y));
    }
}

void Connection::init()
{
    // determine whether orientation is vertical or horizontal
    glm::vec2 apos = a->getPosition();
    float topA = apos.y;
    float botA = apos.y + a->size.y;
    float leftA = apos.x;
    float rightA = apos.x + a->size.x;
    glm::vec2 bpos = b->getPosition();
    float topB = bpos.y;
    float botB = bpos.y + b->size.y;
    float leftB = bpos.x;
    float rightB = bpos.x + b->size.x;
    if (!(botB < topA || topB > botA))
        orientation = "horizontal";
    if (!(rightB < leftA || leftB > rightA))
        orientation = "vertical";

    // calculate p1, p2
    if (orientation == "horizontal")
    {
        if (leftB > leftA)
        {
            p1 = glm::vec2(rightA, topB);
            p2 = glm::vec2(leftB, topB);
        }
        else
        {
            p1 = glm::vec2(rightB, topB);
            p2 = glm::vec2(leftA, topB);
        }
        createRect();
    }
}

void Connection::createRect()
{
    rect = new s2d::Object();
    if (orientation == "horizontal")
    {
        rect->setPosition(glm::vec2(p1.x, p1.y + width / 2.f));
        rect->size = glm::vec2(p2.x - p1.x, width);
    }
    scene->addChild(rect, "canvas");
}
