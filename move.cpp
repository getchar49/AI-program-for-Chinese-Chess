#include<iostream>
#include<stdio.h>
#include<vector>
#include<algorithm>
using namespace std;

extern int field[11][10];
extern int record[300];
extern int chase_record[300];
extern int turns;

extern vector<int> valid_moves; // Ban ghi cac nuoc di hop le

struct pos{
public:
	int i;
	int j;
	pos(int i_ = 0, int j_ = 0): i(i_), j(j_){};
	friend bool operator == (const pos& x, const pos& y){
		return (x.i == y.i) && (x.j == y.j) && (x.i != 0) && (x.j != 0);
	};
};

int get_color(int id){
	if(!id)
		return -1; //-1 = Ko co gi
	if(id < 10)
		return 1; //1 = Do
	return 0; //0 = Den
}

inline int valid_pos(int i, int j){
	if(i > 0 && i <= 10 && j > 0 && j <= 9)
		return 1;
	return 0;
}

bool bailianjiang(){ // Kiem tra xem ban co co roi vao trang thai vua doi mat nhau khong
	int black_i = 0, black_j = 0;
	for(int i = 1; i <= 3; ++i){
		for(int j = 4; j <= 6; ++j){
			if(field[i][j] == 11){
				black_i = i;
				black_j = j;
			}
		}
	}
	int red_i = 0, red_j = 0;
	for(int i = 8; i <= 10; ++i){
		for(int j = 4; j <= 6; ++j){
			if(field[i][j] == 1){
				red_i = i;
				red_j = j;
			}
		}
	}
	if(red_j != black_j)
		return 0;
	for(int i = black_i + 1; i < red_i; ++i){
		if(field[i][red_j])
			return 0;
	}
	return 1;
}

int recur(int director, bool king){ // Kiem tra vong lap
	if (king)
		return 0;
	if(turns > 16){
		if((record[turns - 1] == record[turns - 5])
			//&& (record[turns - 1] == record[turns - 9])
			&& (record[turns - 2] == record[turns - 6])
			//&& (record[turns - 2] == record[turns - 10])
			&& (record[turns - 3] == record[turns - 7])
			//&& (record[turns - 3] == record[turns - 11])
			&& (director == record[turns - 4]))
			//&& (director == record[turns - 8]))
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

int push(int color, int type, int from_i, int from_j, int to_i, int to_j, bool king, bool check_long_chase){
	int move_with_info(int director);
	int move(int director);
	if(valid_pos(to_i,to_j) && get_color(field[to_i][to_j]) != color){
		int save[11][10];
		for(int i = 0; i <= 10; ++i){
			for(int j = 0; j <= 9; ++j)
				save[i][j] = field[i][j];
		}
		int director = (from_i << 12) + (from_j << 8) + (to_i << 4) + to_j;
		move(director);
		bool invalid = 0;
		if(bailianjiang())
			invalid = 1;
		for(int i = 0; i <= 10; ++i){
			for(int j = 0; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
		if(!invalid){
			if(recur(director, king))
				return -1;
			if ((check_long_chase) && (turns > 18)){
				if (chase_record[turns - 2] && chase_record[turns - 4] &&
					chase_record[turns - 6] && chase_record[turns - 8] &&
					chase_record[turns - 10] && chase_record[turns - 12] &&
					chase_record[turns - 14]) {
					if ((move_with_info(director) == 24) && (!king)) {
						for (int i = 0; i <= 10; ++i) {
							for (int j = 0; j <= 9; ++j)
								field[i][j] = save[i][j];
						}
						return -1;
					}
					for (int i = 0; i <= 10; ++i) {
						for (int j = 0; j <= 9; ++j)
							field[i][j] = save[i][j];
					}
				}
			}
			valid_moves.push_back(director);
		}
		return invalid;
	}
	return -1;
}

void king(int i, int j, int color, bool check_long_chase){
	int di[4] = {-1,0,1,0};
	int dj[4] = {0,1,0,-1};
	for(int k = 0; k < 4; ++k){
		int to_i = i + di[k], to_j = j + dj[k];
		if((to_i > 3 && to_i < 8) || to_j < 4 || to_j > 6)
			continue;
		push(color,1,i,j,to_i,to_j,1,check_long_chase);
	}
	return;
}

void guard(int i, int j, int color, bool check_long_chase){
	int di[4] = {-1,-1,1,1};
	int dj[4] = {-1,1,1,-1};
	for(int k = 0; k < 4; ++k){
		int to_i = i + di[k], to_j = j + dj[k];
		if((to_i > 3 && to_i < 8) || to_j < 4 || to_j > 6)
			continue;
		push(color,2,i,j,to_i,to_j,0,check_long_chase);
	}
	return;
}

void bishop(int i, int j, int color, bool check_long_chase){
	int di[4] = {-2,-2,2,2};
	int dj[4] = {-2,2,2,-2};
	int obi[4] = {-1,-1,1,1}; // Vat can
	int obj[4] = {-1,1,1,-1};
	for(int k = 0; k < 4; ++k){
		int to_i = i + di[k], to_j = j + dj[k];
		int by_i = i + obi[k], by_j = j + obj[k]; //  //Kiem tra xem co bi can khong
		if(valid_pos(by_i,by_j) && !field[by_i][by_j]){
			if((color && to_i > 5) || (!color && to_i < 6)) // Tuong khong duoc vuot song
				push(color,3,i,j,to_i,to_j,0, check_long_chase);
		}
	}
	return;
}

void rook(int i, int j, int color, bool check_long_chase){
	int di[4] = {-1,0,1,0};
	int dj[4] = {0,1,0,-1};
	for(int k = 0; k < 4; ++k){
		int to_i = i + di[k], to_j = j + dj[k];
		while(valid_pos(to_i,to_j)){
			if(get_color(field[to_i][to_j]) == color)
				break;
			if(get_color(field[to_i][to_j]) == 1 - color){
				push(color,4,i,j,to_i,to_j,0, check_long_chase);
				break;
			}
			push(color,4,i,j,to_i,to_j,0, check_long_chase);
			to_i += di[k];
			to_j += dj[k];
		}
	}
	return;
}

void knight(int i, int j, int color, bool check_long_chase){
	int di[8] = {-2,-1,1,2,2,1,-1,-2};
	int dj[8] = {1,2,2,1,-1,-2,-2,-1};
	int obi[8] = {-1,0,0,1,1,0,0,-1}; // Vat can
	int obj[8] = {0,1,1,0,0,-1,-1,0};
	for(int k = 0; k < 8; ++k){
		int to_i = i + di[k], to_j = j + dj[k];
		int by_i = i + obi[k], by_j = j + obj[k]; // Kiem tra vat can
		if(valid_pos(by_i,by_j) && !field[by_i][by_j])
			push(color,5,i,j,to_i,to_j,0, check_long_chase);
	}
	return;
}

void cannon(int i, int j, int color, bool check_long_chase){
	int di[4] = {-1,0,1,0};
	int dj[4] = {0,1,0,-1};
	for(int k = 0; k < 4; ++k){
		int to_i = i + di[k], to_j = j + dj[k];
		int by = 0; //come by
		while(valid_pos(to_i,to_j)){
			if(by == 0){
				if(get_color(field[to_i][to_j]) == -1)
					push(color,6,i,j,to_i,to_j,0, check_long_chase);
				else
					by = 1;
			}
			else if(by == 1){
				if(get_color(field[to_i][to_j]) == 1 - color){
					push(color,6,i,j,to_i,to_j,0, check_long_chase);
					break;
				}
				else if(get_color(field[to_i][to_j]) == color)
					break;
			}
			to_i += di[k];
			to_j += dj[k];
		}
	}
	return;
}

void pawn(int i, int j, int color, bool check_long_chase){
	if(color){
		if(i > 5)
			push(color,7,i,j,i - 1,j,0, check_long_chase);
		else{
			push(color,7,i,j,i - 1,j,0, check_long_chase);
			push(color,7,i,j,i,j + 1,0, check_long_chase);
			push(color,7,i,j,i,j - 1,0, check_long_chase);
		}
	}
	else{
		if(i < 6)
			push(color,7,i,j,i + 1,j,0, check_long_chase);
		else{
			push(color,7,i,j,i + 1,j,0, check_long_chase);
			push(color,7,i,j,i,j + 1,0, check_long_chase);
			push(color,7,i,j,i,j - 1,0, check_long_chase);
		}
	}
	return;
}

int get_valid_moves(int color, bool check_long_chase = 1){ // Sinh ra tat ca cac nuoc di hop le, luu vao mang valid_moves
	valid_moves.clear();
	for(int i = 1; i <= 10; ++i){
		for(int j = 1; j <= 9; ++j){
			if(field[i][j]){
				if(color == get_color(field[i][j])){
					int type = field[i][j] % 10;
					switch(type){
						case 1: king(i,j,color, check_long_chase); break;
						case 2: guard(i,j,color, check_long_chase); break;
						case 3: bishop(i,j,color, check_long_chase); break;
						case 4: rook(i,j,color, check_long_chase); break;
						case 5: knight(i,j,color, check_long_chase); break;
						case 6: cannon(i,j,color, check_long_chase); break;
						case 7: pawn(i,j,color, check_long_chase); break;
					}
				}
			}
		}
	}
	return 0;
}

int move(int director){ // Tham so: Thong tin nuoc di. Tra ve: Thong tin quan co tai vi tri dich
	int i = 4;
	int dgt[4] = {0, 0, 0, 0};
	while(i){
		--i;
		dgt[i] = (director >> ((3 - i) << 2)) & 15;
	}
	int ret = field[dgt[2]][dgt[3]];
	field[dgt[2]][dgt[3]] = field[dgt[0]][dgt[1]];
	field[dgt[0]][dgt[1]] = 0;
	return ret;
}

int move_with_info(int director) { // Tham so: Thong tin nuoc di. Tra ve: Thong tin quan co tai vi tri dich. Hoac 20 neu thuc hien long catch.
	int i = 4;
	int dgt[4] = { 0, 0, 0, 0 };
	while (i) {
		--i;
		dgt[i] = (director >> ((3 - i) << 2)) & 15;
	}
	int ret = field[dgt[2]][dgt[3]];
	field[dgt[2]][dgt[3]] = field[dgt[0]][dgt[1]];
	field[dgt[0]][dgt[1]] = 0;
	if (!ret) {
		int save[11][10];
		for (int i = 0; i < 11; ++i) {
			for (int j = 1; j < 10; ++j)
				save[i][j] = field[i][j];
		}
		vector<int> save_valid_moves = valid_moves;
		valid_moves.clear();
		switch (field[dgt[2]][dgt[3]]) {
		case 1: king(dgt[2], dgt[3], 1, 0); break;
		case 2: guard(dgt[2], dgt[3], 1, 0); break;
		case 3: bishop(dgt[2], dgt[3], 1, 0); break;
		case 4: rook(dgt[2], dgt[3], 1, 0); break;
		case 5: knight(dgt[2], dgt[3], 1, 0); break;
		case 6: cannon(dgt[2], dgt[3], 1, 0); break;
		case 7: pawn(dgt[2], dgt[3], 1, 0); break;
		case 11: king(dgt[2], dgt[3], 0, 0); break;
		case 12: guard(dgt[2], dgt[3], 0, 0); break;
		case 13: bishop(dgt[2], dgt[3], 0, 0); break;
		case 14: rook(dgt[2], dgt[3], 0, 0); break;
		case 15: knight(dgt[2], dgt[3], 0, 0); break;
		case 16: cannon(dgt[2], dgt[3], 0, 0); break;
		case 17: pawn(dgt[2], dgt[3], 0, 0); break;
		}
		while (!valid_moves.empty()) {
			int director = valid_moves.back();
			valid_moves.pop_back();
			if (move(director)) {
				ret = 20;
				break;
			}
		}
		// Thuc hien backtrack
		for (int i = 0; i < 11; ++i) {
			for (int j = 1; j < 10; ++j)
				field[i][j] = save[i][j];
		}
		valid_moves = save_valid_moves;
	}
	return ret;
}

int player_move(int cmd){
	int i = 4;
	int dgt[4] = {0, 0, 0, 0};
	bool b = 0;
	if(cmd < 1000)
		b = 1;
	while(i){
		--i;
		dgt[i] = cmd % 10;
		cmd -= dgt[i];
		cmd /= 10;
		if(!dgt[i])
			dgt[i] = 10;
	}
	if(b)
		dgt[0] = 10;
	int director = (dgt[0] << 12) + (dgt[1] << 8) + (dgt[2] << 4) + dgt[3];
	return director;
}
