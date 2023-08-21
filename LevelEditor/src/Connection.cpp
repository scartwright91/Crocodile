#include "Connection.h"

Connection::Connection(
    s2d::Scene *scene,
    float width,
    std::string start,
    std::string end,
    s2d::Object *a,
    s2d::Object *b) : scene(scene), width(width), start(start), end(end), a(a), b(b)
{
}

Connection::Connection(
    s2d::Scene *scene,
    float width,
    std::string start,
    std::string end,
    s2d::Object *a,
    s2d::Object *b,
    std::string orientation,
    glm::vec2 pos,
    glm::vec2 size) : scene(scene), width(width), start(start), end(end), a(a), b(b), orientation(orientation)
{
    createRectFromData(pos, size);
}

Connection::~Connection()
{
    delete rect;
    scene->removeChild(rect, "canvas");
}

ConnectionData Connection::serialise()
{
    ConnectionData cd = {};
    cd.start = start;
    cd.end = end;
    cd.width = width;
    cd.orientation = orientation;
    cd.pos = rect->getPosition();
    cd.size = rect->size;
    return cd;
}

void Connection::update(glm::vec2 pos)
{
    glm::vec2 current = rect->getPosition();
    if (orientation == "horizontal")
        rect->setPosition(glm::vec2(current.x, pos.y));
    else
        rect->setPosition(glm::vec2(pos.x, current.y));
}

void Connection::createConnection()
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
    if (orientation != "")
    {
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
        }
        else
        {
            if (topB > topA)
            {
                p1 = glm::vec2(rightA, botA);
                p2 = glm::vec2(rightA, topB);
            }
            else
            {
                p1 = glm::vec2(rightA, botB);
                p2 = glm::vec2(rightA, topA);
            }
        }
        createRect();
    }
}

void Connection::createRect()
{
    scene->removeChild(rect, "canvas");
    delete rect;
    rect = new s2d::Object();
    if (orientation == "horizontal")
    {
        rect->setPosition(glm::vec2(p1.x, p1.y + width / 2.f));
        rect->size = glm::vec2(p2.x - p1.x, width);
    }
    else
    {
        rect->setPosition(glm::vec2(p1.x + width / 2.f, p1.y));
        rect->size = glm::vec2(width, p2.y - p1.y);
    }
    scene->addChild(rect, "canvas");
}

void Connection::createRectFromData(glm::vec2 pos, glm::vec2 size)
{
    rect = new s2d::Object();
    rect->setPosition(pos);
    rect->size = size;
    scene->addChild(rect, "canvas");
}
