#include "BoardEntry.h"
#include <iostream>
#include <cstdlib>
#include <iterator>

using namespace std;

Role :: Role() {
    current_weight = 0;
    amount_of_gold = 0;
}

Role :: Role(int choice) {
    current_weight = 0;
    amount_of_gold = 0;
    switch (choice) {
        case 1:
            initialize_human();
            break;
        case 2:
            initialize_elf();
            break;
        case 3:
            initialize_dwarf();
            break;
        case 4:
            initialize_hobbit();
            break;
        case 5:
            initialize_orc();
            break;
    }
}

void Role :: initialize_human() {
    name = "human";
    attack = 30;
    attack_chance = 2/3.0;
    defence = 20;
    defence_chance = 1/2.0;
    health = 60;
    strength = 100 ;
}

void Role :: initialize_elf() {
    name = "elf";
    attack = 40;
    attack_chance = 1/1.0;
    defence = 10;
    defence_chance = 1/4.0;
    health = 40;
    strength = 70;
}

void Role :: initialize_dwarf() {
    name = "dwarf";
    attack = 30;
    attack_chance = 2/3.0;
    defence = 20;
    defence_chance = 2/3.0;
    health = 50;
    strength = 130;
}

void Role :: initialize_hobbit() {
    name = "hobbit";
    attack = 25;
    attack_chance = 1/3.0;
    defence = 20;
    defence_chance = 2/3.0;
    health = 70;
    strength = 85;
}

void Role :: initialize_orc() {
    name = "orc";
    attack = 25;
    attack_chance = 1/4.0;
    defence = 10;
    defence_chance = 1/4.0;
    health = 50;
    strength = 130;
}

int Role :: get_attack(bool daytime) {
    if (name == "orc" && daytime) {
        return 25;
    } else if (name == "orc" && !daytime) {
        return 45;
    }
    return attack;
}

double Role :: get_attack_chance(bool daytime) {
    if (name == "orc" && daytime) {
        return 1/4.0;
    } else if (name == "orc" && !daytime) {
        return 1/1.0;
    }
    return attack_chance;
}

int Role :: get_defence(bool daytime) {
    if (name == "orc" && daytime) {
        return 10;
    } else if (name == "orc" && !daytime) {
        return 25;
    }
    return defence;
}

double Role :: get_defence_chance(bool daytime) {
    if (name == "orc" && daytime) {
        return 1/4.0;
    } else if (name == "orc" && !daytime) {
        return 1/2.0;
    }
    return defence_chance;
}

int Role :: get_health() {
    return health;
}

void Role :: on_successful_defence(int damage, bool daytime) {
    if (name == "elf") {
        health += 1;
    } else if (name == "orc" && daytime) {
        health -= damage/4;
    } else if (name == "org" && !daytime) {
        health += 1;
    } else if (name == "hobbit") {
        health -= rand()%6;
    }
}

void Role :: take_damage(int damage) {
    health -= damage;
}

Role Role :: random_role() {
    return Role((rand() % 5) + 1);
}

void Role :: print_current_stats() {
    cout << name << "; ";
    cout << "Attack = " << attack << "; ";
    cout << "Defence = " << defence << "; ";
    cout << "Health = " << health << "; ";
    cout << "Item weight = " << current_weight << "/" << strength;
    cout << endl;
}

void Role :: add_items_effects() {
    // only attack and defence effects
    // Use only if you completely overwritten already
    // added effects
    vector<Item> :: iterator it;
    for (it=inventory.begin(); it != inventory.end(); it++) {
        attack += it->get_attack_change();
        defence += it->get_defence_change();
    }
}

int Role :: confront(Role enemy, bool daytime) {
    if (name == "orc" && daytime) {
        attack = 25;
        attack_chance = 1/4.0;
        defence = 10;
        defence_chance = 1/4.0;
        add_items_effects();
    } else if (name == "orc" && !daytime) {
        attack = 45;
        attack_chance = 1/1.0;
        defence = 25;
        defence_chance = 1/2.0;
        add_items_effects();
    }

    //1 - enemy is at 0 health
    //0 - no one is at 0 health
    //-1 - we are at 0 health
    double prob = (double)rand() / RAND_MAX;
    int damage = 0;
    if (prob <= attack_chance) {
        // Our attack will go through
        prob = (double)rand() / RAND_MAX;
        if (prob <= enemy.get_defence_chance(daytime)) {
            // Their defence went through
            damage = attack - enemy.get_defence(daytime);
            enemy.on_successful_defence(damage, daytime);
            if (enemy.get_health() <= 0) {
                amount_of_gold += enemy.get_defence(daytime);
                return 1;
            }
        } else {
            // Their defence failed
            damage = attack - enemy.get_defence(daytime);
            enemy.take_damage(damage);
            if (enemy.get_health() <= 0) {
                amount_of_gold += enemy.get_defence(daytime);
                return 1;
            }
        }
    }

    prob = (double)rand() / RAND_MAX;
    damage = 0;
    if (prob <= enemy.get_attack_chance(daytime)) {
        // Their attack will go through
        prob = (double)rand() / RAND_MAX;
        if (prob <= defence_chance) {
            // Our defence went through
            damage = enemy.get_attack(daytime) - defence;
            on_successful_defence(damage, daytime);
            if (health <= 0) {
                return -1;
            }
        } else {
            // Our defence failed
            damage = enemy.get_attack(daytime) - defence;
            take_damage(damage);
            if (health <= 0) {
                return -1;
            }
        }
    }
    return 0;
}

bool Role :: pick(Item item) {
    if (available_categories.find(item.get_category()) != available_categories.end()) {
        if (item.get_weight() + current_weight <= strength) {
            inventory.push_back(item);
            current_weight += item.get_weight();
            available_categories.insert(item.get_category());
            attack += item.get_attack_change();
            defence += item.get_defence_change();
            health += item.get_health_change();
            strength += item.get_strength_change();
            return true;
        } else {
            cout << "Cannot pick the item as it is beyond the weight you can carry." << endl;
            return false;
        }
    } else {
        cout << "Cannot pick the item as you already have an item of category: " << item.get_category() << endl;
        return false;
    }
}

void Role :: print() {
    //Prints out inventory and gold.
    cout << "You have " << amount_of_gold << " gold" << endl;
    cout << ":::: Inventory ::::" << endl;
    vector<Item> :: iterator it;
    for (it=inventory.begin(); it != inventory.end(); it++) {
        cout << it->get_name() << ", " << "weight " << it->get_weight() << endl;
    }
}

Item Role :: choose_item() {
    cout << ":::: Inventory ::::" << endl;
    vector<Item> :: iterator it;
    int i = 1;
    for (it=inventory.begin(); it != inventory.end(); it++, i++) {
        cout << i << ". " << it->get_name() << ", " << "weight " << it->get_weight() << endl;
    }
    cout << "Your choice: ";
    int choice;
    cin >> choice;
    return inventory[choice-1];
}

Item :: Item() {
    attack_change = 0;
    defence_change = 0;
    health_change = 0;
    strength_change = 0;
}

Item :: Item(int choice) {
    attack_change = 0;
    defence_change = 0;
    health_change = 0;
    strength_change = 0;

    switch (choice) {
        case 1:
            initialize_sword();
            break;
        case 2:
            initialize_dagger();
            break;
        case 3:
            initialize_plate_armour();
            break;
        case 4:
            initialize_leather_armour();
            break;
        case 5:
            initialize_large_shield();
            break;
        case 6:
            initialize_small_shield();
            break;
        case 7:
            initialize_ring_of_life();
            break;
        case 8:
            initialize_ring_of_strength();
            break;
    }
}

void Item :: initialize_sword() {
    name = "sword";
    category = "weapon";
    weight = 10;
    attack_change = 10;
}

void Item :: initialize_dagger() {
    name = "dagger";
    category = "weapon";
    weight = 5;
    attack_change = 5;
}

void Item :: initialize_plate_armour() {
    name = "plate armour";
    category = "armour";
    weight = 40;
    defence_change = 10;
    attack_change = -5;
}

void Item :: initialize_leather_armour() {
    name = "leather armour";
    category = "armour";
    weight = 20;
    defence_change = 5;
}

void Item :: initialize_large_shield() {
    name = "large shield";
    category = "shield";
    weight = 30;
    defence_change = 10;
    attack_change = -5;
}

void Item :: initialize_small_shield() {
    name = "small shield";
    category = "shield";
    weight = 10;
    defence_change = 5;
}

void Item :: initialize_ring_of_life() {
    name = "ring of life";
    category = "ring";
    weight = 1;
    health_change = 10;
}

void Item :: initialize_ring_of_strength() {
    name = "ring of strength";
    category = "ring";
    weight = 1;
    strength_change = 50;
    health_change = -10;
}

string Item :: get_name() {
    return name;
}

string Item :: get_category() {
    return category;
}

int Item :: get_weight() {
    return weight;
}

int Item :: get_attack_change() {
    return attack_change;
}

int Item :: get_defence_change() {
    return defence_change;
}

int Item :: get_health_change() {
    return health_change;
}

int Item :: get_strength_change() {
    return strength_change;
}

Item Item :: random_item() {
    return Item((rand() % 8) + 1);
}
