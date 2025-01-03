#include <iostream>

using namespace std;

int gold = 100;
int menu, shield = 0, health = 0, doublegold = 0, shop;
string item[3] = {"Shield", "Health", "Double Gold"};

int main() {
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
                } else if (shop != 0) {
                    cout << "Input tidak valid" << endl;
                }
            } while (shop != 0);
        } else if (menu == 3) {

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
        }else if (menu != 0) {
            cout << "Input tidak valid" << endl;
        }

    } while (menu != 0);

    return 0;
}
