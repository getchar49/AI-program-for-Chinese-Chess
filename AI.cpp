
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<map>
#include<queue>
#include<set>
#include<algorithm>

#include"head.h"
using namespace std;

int cnn_score(int color);

const char punctuation = ',';

extern int field[11][10];
extern vector<int> valid_moves;
int deep_limit = 4;
int cuttab_index = 0;
extern int turns;

int deep_limit_cnt = 0;
int eval = 0;

const bool debug = 0;
const bool debug2 = 1;

const bool cache = 0;	// 1 neu su dung rectab cache

const int time_limit_k = 3900000;
const int time_limit_t = 5;
float eat_k = 0.3;

const int cuttabR = 17;
const int cuttabJ = 10;
const float cuttab[cuttabR][13] = {
 1, 1, 1.00, 1.00, 1.00, 0.04, 0.05, 0.05, 0.05, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.06, 0.06, 0.06, 0.06, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.08, 0.08, 0.08, 0.08, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.10, 0.10, 0.10, 0.12, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.12, 0.12, 0.15, 0.20, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.17, 0.17, 0.20, 0.30, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.22, 0.22, 0.25, 0.40, 0.50, 0.50, 0.70, 0.70,
 1, 1, 1.00, 1.00, 1.00, 0.30, 0.30, 0.30, 0.50, 0.55, 0.55, 0.75, 0.75,
 1, 1, 1.00, 1.00, 1.00, 0.40, 0.40, 0.40, 0.60, 0.60, 0.60, 0.80, 0.80,
 1, 1, 1.00, 1.00, 1.00, 0.50, 0.50, 0.50, 0.65, 0.65, 0.65, 0.82, 0.82,
 1, 1, 1.00, 1.00, 1.00, 0.60, 0.60, 0.60, 0.70, 0.70, 0.70, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.70, 0.70, 0.70, 0.70, 0.70, 0.70, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.80, 0.80, 0.80, 0.80, 0.80, 0.80, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.90, 0.90, 0.90, 0.90, 0.90, 0.90, 0.90, 0.90,
 1, 1, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
};

int decision = 0;
int deep_now = 0;
const int large_num = 1000000;
int maxmin[15];
int count_cal = 0;
int abcut = 0;	// he so cat tia, nhan 0 neu tinh chinh xac
float cut_k = 0.2;	// he so cat tia, nhan 1 neu tinh chinh xac
int hit_count = 0;

int score_king[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 5, 5, 5, 0, 0, 0,
	};

int score_bishop[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,-3, 0, 0, 0, 6, 0, 0, 0,-3,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 2, 0, 0, 0, 2, 0, 0,
	};

int score_knight[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 7, 8, 7, 6, 7, 8, 7, 4,
		0, 5, 9,15, 7, 7, 7,15, 9, 5,
		0, 7,10,12,14,12,14,12,10, 7,
		0, 4, 8, 6, 7, 5, 7, 6, 8, 4,
		0, 2, 8, 8, 8, 8, 8, 8, 8, 2,
		0, 2, 8, 9, 8, 8, 8, 9, 8, 2,
		0, 0, 7, 7, 6, 8, 6, 7, 7, 0,
		0, 0, 4, 3, 4, 4, 4, 3, 4, 0,
		0,-2, 2, 2, 2,-7, 2, 2, 2,-2,
		0,-3,-3, 0, 0, 0, 0, 0,-3,-3,
	};

int score_cannon[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,12, 9, 3, 0, 0, 0, 3, 9,12,
		0, 3, 2, 2, 3, 1, 3, 2, 2, 3,
		0, 3, 2, 1, 1, 2, 1, 1, 2, 3,
		0, 2, 2, 1, 1,12, 1, 1, 2, 2,
		0, 1, 1, 1, 1,11, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 7, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 4, 1, 1, 1, 1,
		0, 5, 5, 5, 6, 7, 6, 5, 5, 5,
		0, 0, 1, 1, 0, 1, 0, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

int score_rook[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,11,15, 6,15, 6,15, 6,15,11,
		0,10,15,12,16,10,16,12,15,10,
		0,10,11,11,12,10,12,11,11,10,
		0, 2,10, 2,10, 4,10, 2,10, 2,
		0, 2,10, 2,10, 2,10, 2,10, 2,
		0, 2,12, 2,15, 2,15, 2,12, 2,
		0, 2, 6, 2, 6, 2, 6, 2, 6, 2,
		0, 3, 7, 3, 7, 3, 7, 3, 7, 3,
		0, 4, 8, 4, 2, 2, 2, 4, 8, 4,
		0,-5, 6, 2, 8, 2, 8, 2, 6,-5,
	};

int score_pawn[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 3, 6,11, 6,11, 6, 3, 1,
		0,18,22,27,30,30,30,27,22,18,
		0,18,22,27,32,32,32,27,22,18,
		0,17,20,22,27,27,27,22,20,17,
		0,16,17,21,25,25,25,21,17,16,
		0, 1, 0, 7, 0,12, 0, 7, 0, 1,
		0, 0, 0,-2, 0,12, 0,-2, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};


// Thu tu anh xa type cua quan co:
/*
        Do      Den
Vua     1       11
Si      2       12
Tuong   3       13
Xe      4       14
Ma      5       15
Phao    6       16
Tot     7       17
*/

class packed_board {	// Class nay de luu trang thai ban co da duoc nen (1 trang thai ban co duoc bieu dien boi 6 so nguyen)
private:
	int info[6];	/*	Chung ta se su dung tat ca la 6*32 = 192 bit, dung moi 96 bit de luu vi tri cua quan co cua moi ben
                        Thu tu luu tu bit cao den thap:
            Quan den    info[0]: xe 1, xe 2, ma 1, ma 2, vua (7+7+7+7+4)
                        info[1]: phao 1, phao 2, tot 1, tot 2, si 1, do hay den di truoc (1 cho do) (7+7+7+7+3+1)
                        info[2]: tot 3, tot 4, tot 5, si 2, tuong 1, tuong 2, bo trong 2 bit (7+7+7+3+3+3+2)
            Quan do     info[0]: xe 1, xe 2, ma 1, ma 2, vua (7+7+7+7+4)
                        info[1]: phao 1, phao 2, tot 1, tot 2, si 1, bo trong 1 bit (7+7+7+7+3+1)
                        info[2]: tot 3, tot 4, tot 5, si 2, tuong 1, tuong 2, bo trong 2 bit (7+7+7+3+3+3+2)
			*/
private:
	inline int shift_right(int val, int dgt) {	//dich phai dgt bit
		if (dgt >= 0)
			return val >> dgt;
		return val << (-dgt);
	};
	inline void put(int val, int len, int arr_idx, int dgt_idx) {	//dua len bit cua val vao info[arr_idx], bat dau tu vi tri dgt_idx
		int tmp = ((int)0x80000000 >> (31 - dgt_idx)) ^ (shift_right((int)0x80000000, 31 - dgt_idx - len));
		info[arr_idx] |= tmp;
		info[arr_idx] &= ((~tmp) | (val << dgt_idx));
		return;
	};
	inline int get(int len, int arr_idx, int dgt_idx) {	// Tra ve len bit trong info[arr_idx] bat dau tu dgt_idx
		int tmp = ((int)0x80000000 >> (31 - dgt_idx)) ^ (shift_right((int)0x80000000, 31 - dgt_idx - len));
		tmp &= info[arr_idx];
		return ((unsigned)tmp) >> dgt_idx;
	};
	inline int get_value(int type, int i, int j) {	// Toa do chinh xac ->Gia tri ma hoa
		switch (type) {
		case 1: return 3 * i + j - 27;	// Vua do
		case 2: return (3 * i + j - 26) / 2;	// Si do
		case 3: return (4 * i + j) / 3 - 8;	// Tuong do
		case 11: return 3 * i + j - 6;	//  Vua den
		case 12: return (3 * i + j - 5) / 2;	// Si den
		case 13: return (4 * i + j - 1) / 3 - 1;	// Tuong den
		default: return 10 * i + j;	// Cac quan con lai
		}
	};
	inline int get_len(int type) {	//
		int len[18] = {0, 4, 3, 3, 7, 7, 7, 7, 0, 0, 0, 4, 3, 3, 7, 7, 7, 7};
		return len[type];
	};
	inline int get_arridx(int type, int id) {	// Tra ve chi so trong mang info, ung voi type va id
		int arridx[18] = { 0, 3, 4, 5, 3, 3, 4, 4, 0, 0, 0, 0, 1, 2, 0, 0, 1, 1 };
		int tmp = arridx[type];
		if (((type == 2) || (type == 12)) && (id == 2))	//Si 2
			++tmp;
		else if (((type == 7) || (type == 17)) && (id >= 3))	//Tot 3, 4, 5
			++tmp;
		return tmp;
	};
	inline int get_dgtidx(int type, int id) {	// Tra ve gia tri dgtidx (vi tri bit trong info) cua quan co ung voi type va id
		int dgtidx[17] = {0, 0, 1, 8, 5, 2, 25, 18, 11, 4, 25, 18, 11, 4, 25, 18, 11};
		return dgtidx[2 * (type % 10) + id - 3];
	};
public:
	void print() {
		for (int i = 0; i < 6; ++i)
			printf("%x ", info[i]);
		cout << endl;
		return;
	};
	void pack(const int board[][10], const int red_black) {	// Nen ban co
		for (int i = 0; i < 6; ++i)
			info[i] = 0;
		int cnt[18] = {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 };
		for (int i = 1; i <= 10; ++i) {
			for (int j = 1; j <= 9; ++j) {
				int tmp = board[i][j];
				if (tmp) {
					put(get_value(tmp, i, j), get_len(tmp), get_arridx(tmp, cnt[tmp]), get_dgtidx(tmp, cnt[tmp]));
					++cnt[tmp];
				}
			}
		}
		// Do di truonc thi red_black = 1
		put(red_black, 1, 1, 0);
		return;
	};

	friend const bool operator < (const packed_board& b1, const packed_board& b2) {	// Ham so sanh can de thuc hien truy van tren cay tim kiem
		for (int i = 0; i < 6; ++i) {
			if (b1.info[i] < b2.info[i])
				return 1;
			if (b1.info[i] > b2.info[i])
				return 0;
		}
		return 0;
	};
};

struct child_node {
public:
	short value;
	child_node* next;
};

class board_info {
private:
	int info[3];
public:
	board_info(){
		for(int i = 0; i < 3; ++i)
			info[i] = 0;
	};
	int read_depth(int mode) {
		int tmp = ((int)0x80000000) >> 7;
		return ((unsigned)(tmp & info[mode])) >> 24;
	};
	void write_depth(int mode, int val) {
		int tmp = ((int)0x80000000) >> 7;
		info[mode] &= (~tmp);
		info[mode] |= (val << 24);
		return;
	};
	int read_score(int mode) {
		int tmp = ((int)0x80000000) >> 7;
		int res = info[mode] & (~tmp);
		if (res & 0x800000)
			res |= 0xff000000;
		return res;
	};
	void write_score(int mode, int val) {
		int tmp = ((int)0x80000000) >> 7;
		info[mode] &= tmp;
		info[mode] |= val;
		return;
	};
	void make_blank(int mode) {
		info[mode] = 0;
		return;
	};
	bool is_blank(int mode) {
		if (info[mode] == 0)
			return 1;
		return 0;
	};
};

class record_table {
private:
	map<packed_board, board_info> table;
	queue<packed_board> table_seq;
public:
	unsigned max_volume;
private:
	void traversemap() {
		map<packed_board, board_info>::iterator iter;
		cout << "traverse map: " << endl;
		for (iter = table.begin(); iter != table.end(); iter++) {
			packed_board tmp = iter->first;
			tmp.print();
		}
		cout << endl;
		return;
	};
	int power(int expo) {
		int res = 1;
		while (expo--)
			res *= 3;
		return res;
	};
public:
	record_table() {
		max_volume = cut_k * 250 * power(deep_limit);
		table.clear();
		while (!table_seq.empty())
			table_seq.pop();
		return;
	};
	board_info find(const int board[][10], const int red_black) {
		packed_board obj;
		obj.pack(board, red_black);
		map<packed_board, board_info>::iterator it = table.find(obj);
		if (it != table.end()) {
			board_info tmp = it->second;
			return tmp;
		}

		board_info blank;
		return blank;
	};
	void push(const int board[][10], const int red_black, const int depth, const int score, int mode ) {

		if (table.size() >= max_volume) {
			map<packed_board, board_info>::iterator it;
			do {
				packed_board pb = table_seq.front();
				table_seq.pop();
				it = table.find(pb);
			} while (it == table.end());
			table.erase(it);
		}

		board_info info;
		info.write_depth(mode, depth);
		info.write_score(mode, score);
		packed_board pboard;
		pboard.pack(board, red_black);
		map<packed_board, board_info>::iterator it = table.find(pboard);
		if (it == table.end()) {
			table_seq.push(pboard);
			table[pboard] = info;
		}
		else {
			if (it->second.read_depth(mode) < depth) {
				it->second.write_depth(mode, depth);
				it->second.write_score(mode, score);
			}
		}
		return;
	};
	int get_size() {
		return table.size();
	};
};

record_table rectab;
//                              Vua     Si  Tuong  Xe   Ma  Phao  Tot
const int power_score[9] = { 0, 100000, 22, 20,    165, 69, 69,   8,    0 };	// Danh sach diem so cua cac quan co

int single_score(int color) {
	++count_cal;	//for trial
	int score = 0;

	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j) {
			if (get_color(field[i][j]) == color) {
				int type = field[i][j] % 10;
				int ii = i;
				if (color == 0)
					ii = 11 - i;
				score += power_score[type];
				switch (type) {
				case 1: score += score_king[ii][j]; break;
				case 2: break;
				case 3: score += score_bishop[ii][j]; break;
				case 4: score += score_rook[ii][j]; break;
				case 5: score += score_knight[ii][j]; break;
				case 6: score += score_cannon[ii][j]; break;
				case 7: int plus = (turns >> 3); if (plus > 16) plus = 16; score += plus; score += score_pawn[ii][j]; break;
				}
			}
		}
	}
	return score;

}

int score(int color) {
		int red_score = single_score(1) - single_score(0);
		if (color)
			return red_score;
		else
			return -red_score;
}

// Tra ve mot nuoc di ngau nhien
int random(int color) {
	get_valid_moves(color);
	int size = valid_moves.size();
	srand((unsigned)time(NULL));
	int num = rand() % (size - 1);
	move(valid_moves[num]);
	return valid_moves[num];
}

int greedy(int color) {
	get_valid_moves(color);
	int size = valid_moves.size();
	int num = 0;
	int save[11][10];
	int max_score = -1000000;
	int max_num = size - 1;
	for (num = size - 1; num >= 0; --num) {
		for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 9; ++j)
				save[i][j] = field[i][j];
		}
		move(valid_moves[num]);
		int tmp = score(color);
		if (tmp > max_score) {
			max_score = tmp;
			max_num = num;
		}
		for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
	}
	move(valid_moves[max_num]);
	return valid_moves[max_num];
}

struct move_info {
public:
	int director;
	int score;
	friend bool operator < (const move_info& x, const move_info& y) {
		return x.score < y.score;
	};
	move_info(int dir, int sco) : director(dir), score(sco) {};
};

int prejudge(int color, int deep, bool from_max);

int search_min(int color)
{
	int search_max(int color);
	int king = 0;
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j) {
			if (field[i][j] % 10 == 1)
				++king;
		}
	}
	if (king < 2 || deep_now >= deep_limit)
		return score(color);
	int min = large_num;

	if (cache) {
		board_info info = rectab.find(field, 1);
		if (color) {
			if ((!info.is_blank(2)) && (info.read_depth(2) == 10 + deep_limit - deep_now)) {
				min = info.read_score(2);
				++hit_count;
			}
		}
		else {
			if ((!info.is_blank(1)) && (info.read_depth(1) == 10 + deep_limit - deep_now)) {
				min = -info.read_score(1);
				++hit_count;
			}
		}
	}
	// Luu trang thai khoi tao
	int save[11][10];
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j)
			save[i][j] = field[i][j];
	}
	// Tinh toan tu truoc, nham toi uu hoa thu tu tim kiem, cai thien su chinh xac cua cat tia
	vector<move_info> list;
	get_valid_moves(1 - color);
	int size = valid_moves.size();
	for (int p = 0; p < size; ++p)
		list.push_back(move_info(valid_moves[p], 0));
	valid_moves.clear();
	for (int p = 0; p < size; ++p) {
		int eat = move(list[p].director);
		list[p].score = prejudge(color, deep_limit - deep_now, 0) - eat_k * power_score[eat % 10];
		for (int i = 1; i <= 10; ++i) { // Quay lui
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
	}
	sort(list.begin(), list.end());
	// Tim vi tri tot de cat tia
	float cut = cuttab[cuttab_index][deep_limit - deep_now];
	for (int p = 0; p < cut * (float)size; ++p)
	{
		move(list[p].director);
		maxmin[deep_now] = min; // Luu du lieu cua dinh
		++deep_now;
		int temp = search_max(color); // Goi de quy max
		--deep_now; //Quay lui
		if (temp < min)
			min = temp;
		for (int i = 1; i <= 10; ++i) { // Quay lui
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
		for (int step = deep_now - 1; step >= 0; step = step - 2) {
			if (min <= maxmin[step] + abcut) { // cat tia alpha beta
				if (cache) {
					if (color)
						rectab.push(field, 1, 10 + deep_limit - deep_now, min, 2);
					else
						rectab.push(field, 1, 10 + deep_limit - deep_now, -min, 1);
				}
				return min;
			}
		}
	}
	if (cache) {
		if (color) {
			rectab.push(field, 1, 10 + deep_limit - deep_now, min, 2);
			rectab.push(field, 1, 10 + deep_limit - deep_now, min, 1);
		}
		else {
			rectab.push(field, 1, 10 + deep_limit - deep_now, -min, 1);
			rectab.push(field, 1, 10 + deep_limit - deep_now, -min, 2);
		}
	}
	return min;
}

int search_max(int color)
{
	int king = 0;
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j) {
			if (field[i][j] % 10 == 1)
				++king;
		}
	}
	if (king < 2 || deep_now >= deep_limit)
		return score(color);

	int max = -large_num;

	if (cache) {
		board_info info = rectab.find(field, 1);
		if (color) {
			if ((!info.is_blank(1)) && (info.read_depth(1) == 10 + deep_limit - deep_now)) {
				max = info.read_score(1);
				++hit_count;
			}
		}
		else {
			if ((!info.is_blank(2)) && (info.read_depth(2) == 10 + deep_limit - deep_now)) {
				max = -info.read_score(2);
				++hit_count;
			}
		}
	}

	int save[11][10];
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j)
			save[i][j] = field[i][j];
	}

	vector<move_info> list;
	get_valid_moves(color);
	int size = valid_moves.size();
	for (int p = 0; p < size; ++p)
		list.push_back(move_info(valid_moves[p], 0));
	valid_moves.clear();
	for (int p = 0; p < size; ++p) {
		int eat = move(list[p].director);
		list[p].score = prejudge(color, deep_limit - deep_now, 1) + eat_k * power_score[eat % 10];
		for (int i = 1; i <= 10; ++i) {
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
	}
	sort(list.begin(), list.end());

	float cut = cuttab[cuttab_index][deep_limit - deep_now];
	int best_move = 0;
	for (int p = size - 1; p >= ((float)1 - cut) * ((float)size - (float)1); --p)
	{
		move(list[p].director);
		maxmin[deep_now] = max;
		++deep_now;
		int temp = search_min(color);
		--deep_now;
		if (temp > max) {
			max = temp;
			best_move = p;
		}
		for (int i = 1; i <= 10; ++i) {
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
		for (int step = deep_now - 1; step >= 0; step = step - 2) {
			if (max >= maxmin[step] - abcut) {

				if (cache) {
					if (color)
						rectab.push(field, 1, 10 + deep_limit - deep_now, max, 1);
					else
						rectab.push(field, 1, 10 + deep_limit - deep_now, -max, 2);
				}
				decision = list[best_move].director;
				return max;
			}
		}
	}

	if (cache) {
		if (color) {
			rectab.push(field, 1, 10 + deep_limit - deep_now, max, 1);
			rectab.push(field, 1, 10 + deep_limit - deep_now, max, 2);
		}
		else {
			rectab.push(field, 1, 10 + deep_limit - deep_now, -max, 2);
			rectab.push(field, 1, 10 + deep_limit - deep_now, -max, 1);
		}
	}
	decision = list[best_move].director;
	return max;
}

int AI_alphabeta(int color) {
	//save and reset
	int save_deep_limit = deep_limit;
	deep_limit = 6;
	int save_cuttab_index = cuttab_index;
	cuttab_index = cuttabR - 1;
	hit_count = 0;
	count_cal = 0;
	deep_now = 0;
	eval = search_max(color);
	deep_limit = save_deep_limit;
	cuttab_index = save_cuttab_index;
	return decision;
}

int AI(int color) {
	return AI_alphabeta(color);
}


int prejudge(int color, int deep, bool from_max) {
	if (deep == 1)
		return 0;
	if (deep <= 4)
		return score(color);

	int save_field[11][10];
	for (int i = 0; i < 11; ++i) {
		for (int j = 0; j < 10; ++j)
			save_field[i][j] = field[i][j];
	}
	int save_deep_limit = deep_limit;
	int save_turns = turns;
	int save_deep_now = deep_now;
	int save_maxmin[15];
	for (int i = 0; i < 15; ++i)
		save_maxmin[i] = maxmin[i];
	int save_abcut = abcut;

	int res = 0;

	deep_limit = deep - 3;
	deep_now = 0;
	for (int i = 0; i < 15; ++i)
		maxmin[i] = 0;

	if (from_max)
		res = search_min(color);
	else
		res = search_max(color);

	for (int i = 0; i < 11; ++i) {
		for (int j = 0; j < 10; ++j)
			field[i][j] = save_field[i][j];
	}
	deep_limit = save_deep_limit;
	turns = save_turns;
	deep_now = save_deep_now;
	for (int i = 0; i < 15; ++i)
		maxmin[i] = save_maxmin[i];
	abcut = save_abcut;
	return res;
}


