#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <algorithm>

using namespace std;

int gold = 0;
int menu, shield = 0, health = 0, doublegold = 0, shop;
string item[3] = {"Shield", "Health", "Double Gold"};
const int WIDTH = 20;
const int HEIGHT = 20;

struct HighScore {
    string name;
    int score;
    int level;
};

struct Spaceship {
    int x, y;
    int bullets;
};

struct Alien {
    int x, y;
    bool alive;
    int health;
    int direction;
};

struct Bullet {
    int x, y;
    bool active;
};

struct Barrier {
    int x, y;
    int direction;
};

vector<HighScore> highScores;
Spaceship player;
vector<Alien> aliens;
vector<Bullet> bullets;
vector<Bullet> enemyBullets;
Barrier barrier;
int level = 1;
int score = 0;
bool gameOver = false;

void initAliens() {
    aliens.clear();
    enemyBullets.clear();

    if (level == 1 || level == 2) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                Alien alien = {j * 4, i * 3, true, 1, 1};
                aliens.push_back(alien);
            }
        }
    } else if (level == 3) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                Alien alien = {j * 4, i * 3, true, 2, 1};
                aliens.push_back(alien);
            }
        }
        player.bullets = 75;
    }
}

void drawGame() {
    system("cls");

    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << endl;

    for (int y = 0; y < HEIGHT; y++) {
        cout << "#";
        for (int x = 0; x < WIDTH; x++) {
            bool drawn = false;

            if (x == player.x && y == player.y) {
                cout << "A";
                drawn = true;
            }

            for (auto& alien : aliens) {
                if (alien.alive && alien.x == x && alien.y == y) {
                    cout << (level == 3 ? "M" : "W");
                    drawn = true;
                    break;
                }
            }

            for (auto& bullet : bullets) {
                if (bullet.active && bullet.x == x && bullet.y == y) {
                    cout << "|";
                    drawn = true;
                    break;
                }
            }

            for (auto& bullet : enemyBullets) {
                if (bullet.active && bullet.x == x && bullet.y == y) {
                    cout << "v";
                    drawn = true;
                    break;
                }
            }

            if (level >= 2 && y == barrier.y && x >= barrier.x && x < barrier.x + 5) {
                cout << "_";
                drawn = true;
            }

            if (!drawn) cout << " ";
        }
        cout << "#" << endl;
    }

    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
    cout << "Level: " << level << endl;
    cout << "Peluru: " << player.bullets << endl;
    cout << "Gold: " << gold << endl;
    cout << "HP: " << health << endl;
    cout << "Shield: " << shield << endl;
    cout << "Double Gold: " << doublegold << endl;
}

void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'a': if (player.x > 0) player.x--; break;
            case 'd': if (player.x < WIDTH - 1) player.x++; break;
            case ' ':
                if (player.bullets > 0) {
                    Bullet bullet = {player.x, player.y - 1, true};
                    bullets.push_back(bullet);
                    player.bullets--;
                }
                break;
        }
    }
}

void enemyCounterAttack(int alienX) {
    Bullet bullet = {alienX, aliens[0].y + 1, true};
    enemyBullets.push_back(bullet);
}

void updateBullets() {
    for (auto& bullet : bullets) {
        if (bullet.active) {
            bullet.y--;

            for (auto& alien : aliens) {
                if (alien.alive && bullet.x == alien.x && bullet.y == alien.y) {
                    if (level == 3) {
                        alien.health--;
                        if (alien.health <= 0) {
                            alien.alive = false;
                            score += 20;
                            gold += (doublegold > 0) ? 4 : 2;
                            enemyCounterAttack(alien.x);
                        }
                    } else {
                        alien.alive = false;
                        score += 10;
                        gold += (doublegold > 0) ? 4 : 2;
                        enemyCounterAttack(alien.x);
                    }
                    bullet.active = false;
                    break;
                }
            }

            if (level >= 2 && bullet.y == barrier.y && bullet.x >= barrier.x && bullet.x < barrier.x + 5) {
                bullet.active = false;
            }

            if (bullet.y < 0) bullet.active = false;
        }
    }

    for (auto& bullet : enemyBullets) {
        if (bullet.active) {
            bullet.y++;
            if (bullet.y >= HEIGHT) {
                bullet.active = false;
            }
            if (bullet.x == player.x && bullet.y == player.y) {
                if (shield > 0) {
                    shield--;
                    cout << "Shield melindungi! Shield tersisa: " << shield << endl;
                    Sleep(1000);
                } else if (health > 0) {
                    health--;
                    cout << "Health berkurang! Health tersisa: " << health << endl;
                    Sleep(1000);
                } else {
                    gameOver = true;
                }
                bullet.active = false;
            }
        }
    }

    if (player.bullets <= 0) {
        bool canShoot = false;
        for (auto& bullet : bullets) {
            if (bullet.active) {
                canShoot = true;
                break;
            }
        }
        if (!canShoot) {
            cout << "Peluru habis! Game Over!" << endl;
            gameOver = true;
        }
    }
}

void updateAliens() {
    for (auto& alien : aliens) {
        if (alien.alive && level > 1) {
            alien.x += alien.direction;
            if (alien.x <= 0 || alien.x >= WIDTH - 1) {
                alien.direction = -alien.direction;
            }
        }
    }
}

void updateBarrier() {
    if (level >= 2) {
        barrier.x += barrier.direction;
        if (barrier.x <= 1 || barrier.x >= WIDTH - 2) {
            barrier.direction = -barrier.direction;
        }
    }
}

bool checkWin() {
    for (auto& alien : aliens) {
        if (alien.alive) return false;
    }
    return true;
}

void saveHighScore() {
    string name;
    cout << "Masukkan nama Anda (maksimal 10 huruf): ";
    cin >> name;
    if (name.length() > 10) name = name.substr(0, 10);

    HighScore hs = {name, score, level};
    highScores.push_back(hs);
    sort(highScores.begin(), highScores.end(), [](const HighScore& a, const HighScore& b) {
        return a.score > b.score;
    });

    ofstream file("highscore.txt");
    if (file.is_open()) {
        for (const auto& hs : highScores) {
            file << hs.name << " " << hs.score << " " << hs.level << endl;
        }
        file.close();
    }
}

void runGame() {
    player = {WIDTH / 2, HEIGHT - 2, 50};
    initAliens();
    barrier = {WIDTH / 2, HEIGHT - 6, 1};

    while (!gameOver) {
        drawGame();
        handleInput();
        updateBullets();
        updateAliens();
        if (level >= 2) updateBarrier();

        if (checkWin()) {
            bullets.clear();
            enemyBullets.clear();
            if (level == 3) {
                cout << "Selamat! Kamu Menang!" << endl;
                break;
            }
            level++;
            player.bullets = (level == 3) ? 75 : 50;
            cout << "Level " << level << " dimulai!" << endl;
            Sleep(2000);
            initAliens();
        }

        Sleep(100);
    }

    if (gameOver) {
        cout << "Game Over! Skor Akhir: " << score << endl;
        saveHighScore();
    }
}

void loadHighScores() {
    ifstream file("highscore.txt");
    if (file.is_open()) {
        highScores.clear();
        HighScore hs;
        while (file >> hs.name >> hs.score >> hs.level) {
            highScores.push_back(hs);
        }
        file.close();
    }
}

void showHighScores() {
    system("cls");
    cout << "=== HIGH SCORES ===" << endl;
    for (const auto& hs : highScores) {
        cout << hs.name << " - Skor: " << hs.score << " (Level: " << hs.level << ")" << endl;
    }
    cout << "1. Reset High Score" << endl;
    cout << "0. Kembali" << endl;
    int choice;
    do {
        cout << ">> ";
        cin >> choice;
    } while (choice < 0 || choice > 1);

    if (choice == 1) {
        highScores.clear();
        ofstream file("highscore.txt", ios::trunc);
        file.close();
        cout << "High Score telah direset!" << endl;
        Sleep(1000);
    }
}

void saveGame() {
    ofstream file("savegame.txt");
    if (file.is_open()) {
        file << gold << " " << shield << " " << health << " " << doublegold << endl;
        file.close();
    }
}

void loadGame() {
    ifstream file("savegame.txt");
    if (file.is_open()) {
        file >> gold >> shield >> health >> doublegold;
        file.close();
    }
}

int main() {
    loadHighScores();

    int newOrLoad;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    do {
        cout << ">> ";
        cin >> newOrLoad;
    } while (newOrLoad < 1 || newOrLoad > 2);

    if (newOrLoad == 2) {
        loadGame();
    } else {
        gold = 100;
        shield = 0;
        health = 0;
        doublegold = 0;
    }

    do {
        system("cls");
        cout << "=== SPACE INVADERS ===" << endl;
        cout << "Gold : " << gold << endl;
        cout << "1. Play" << endl << "2. Shop" << endl << "3. High Score" << endl << "4. Check Item" << endl << "0. Exit" << endl;
        do {
            cout << ">> ";
            cin >> menu;
        } while (menu < 0 || menu > 4);

        if (menu == 1) {
            system("cls");
            runGame();
            system("pause");
        } else if (menu == 2) {
            do {
                system("cls");
                cout << "=== SHOP ===" << endl;
                cout << "Gold : " << gold << endl;
                cout << "1. Shield : 10 Gold     (Jumlah Item : " << shield << ")" << endl;
                cout << "2. Health : 15 Gold     (Jumlah Item : " << health << ")" << endl;
                cout << "3. Double Gold : 12 Gold  (Jumlah Item : " << doublegold << ")" << endl;
                cout << "0. Exit" << endl;
                do {
                    cout << ">> ";
                    cin >> shop;
                } while (shop < 0 || shop > 3);

                if (shop == 1) {
                    if (gold >= 10) {
                        shield += 1;
                        gold -= 10;
                        cout << "Berhasil membeli " << item[0] << endl;
                    } else {
                        cout << "Gold tidak cukup!" << endl;
                    }
                } else if (shop == 2) {
                    if (gold >= 15) {
                        health += 1;
                        gold -= 15;
                        cout << "Berhasil membeli " << item[1] << endl;
                    } else {
                        cout << "Gold tidak cukup!" << endl;
                    }
                } else if (shop == 3) {
                    if (gold >= 12) {
                        doublegold += 1;
                        gold -= 12;
                        cout << "Berhasil membeli " << item[2] << endl;
                    } else {
                        cout << "Gold tidak cukup!" << endl;
                    }
                }
                Sleep(1000);
            } while (shop != 0);
        } else if (menu == 3) {
            showHighScores();
        } else if (menu == 4) {
            system("cls");
            cout << "=== CHECK ITEM ===" << endl;
            bool adaItem = false;

            for (int i = 0; i < 3; i++) {
                int jumlahItem = 0;
                if (i == 0) jumlahItem = shield;
                else if (i == 1) jumlahItem = health;
                else if (i == 2) jumlahItem = doublegold;

                if (jumlahItem > 0) {
                    cout << item[i] << " : " << jumlahItem << endl;
                    adaItem = true;
                }
            }
            if (!adaItem) {
                cout << "Anda belum memiliki item." << endl;
            }
            system("pause");
        }

    } while (menu != 0);

    if (menu == 0) {
        char save;
        do {
            cout << "Apakah Anda ingin menyimpan progress? (y/n): ";
            cin >> save;
            if (save != 'y' && save != 'Y' && save != 'n' && save != 'N') {
                cout << "Input tidak valid! Masukkan 'y' atau 'n'." << endl;
            }
        } while (save != 'y' && save != 'Y' && save != 'n' && save != 'N');

        if (save == 'y' || save == 'Y') {
            saveGame();
            cout << "Progress telah disimpan!" << endl;
        } else {
            cout << "Progress tidak disimpan." << endl;
        }
        Sleep(1000);
    }

    return 0;
}
