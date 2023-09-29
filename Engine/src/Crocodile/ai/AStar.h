/*
    Copyright (c) 2015, Damian Barczynski <daan.net@wp.eu>
    Following tool is licensed under the terms and conditions of the ISC license.
    For more information visit https://opensource.org/licenses/ISC.
*/
#pragma once

#include "glm/glm.hpp"

#include <vector>
#include <functional>
#include <set>

namespace Crocodile
{

    using uint = unsigned int;
    using HeuristicFunction = std::function<uint(glm::vec2, glm::vec2)>;
    using CoordinateList = std::vector<glm::vec2>;

    struct Node
    {
        uint G, H;
        glm::vec2 coordinates;
        Node *parent;

        Node(glm::vec2 coord_, Node *parent_ = nullptr);
        uint getScore();
    };

    using NodeSet = std::vector<Node *>;

    class Generator
    {
        bool detectCollision(glm::vec2 coordinates_);
        Node *findNodeOnList(NodeSet &nodes_, glm::vec2 coordinates_);
        void releaseNodes(NodeSet &nodes_);

    public:
        Generator();
        void setWorldSize(glm::vec2 worldSize_);
        void setDiagonalMovement(bool enable_);
        void setHeuristic(HeuristicFunction heuristic_);
        CoordinateList findPath(glm::vec2 source_, glm::vec2 target_);
        void addCollision(glm::vec2 coordinates_);
        void removeCollision(glm::vec2 coordinates_);
        void clearCollisions();

    private:
        HeuristicFunction heuristic;
        CoordinateList direction, walls;
        glm::vec2 worldSize;
        uint directions;
    };

    class Heuristic
    {
        static glm::vec2 getDelta(glm::vec2 source_, glm::vec2 target_);

    public:
        static uint manhattan(glm::vec2 source_, glm::vec2 target_);
        static uint euclidean(glm::vec2 source_, glm::vec2 target_);
        static uint octagonal(glm::vec2 source_, glm::vec2 target_);
    };
}
