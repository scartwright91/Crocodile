
#include "AStar.h"
#include <algorithm>
#include <math.h>

using namespace std::placeholders;

namespace Crocodile
{

    Node::Node(glm::vec2 coordinates_, Node *parent_)
    {
        parent = parent_;
        coordinates = coordinates_;
        G = H = 0;
    }

    uint Node::getScore()
    {
        return G + H;
    }

    Generator::Generator()
    {
        setDiagonalMovement(false);
        setHeuristic(&Heuristic::manhattan);
        direction = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
    }

    void Generator::setWorldSize(glm::vec2 worldSize_)
    {
        worldSize = worldSize_;
    }

    void Generator::setDiagonalMovement(bool enable_)
    {
        directions = (enable_ ? 8 : 4);
    }

    void Generator::setHeuristic(HeuristicFunction heuristic_)
    {
        heuristic = std::bind(heuristic_, _1, _2);
    }

    void Generator::addCollision(glm::vec2 coordinates_)
    {
        walls.push_back(coordinates_);
    }

    void Generator::removeCollision(glm::vec2 coordinates_)
    {
        auto it = std::find(walls.begin(), walls.end(), coordinates_);
        if (it != walls.end())
        {
            walls.erase(it);
        }
    }

    void Generator::clearCollisions()
    {
        walls.clear();
    }

    CoordinateList Generator::findPath(glm::vec2 source_, glm::vec2 target_)
    {
        Node *current = nullptr;
        NodeSet openSet, closedSet;
        openSet.reserve(100);
        closedSet.reserve(100);
        openSet.push_back(new Node(source_));

        while (!openSet.empty())
        {
            auto current_it = openSet.begin();
            current = *current_it;

            for (auto it = openSet.begin(); it != openSet.end(); it++)
            {
                auto node = *it;
                if (node->getScore() <= current->getScore())
                {
                    current = node;
                    current_it = it;
                }
            }

            if (current->coordinates == target_)
            {
                break;
            }

            closedSet.push_back(current);
            openSet.erase(current_it);

            for (uint i = 0; i < directions; ++i)
            {
                glm::vec2 newCoordinates(current->coordinates + direction[i]);
                if (detectCollision(newCoordinates) ||
                    findNodeOnList(closedSet, newCoordinates))
                {
                    continue;
                }

                uint totalCost = current->G + ((i < 4) ? 10 : 14);

                Node *successor = findNodeOnList(openSet, newCoordinates);
                if (successor == nullptr)
                {
                    successor = new Node(newCoordinates, current);
                    successor->G = totalCost;
                    successor->H = heuristic(successor->coordinates, target_);
                    openSet.push_back(successor);
                }
                else if (totalCost < successor->G)
                {
                    successor->parent = current;
                    successor->G = totalCost;
                }
            }
        }

        CoordinateList path;
        while (current != nullptr)
        {
            path.push_back(current->coordinates);
            current = current->parent;
        }

        releaseNodes(openSet);
        releaseNodes(closedSet);

        return path;
    }

    Node *Generator::findNodeOnList(NodeSet &nodes_, glm::vec2 coordinates_)
    {
        for (auto node : nodes_)
        {
            if (node->coordinates == coordinates_)
            {
                return node;
            }
        }
        return nullptr;
    }

    void Generator::releaseNodes(NodeSet &nodes_)
    {
        for (auto it = nodes_.begin(); it != nodes_.end();)
        {
            delete *it;
            it = nodes_.erase(it);
        }
    }

    bool Generator::detectCollision(glm::vec2 coordinates_)
    {
        if (coordinates_.x < 0 || coordinates_.x >= worldSize.x ||
            coordinates_.y < 0 || coordinates_.y >= worldSize.y ||
            std::find(walls.begin(), walls.end(), coordinates_) != walls.end())
        {
            return true;
        }
        return false;
    }

    glm::vec2 Heuristic::getDelta(glm::vec2 source_, glm::vec2 target_)
    {
        return {abs(source_.x - target_.x), abs(source_.y - target_.y)};
    }

    uint Heuristic::manhattan(glm::vec2 source_, glm::vec2 target_)
    {
        auto delta = std::move(getDelta(source_, target_));
        return static_cast<uint>(10 * (delta.x + delta.y));
    }

    uint Heuristic::euclidean(glm::vec2 source_, glm::vec2 target_)
    {
        auto delta = std::move(getDelta(source_, target_));
        return static_cast<uint>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
    }

    uint Heuristic::octagonal(glm::vec2 source_, glm::vec2 target_)
    {
        auto delta = std::move(getDelta(source_, target_));
        return 10 * (delta.x + delta.y) + (-6) * (uint)std::min(delta.x, delta.y);
    }

}
