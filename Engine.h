#ifndef ENGINE_H
#define ENGINE_H

//Usual C++ Libraries
#include <iostream>

#include <vector>
#include <string>

/* 
    Set up input function for Windows or Linux
    Is a weird beast that I do not fully comprehend but it works? I think...
*/
#ifdef __linux__
// If the code is being compiled on Linux
#include <termios.h>
#include <unistd.h>
int getch() {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO,&oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO,TCSANOW,&oldattr);
    return ch;
}
#endif
#ifdef _WIN32
// If the code is being compiled on Windows
#include <conio.h>
#endif

using std::string;
using std::to_string;

namespace loorna {
    
    //A pair of 2 int values with no predefined context
    class Vector2 {
        public:
            int x; 
            int y;
            Vector2(int _x, int _y) {
                x = _x;
                y = _y;
            }
            Vector2 operator +(Vector2 v) {
                return Vector2(x + v.x, y + v.y);
            }
            Vector2 operator +=(Vector2 v) {
                *this = Vector2(x + v.x, y + v.y);
                return *this;
            }
            Vector2 operator -=(Vector2 v) {
                *this = Vector2(x - v.x, y - v.y);
                return *this;
            }
            Vector2 operator -(Vector2 v) {
                return Vector2(x - v.x, y - v.y);
            }
            Vector2 operator *(Vector2 v) {
                return Vector2(x * v.x, y * v.y);
            }
            Vector2 operator /(Vector2 v) {
                return Vector2(x / v.x, y / v.y);
            }

    };

    //A 3 int variable that is used for color
    class Color {
        public:
            int r;
            int g;
            int b;

            //Default Constructor
            Color(int _r, int _g, int _b) {
                r = _r;
                g = _g;
                b = _b;
            }

            //Copy Constructor
            Color(const Color& c) { r = c.r; g = c.g; b = c.b; }

            //Returns a string without ansi escape sequence included
            string ToString() {
                return to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m";
            }
    };

    //A string that has information about the colors it will have
    class Tile {
        public:
            string sprite;
            Color textColor;
            Color bgColor;

            //Default constructor
            Tile() : textColor(255, 255, 255), bgColor(38, 38, 38) {
                sprite = " ";
            }

            //Constructor with a string provided
            Tile(string _sprite) : textColor(255, 255, 255), bgColor(38, 38, 38) {
                sprite = _sprite;
            }
            
            //Constructor with a string and color information provided
            Tile(string _sprite, Color text, Color bg) : textColor(text), bgColor(bg) {
                sprite = _sprite;
            }

            // //Constructor with added color options
            // StringSprite(string _sprite, int tr = 255, int tg = 255, int tb = 255, int br = 38, int bg = 38, int bb = 38) {
            //     sprite = _sprite;
            //     textColor = "\33[38;2;" + to_string(tr) + ";" + to_string(tg) + ";" + to_string(tb) + "m";
            //     bgColor = "\33[38;2;" + to_string(br) + ";" + to_string(bg) + ";" + to_string(bb) + "m";
            // }
            
            string GetColoredString() {
                return "\33[38;2;" + textColor.ToString() + "\33[48;2;" + bgColor.ToString() + sprite;
            }

    };

    //A position and sprite for representing the object on the grid
    class PointObject : public Tile {
        public:
            Vector2 pos;    //The x,y value of where the PointObject is located on the grid
            Tile tile;      //The "sprite" of the point, supporting UNICODE!!
            string name;    //Currently unused but will be used for easier access to get
        
            PointObject(int _x, int _y, Tile _sprite) : pos(_x, _y), Tile() {
                tile = _sprite;
            }

            //Gives the information of the PointObject, mainly for debug
            void PrintOutInfo() {
                // std::cout << "My sprite is " << tile.sprite << "\nMy x:" << pos.x << " My y:" << pos.y << std::endl;
                printf("My sprite is %s\nMy x: %i My y: %i\n", tile.sprite.c_str(), pos.x, pos.y);
            }

    };


    /*
        Contains the foundation logic for Update and Render
    */
    class Scene {
        private:
            int cols;
            int rows;
            Tile** grid;
            std::vector<PointObject*> AlivePointObjs;

        public:
            //Default constructor that initializes the game grid
            Scene(int height, int width) {
                rows = height;
                cols = width;
                grid = new Tile*[rows];

                for (int i = 0; i < rows; i++) {
                    grid[i] = new Tile[cols];
                }

                for (int y = 0; y < rows; y++) {
                    for (int x = 0; x < cols; x++) {
                        grid[y][x] = Tile();
                    }
                }
                

            }
            //How to destructor?
            ~Scene() {

            }


            //Assigns grid values of the positions from the objects present
            void Update() {
                //Turns the grid to blank Tiles (a gray space)
                for (int y = 0; y < rows; y++) {
                    for (int x = 0; x < cols; x++) {
                        grid[y][x] = Tile();
                    }
                }

                //Checks all AlivePointObjs in scene and adds them into the grid overwritting previous sprites.
                for(int i = 0; i < AlivePointObjs.size(); i++) {
                    grid[AlivePointObjs[i]->pos.y][AlivePointObjs[i]->pos.x] = AlivePointObjs[i]->tile;
                }

            }

            //Draws the grid to the console
            void Render() {                
                //Gives the game the feeling of a frame being display rather than a chat log effect
                std::cout << "\033[2J\033[0;0H";
                string output;

                for (int y = 0; y < rows; y++) {
                    for (int x = 0; x < cols; x++) {
                        output += grid[y][x].GetColoredString();
                    }
                    output += '\n';
                }

                //Both accomplish same thing, but printf seems better at times
                printf("%s\033[0m", output.c_str());
                // std::cout << output << "\033[0m";
            }

            void AddPointToScene(PointObject* p) {
                AlivePointObjs.push_back(p);
            }
            
            //Retunrs the grid's width/cols
            int GetSceneWidth() { return cols; }
            // Returns the grid's height/rows
            int GetSceneHeight() { return rows; }

            //A color rendering test
            void RenderColors() {
                int i, j, n;
  
                for (i = 0; i < 11; i++) {
                    for (j = 0; j < 10; j++) {
                        n = 10 * i + j;
                        if (n > 108) break;
                        printf("\033[%dm %3d\033[m", n, n);
                    }
                    printf("\n");
                }
            }

            //A test for rendering letters
            void RenderLetters() {
                string test = "\u263A";
                std::cout << test;
                    
            }

    };

}

#endif

/*
                A example for seeing how the grid should be iterated through 
                to appear similar to the 1st quadrant of the cartesian plane
                THIS is pretty shit mate but maybe a look over it in the future would be better
            
                void RenderPositions() {
                    for (int y = gridHeight - 1; y > -1; y--) {
                        for (int x = 0; x < gridWidth; x++) {
                            std::cout << "(" << x << ", " << y << ") ";
                        }
                        std::cout << std::endl;
                    }
                }
            */

/*  
        The following ANSI escape sequences makes the input error message display
        as strong ([1m) underlined ([4m) white text ([97m) on a bright red background ([101m),
        cout << "\033[1m\033[4m\033[101m\033[97m";
        
        then resets all attributes ([0m) to display normal text after the message.

        ~An idea of how to implement colorful, stylable, text

        int textStyle = 0;
        int brightBG = 102;
        int darkBG = 42;
        int textColor = 34;

        std::cout << "\033[" << textStyle << "m\033[" << textColor << "m\033[" << brightBG << "m\033[" << darkBG << "m";
        string test = "☻";
        std::cout << test;
        std::cout << "\033[0m" << std::endl;

        An implementation of using rgb values to color text color, 38 can be changed to 48 for bg color
            int r = 238;
            int g = 255;
            int b = 80;
            std::cout << "\33[38;2;" << r << ";" << g << ";" << b << "m";
*/