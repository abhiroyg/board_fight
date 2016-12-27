#include "Board.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Board :: Board() {initialize();}

BoardEntry* Board :: randomBoardEntry() {
    //Empty, Role, Item
    int entry = rand() % 3;
    if (entry == 0) {
        return new EmptyBoardEntry;
    } else if (entry == 1) {
        return new Role((rand() % 5) + 1);
    } else {
        return new Item((rand() % 8) + 1);
    }
}

void Board :: print_current_location_information() {
    EmptyBoardEntry* empty = dynamic_cast<EmptyBoardEntry*> (board[cur_row][cur_col]);
    if (empty != 0) {
        cout << "This location is empty." << endl;
        return;
    }

    Role* role = dynamic_cast<Role*> (board[cur_row][cur_col]);
    if (role != 0) {
        cout << "This location has an enemy:" << endl;
        role->print_current_stats();
        return;
    }

    Item* item = dynamic_cast<Item*> (board[cur_row][cur_col]);
    if (item != 0) {
        cout << "This location has an item: " << item->get_name() << endl;
        return;
    }

    cout << "Unrecognized entity." << endl;
}

bool Board :: is_empty() {
    EmptyBoardEntry* empty = dynamic_cast<EmptyBoardEntry*> (board[cur_row][cur_col]);
    if (empty != 0) {
        return true;
    }
    return false;
}

void Board :: initialize() {
    cout << ":::::::: Please input board size ::::::::" << endl;
    cout << "Number of rows: ";
    cin >> num_rows;
    cout << "Number of columns: ";
    cin >> num_columns;

    if (num_rows <= 0 || num_columns <= 0) {
        cout << "Invalid board size. Terminating game.";
        return;
    }

    cout << ":::::::: Please choose your character ::::::::" << endl;
    cout << "1: Human" << endl;
    cout << "2: Elf" << endl;
    cout << "3: Dwarf" << endl;
    cout << "4: Hobbit" << endl;
    cout << "5: Orc" << endl;
    cout << "6: Exit game" << endl;
    cout << "Your choice: ";
    int choice;
    cin >> choice;

    if (choice == 6) {
        return;
    }

    hero = new Role(choice);

    board = vector< vector<BoardEntry *> >(num_rows, vector<BoardEntry *>(num_columns));

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_columns; j++) {
            if (i == 0 && j == 0) {
                board[i][j] = new EmptyBoardEntry;
            } else {
                board[i][j] = randomBoardEntry();
            }
        }
    }

    cur_row = 0;
    cur_col = 0;

    int num_turns = 0;
    bool daytime = true;
    for(;;) {
        cout << endl;
        num_turns++;

        if (num_turns % 5 == 0) {
            daytime = !daytime;

            if (daytime) {
                cout << "A new day has dawned upon us." << endl;
            } else {
                cout << "The sun has set. Time for the night crawlers to roam." << endl;
            }
        }

        cout << "You are currently on the board at (" << cur_row << ", " << cur_col << ")." << endl;

        // Optional: Valid/Invalid movement patterns.

        hero->print_current_stats();

        cout << ":::::::: Please choose an action ::::::::" << endl;
        cout << "Move (N)orth, (S)outh, (E)ast, (W)est or" << endl;
        cout << "(A)ttack, (P)ickup, (D)rop, (L)ook, (I)nventory or E(x)it" << endl;
        cout << "Your choice (case in-sensitive): ";
        char action;
        cin >> action;

        switch (action) {
            case 'N':
            case 'n':
                cout << endl;
                if (cur_row > 0 && cur_row < num_rows) {
                    cur_row -= 1;
                } else if (cur_row == 0) {
                    cout << "You are already at the top. You can't move further high." << endl;
                }
                print_current_location_information();
                break;
            case 'S':
            case 's':
                cout << endl;
                if (cur_row >= 0 && cur_row <= num_rows-2) {
                    cur_row += 1;
                } else if (cur_row == num_rows-1) {
                    cout << "You are already at the bottom. You can't move further deep." << endl;
                }
                print_current_location_information();
                break;
            case 'E':
            case 'e':
                cout << endl;
                if (cur_col >= 0 && cur_col <= num_columns-2) {
                    cur_col += 1;
                } else if (cur_col == num_columns-1) {
                    cout << "You are already at furthest right. You can't go any further." << endl;
                }
                print_current_location_information();
                break;
            case 'W':
            case 'w':
                cout << endl;
                if (cur_col > 0 && cur_col < num_columns) {
                    cur_col -= 1;
                } else if (cur_col == 0) {
                    cout << "You are already at furthest left. You can't go any further." << endl;
                }
                print_current_location_information();
                break;
            case 'A':
            case 'a':
                {
                    cout << endl;
                    Role* enemy = dynamic_cast<Role*>(board[cur_row][cur_col]);
                    if (enemy == 0) {
                        cout << "There is no enemy here. You can't attack." << endl;
                    } else if (hero->confront(*enemy, daytime)) {
                        // If hero defeats enemy, empty the board in that position.
                        board[cur_row][cur_col] = new EmptyBoardEntry;
                    }
                }
                break;
            case 'P':
            case 'p':
                {
                    cout << endl;
                    Item* item = dynamic_cast<Item*>(board[cur_row][cur_col]);
                    if (item == 0) {
                        cout << "There is no item here to pick." << endl;
                    } else if (hero->pick(*item)) {
                        // If hero is successfully able to pick the item.
                        board[cur_row][cur_col] = new EmptyBoardEntry;
                        cout << "Picked " << item->get_name() << " from (" << cur_row << ", " << cur_col << ")." << endl;
                    } /* else {
                        cout << "Could not pick the item." << endl;
                    } */
                }
                break;
            case 'D':
            case 'd':
                cout << endl;
                if (is_empty()) {
                    try {
                        Item item = hero->drop_item();
                        board[cur_row][cur_col] = &item;
                        cout << "Dropped " << item.get_name() << " at (" << cur_row << ", " << cur_col << ")." << endl;
                    } catch (const char* msg) {
                        cout << msg << endl;
                    }
                } else {
                    cout << "This place is not empty. You may not drop here." << endl;
                }
                break;
            case 'L':
            case 'l':
                cout << endl;
                print_current_location_information();
                break;
            case 'I':
            case 'i':
                cout << endl;
                hero->print();
                break;
            case 'X':
            case 'x':
                return;
        }
    }
}

Board :: ~Board() {}
