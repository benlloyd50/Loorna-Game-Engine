#include "Engine.h"
#include "BinaryCollision.h"
#include "Singleton.h"

namespace game {
    /*
    *   Object containing the systems running in the scene
    *   Includes Collisions and in the future possibly Interactions and more
    */
    class GameManager {
        public:
            BinaryCollision CollMap;
            GameManager(int height, int width) : CollMap(height, width) {}
    };

    class Player : public loorna::PointObject {
        using loorna::PointObject::PointObject;            
        public:
            void Move(loorna::Scene* const scene, BinaryCollision collMap) {
                char c = getch();
                std::cout << "x: " << this->pos.x << " y: " << this->pos.y + 1 << std::endl;

                if (c == 'w' && collMap.isPassable(this->pos.x, this->pos.y - 1))
                    this->pos.y -= 1;
                else if (c == 'a' && collMap.isPassable(this->pos.x - 1, this->pos.y))
                    this->pos.x -= 1;
                else if (c == 's' && collMap.isPassable(this->pos.x, this->pos.y + 1))
                    this->pos.y += 1;
                else if (c == 'd' && collMap.isPassable(this->pos.x + 1, this->pos.y))
                    this->pos.x += 1;
            }
    };

    class Box : public loorna::PointObject {
        using loorna::PointObject::PointObject;            
        public:
            
    };
}

int main() {
    int width = 10;
    int height = 12;
    loorna::Scene *myScene;
    
    loorna::Scene testLevel(height, width);
    game::Player player(4, 5, loorna::Tile("\u263A", loorna::Color(129, 60, 253), loorna::Color(38, 38, 38)) );
    game::Box box1(5, 7, loorna::Tile("\u2317", loorna::Color(32, 212, 80), loorna::Color(38, 38, 38)) );
    game::Box box2(1, 2, loorna::Tile("\u2317", loorna::Color(32, 212, 80), loorna::Color(38, 38, 38)) );

    testLevel.AddPointToScene(&player);
    testLevel.AddPointToScene(&box1);
    testLevel.AddPointToScene(&box2);

    myScene = &testLevel;

    game::GameManager GM(myScene->GetSceneHeight(), myScene->GetSceneWidth());
    Singleton<game::GameManager>* singleton = Singleton<game::GameManager>::GetInstance(GM);

    while(1) {
        myScene->Update();
        myScene->Render();
        player.PrintOutInfo();
        std::cout << "Height: " << myScene->GetSceneHeight() << " Width: " << myScene->GetSceneWidth() << std::endl;
        
        
        player.Move(myScene, singleton->value().CollMap);
    }


}