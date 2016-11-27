#ifndef BOARD_H
#define BOARD_H

#include "BoardEntry.h"
#include <vector>

class Board {
    private:
        int num_rows;
        int num_columns;
        std::vector< std::vector<BoardEntry *> > board;
        Role* hero;
        int cur_row;
        int cur_col;
        BoardEntry* randomBoardEntry();
        void print_current_location_information();
        bool is_empty();

    public:
        Board();
        void initialize();
        ~Board();
};

#endif  // BOARD_H
