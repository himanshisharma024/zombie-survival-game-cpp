#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <windows.h>
#include <conio.h> 
using namespace std;
class Character {
protected:
    int x, y;
    int health;
public:
    Character(int xPos, int yPos, int hp) : x(xPos), y(yPos), health(hp) {}
    virtual void move(int maxX, int maxY) = 0;
    virtual void attack(Character &target) = 0;
    int getX() const {
         return x; 
    }
    int getY() const {
         return y; 
        }
    int getHealth() const { 
        return health; 
    }

    void damage(int amount) {
         health -= amount; 
         if (health < 0) health = 0; 
        }
    void heal(int amount) { 
        health += amount; 
        if (health > 100) health = 100; 
    }
    bool isAlive() const { 
        return health > 0;
     }
};
class Player : public Character {
private:
    int ammo;
    int score;

public:
    Player(int xPos, int yPos) : Character(xPos, yPos, 100), ammo(5), score(0) {}
     
void move(int maxX, int maxY) override {
    
}

    void move(int maxX, int maxY, vector<pair<int,int>> &walls) {

         char move = _getch();
    move = toupper(move);

    int newX = x;
    int newY = y;

    if (move == 'W') newY--;
    else if (move == 'S') newY++;
    else if (move == 'A') newX--;
    else if (move == 'D') newX++;
    else if (move == 'Q') { 
        cout << "\nYou quit the game.\n"; exit(0);
     }

    for (auto &w : walls) {
        if (w.first == newX && w.second == newY)
            return;  
    }

    if (newX >= 0 && newX < maxX && newY >= 0 && newY < maxY) {
        x = newX;
        y = newY;
    }
}

    void attack(Character &target) override {
        if (ammo > 0) {
            ammo--;
            target.damage(25);
            cout << "You shot a zombie! Ammo left: " << ammo << "\n";
        } else {
            cout << "No ammo left!\n";
        }
    }

    void addAmmo(int a) { 
        ammo += a;
     }
    void addScore(int s) { 
        score += s;
     }
    int getAmmo() const {
         return ammo; 
        }
    int getScore() const {
         return score;
         }
};

class Zombie : public Character {
public:
    Zombie(int xPos, int yPos) : Character(xPos, yPos, 50) {}

    void move(int maxX, int maxY) override {
        int dir = rand() % 4;
        if (dir == 0 && y > 0) y--;
        else if (dir == 1 && y < maxY - 1) y++;
        else if (dir == 2 && x > 0) x--;
        else if (dir == 3 && x < maxX - 1) x++;
    }

    void attack(Character &target) override {
        target.damage(10);
        cout << "Zombie attacked you! (-10 health)\n";
    }
};

class Game {
private:
    int width, height;
    Player player;
    vector<Zombie> zombies;
    pair<int, int> exitPos;
    vector<pair<int, int>> ammoPacks;
    vector<pair<int, int>> healthPacks;
    vector<pair<int, int>> walls;


public:
    Game(int w, int h) : width(w), height(h), player(w / 2, h / 2) {
        srand(time(0));

for (int i = 0; i < 10; i++)   
    zombies.push_back(Zombie(rand() % width, rand() % height));

do {
    exitPos = { rand() % width, rand() % height };
} while (abs(exitPos.first - player.getX()) + abs(exitPos.second - player.getY()) < 10);

for (int i = 0; i < 8; i++) {
    ammoPacks.push_back({ rand() % width, rand() % height });
    healthPacks.push_back({ rand() % width, rand() % height });
}
   
for (int i = 0; i < 25; i++) {   
    walls.push_back({ rand() % width, rand() % height });
}
      
    }
    

    void clearScreen() {
        system("cls"); 

    }

    void drawGrid() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool drawn = false;

            
            if (player.getX() == j && player.getY() == i) {
                cout << "🙂 ";
                drawn = true;
            }

            
            else if (exitPos.first == j && exitPos.second == i) {
                cout << "🚪 ";
                drawn = true;
            }

        
            for (auto &z : zombies) {
                if (z.isAlive() && z.getX() == j && z.getY() == i) {
                    cout << "🧟 "; 
                    drawn = true;
                    break;
                }
            }

            
            for (auto &a : ammoPacks) {
                if (a.first == j && a.second == i) {
                    cout << "🔫 ";
                    drawn = true;
                    break;
                }
            }

            
            for (auto &h : healthPacks) {
                if (h.first == j && h.second == i) {
                    cout << "💉 ";
                    drawn = true;
                    break;
                }
            }
            
for (auto &w : walls) {
    if (w.first == j && w.second == i) {
        cout << "🧱 ";
        drawn = true;
        break;
    }
}


            if (!drawn) cout << "⬜ ";
        }
        cout << "\n";
    }
    cout << endl;
}


    void checkPickup() {
    try {
        
        for (int i = 0; i < ammoPacks.size(); i++) {
            if (player.getX() == ammoPacks[i].first && player.getY() == ammoPacks[i].second) {
                cout << "You picked up ammo (+3)\n";
                player.addAmmo(3);
                ammoPacks.erase(ammoPacks.begin() + i);
                break;
            }
        }

       
        for (int i = 0; i < healthPacks.size(); i++) {
            if (player.getX() == healthPacks[i].first && player.getY() == healthPacks[i].second) {
                cout << "You picked up health (+20)\n";
                player.heal(20);
                healthPacks.erase(healthPacks.begin() + i);
                break;
            }
        }
    }
    catch (exception &e) {
        cout << "Item pickup error: " << e.what() << endl;
    }
}

        
    void moveZombies() {
        for (auto &z : zombies) {
            if (!z.isAlive()) continue;

            
            if (rand() % 2 == 0) {
                if (z.getX() < player.getX()) z.move(width, height);
                else if (z.getX() > player.getX()) z.move(width, height);
            } else {
                z.move(width, height);
            }
        }
    }

    void checkCombat() {
        for (auto &z : zombies) {
            if (z.isAlive() &&
                abs(z.getX() - player.getX()) <= 1 &&
                abs(z.getY() - player.getY()) <= 1) {

                cout << "Zombie nearby! Press F to attack or any key to skip.\n";
                char c = _getch();
                if (toupper(c) == 'F') {
                    player.attack(z);
                    if (!z.isAlive()) {
                        cout << "Zombie defeated!\n";
                        player.addScore(10);
                    }
                } else {
                    z.attack(player);
                }
            }
        }
    }

    void play() {
        while (player.isAlive()) {
            try{
            clearScreen();
            cout << "=== ZOMBIE SURVIVAL PUZZLE ===\n";
            cout << "Use W/A/S/D to move, F to attack, Q to quit.\n";
            cout << "Health: " << player.getHealth()
                 << " | Ammo: " << player.getAmmo()
                 << " | Score: " << player.getScore() << "\n\n";

            drawGrid();

            
            if (player.getX() == exitPos.first && player.getY() == exitPos.second) {
                cout << "\n🎉 You reached the exit safely! You win!\n";
                cout << "Final Score: " << player.getScore() << endl;
                break;
            }

            player.move(width, height, walls);
            checkPickup();
            checkCombat();
            moveZombies();

            if (!player.isAlive()) {
                cout << "\nYou were eaten by zombies... 💀\n";
                cout << "Final Score: " << player.getScore() << endl;
                break;
            }
        }
         catch (exception &e) {
            cout << "Game error: " << e.what() << endl;
        }
    }


        
        cout << "\n=== GAME OVER ===\n";
    }
};

int main() {
     SetConsoleOutputCP(CP_UTF8);
    Game g(20,20);
    g.play();
    return 0;
}