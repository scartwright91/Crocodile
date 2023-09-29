
#include "Crocodile.h"

#include "Crocodile/ai/AStar.h"

using namespace Crocodile;

class Sandbox : public Crocodile::Application
{

public:
    Sandbox() : Crocodile::Application("Sandbox", true, 1280, 720, false)
    {
        Generator generator;
        generator.setWorldSize({25, 25});
        generator.addCollision({17, 17});
        generator.setHeuristic(Heuristic::euclidean);
        generator.setDiagonalMovement(true);

        std::cout << "Generate path ... \n";
        auto path = generator.findPath({0, 0}, {20, 20});

        for (auto &coordinate : path)
        {
            std::cout << coordinate.x << " " << coordinate.y << "\n";
        }
    }

    void update(float dt)
    {
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Sandbox();
}
