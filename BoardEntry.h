#ifndef BOARDENTRY_H
#define BOARDENTRY_H

#include <vector>
#include <set>
#include <string>

class BoardEntry {
    public:
        BoardEntry();
        virtual ~BoardEntry();
};

class EmptyBoardEntry : public BoardEntry {
    public:
        EmptyBoardEntry();
        ~EmptyBoardEntry();
};

class Item : public BoardEntry {
    private:
        std::string name;
        std::string category;
        int weight;
        int attack_change;
        int defence_change;
        int health_change;
        int strength_change;
        void initialize_sword();
        void initialize_dagger();
        void initialize_plate_armour();
        void initialize_leather_armour();
        void initialize_large_shield();
        void initialize_small_shield();
        void initialize_ring_of_life();
        void initialize_ring_of_strength();

    public:
        Item();
        Item(int);
        std::string get_name();
        std::string get_category();
        int get_weight();
        int get_attack_change();
        int get_defence_change();
        int get_health_change();
        int get_strength_change();
        ~Item();
};

class Role : public BoardEntry {
    private:
        std::string name;
        int attack;
        double attack_chance;
        int defence;
        double defence_chance;
        int health;
        int strength;
        std::vector<Item> inventory;
        int current_weight;
        int amount_of_gold;
        std::set<std::string> available_categories;
        void initialize_human();
        void initialize_elf();
        void initialize_dwarf();
        void initialize_hobbit();
        void initialize_orc();
        void add_items_effects();

    public:
        Role();
        Role(int);
        int get_attack(bool);
        double get_attack_chance(bool);
        int get_defence(bool);
        double get_defence_chance(bool);
        int get_health();
        void print_current_stats();
        int confront(Role, bool);
        void take_damage(int);
        void on_successful_defence(int, bool);
        bool pick(Item);
        void print();
        Item choose_item();
        ~Role();
};

#endif  // BOARDENTRY_H
