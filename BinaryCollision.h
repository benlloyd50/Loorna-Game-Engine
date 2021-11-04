#ifndef BINARYCOLLISION_H
#define BINARYCOLLISION_H

class BinaryCollision {
    private:
        int cols;
        int rows;
        int** collisionGrid;

    public:
        //Constructor requiring (height, width)
        BinaryCollision(int height, int width) {
            cols = width;
            rows = height;
            collisionGrid = new int*[rows];

            for (int i = 0; i < rows; i++) {
                collisionGrid[i] = new int[cols];
            }

            for (int y = 0; y < rows; y++) {
                for (int x = 0; x < cols; x++) {
                    collisionGrid[y][x] = 0;
                }
            }

            AddCollision(1,2);
            AddCollision(5,7);
        }

        void AddCollision(int x, int y) {
            collisionGrid[y][x] = 1;
        }

        bool isPassable(int x, int y) {
            if(x < 0 || y < 0 || x > cols - 1 || y > rows - 1)
                return false;

            if(collisionGrid[y][x] == 1) 
                return false;
            else if(collisionGrid[y][x] == 0)
                return true;
            else 
                return false;
        }

};

#endif