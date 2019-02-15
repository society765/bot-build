#include <stack>

#include "checkers.hpp"

using namespace std;

set<vector<POINT>> checkers::get_avail_move(char player) {
    set<vector<POINT>> cap;
    set<vector<POINT>> nocap;

    char rival = 'w' + 'b' - player;
    auto board_copy = board;
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            int c = board[i][j];

            if (c == BLANK || c == rival || c == toupper(rival)) continue;
            POINT cur = {i, j};

            get_next_nocap(cur, nocap, player);
            vector<POINT> cur_vec = {cur};
            get_next_cap(cur, cap, cur_vec, player);
            board = board_copy;
        }
    }

    if (cap.empty())
        return nocap;
    else
        return cap;
}

void checkers::get_next_cap(const POINT& pt, set<vector<POINT>>& cap,
                            vector<POINT>& cur_vec, char player) {
    char rival = 'w' + 'b' - player;
    vector<POINT> cap_vec;
    if (getvalue(pt) == player) {
        cap_vec =
            exist_capturing_handling(pt, forward_dir_vec[player], 2, rival);
    } else if (getvalue(pt) == towupper(WHOSTURN)) {
        cap_vec = exist_capturing_handling(pt, all_dir_vec, 4, rival);
    } else {
        return;
    }

    if (cap_vec.empty()) {
        if (cur_vec.size() != 1) cap.insert(cur_vec);
        return;
    }

    for (int i = 0; i < cap_vec.size(); i++) {
        POINT next = cap_vec[i];
        POINT next_next = next + next - pt;

        char c_cur = getvalue(pt);
        char rival_cur = getvalue(next);
        if ((next_next.first == 0 && player == 'w') ||
            (next_next.first == board_size - 1 && player == 'b')) {
            setvalue(next_next) = toupper(getvalue(pt));
        } else {
            setvalue(next_next) = getvalue(pt);
        }
        setvalue(pt) = BLANK;
        setvalue(next) = BLANK;
        cur_vec.push_back(next_next);
        get_next_cap(next_next, cap, cur_vec, player);
        cur_vec.pop_back();
        setvalue(next) = rival_cur;
        setvalue(pt) = c_cur;
        setvalue(next_next) = BLANK;
    }
}

void checkers::get_next_nocap(const POINT& pt, set<vector<POINT>>& nocap,
                              char player) {
    if (getvalue(pt) == player) {
        for (int i = 0; i < 2; i++) {
            POINT next = pt + forward_dir_vec[player][i];
            if (!is_inboundary(next)) continue;
            if (getvalue(next) == BLANK) nocap.insert({pt, next});
        }
    } else if (getvalue(pt) == toupper(player)) {
        for (int i = 0; i < 4; i++) {
            POINT next = pt + all_dir_vec[i];
            if (!is_inboundary(next)) continue;
            if (getvalue(next) == BLANK) nocap.insert({pt, next});
        }
    } else
        return;
}

void checkers::move_from_vp(const vector<POINT>& vp, char player) {
    for (int i = 1; i < vp.size(); i++) {
        POINT last = vp[i - 1];
        POINT cur = vp[i];

        setvalue(cur) = getvalue(last);
        setvalue(last) = BLANK;
        if (abs(cur.first - last.first) != 1) {
            // cap move
            setvalue((cur + last) / 2) = BLANK;
        }

        if ((cur.first == 0 && player == 'w') ||
            (cur.first == board_size - 1 && player == 'b')) {
            setvalue(cur) = toupper(getvalue(cur));
        }
    }
}