#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

class Spaceship {
public:
    int x, y;
    Spaceship() : x(WIDTH/2), y(HEIGHT-2) {}

    void moveLeft() { if (x > 0) x--; }
    void moveRight() { if (x < WIDTH-1) x++; }
};

class Alien {
public:
    int x, y;
    bool alive;
    Alien(int px, int py) : x(px), y(py), alive(true) {}
};

class Bullet {
public:
    int x, y;
    bool active;
    Bullet(int px, int py) : x(px), y(py), active(true) {}
};

class Game {
private:
    Spaceship player;
    vector<Alien> aliens;
    vector<Bullet> bullets;
    int score;
    bool gameOver;

public:
    Game() : score(0), gameOver(false) {
        srand(time(NULL));
        initAliens();
    }

    void initAliens() {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                aliens.push_back(Alien(j * 4, i * 3));
            }
        }
    }

    void drawGame() {
        system("cls");

        // Gambar batas atas
        for (int i = 0; i < WIDTH+2; i++) {
            cout << "#";
        }
        cout << endl;

        // Gambar arena
        for (int y = 0; y < HEIGHT; y++) {
            cout << "#";
            for (int x = 0; x < WIDTH; x++) {
                bool drawn = false;

                // Gambar player
                if (x == player.x && y == player.y) {
                    cout << "A";
                    drawn = true;
                }

                // Gambar alien
                for (auto& alien : aliens) {
                    if (alien.alive && alien.x == x && alien.y == y) {
                        cout << "W";
                        drawn = true;
                        break;
                    }
                }

                // Gambar peluru
                for (auto& bullet : bullets) {
                    if (bullet.active && bullet.x == x && bullet.y == y) {
                        cout << "|";
                        drawn = true;
                        break;
                    }
                }

                if (!drawn) cout << " ";
            }
            cout << "#" << endl;
        }

        // Gambar batas bawah
        for (int i = 0; i < WIDTH+2; i++) {
            cout << "#";
        }
        cout << endl;

        cout << "Score: " << score << endl;
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'a': player.moveLeft(); break;
                case 'd': player.moveRight(); break;
                case ' ':
                    bullets.push_back(Bullet(player.x, player.y - 1));
                    break;
            }
        }
    }

    void updateBullets() {
        for (auto& bullet : bullets) {
            if (bullet.active) {
                bullet.y--;

                // Cek tabrakan dengan alien
                for (auto& alien : aliens) {
                    if (alien.alive && bullet.x == alien.x && bullet.y == alien.y) {
                        alien.alive = false;
                        bullet.active = false;
                        score += 10;
                        break;
                    }
                }

                // Hapus peluru yang keluar arena
                if (bullet.y < 0) bullet.active = false;
            }
        }
    }

    bool checkWin() {
        for (auto& alien : aliens) {
            if (alien.alive) return false;
        }
        return true;
    }

    void run() {
        while (!gameOver) {
            drawGame();
            handleInput();
            updateBullets();

            if (checkWin()) {
                cout << "Selamat! Kamu Menang!" << endl;
                break;
            }

            Sleep(100);
        }

        if (gameOver) {
            cout << "Game Over! Skor Akhir: " << score << endl;
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
