// Chuong trinh nay tao va thao tac tren bang du lieu de tra cuu cho cac
// Cau truc du lieu duoc su dung se la Trie

#include<iostream>
#include<fstream>
#include<stdlib.h>
#include"head.h"
using namespace std;
extern int field[11][10];
extern int record[300];
extern int turns;

const int mem_size = 65500;

class node {
public:
	int director;	//0 = rong
	int left_child_index;	//-1 = rong
	int right_sibling_index;	//-1 = rong
public:
	int clear() {
		director = 0;
		left_child_index = -1;
		right_sibling_index = -1;
		return 0;
	}
	node(int d = 0) {
		director = d;
		left_child_index = -1;
		right_sibling_index = -1;
	};
};

class start_table {	//
private:
	node memory[mem_size];
	int mem_index;	// chi so cua node cuoi cung ma du lieu duoc luu
	int ai_index;
private:
	int traverse(int index) {
		//left
		if (memory[index].left_child_index != -1)
			traverse(memory[index].left_child_index);
		//visit
		cout << "index = " << index << endl;
		cout << "left_child_index = " << memory[index].left_child_index << endl;
		cout << "director = " << memory[index].director << endl;
		cout << "right_sibling_index = " << memory[index].right_sibling_index << endl;
		system("pause");
		//right
		if (memory[index].right_sibling_index != -1)
			traverse(memory[index].right_sibling_index);
		return 0;
	};
public:
	int get_mem_idx() {
		return this->mem_index;
	};
	int clear_memory() {
		for (int i = 0; i < mem_size; ++i)
			memory[i].clear();
		mem_index = 0;
		ai_index = 0;
		return 0;
	};
	start_table() {
		clear_memory();
	};
	int load_from_file() {
		char filename[30] = "start_table.dat";
		ifstream fin(filename, ios::binary);
		fin.read((char*)&mem_index, sizeof(int));
		for (int i = 0; i < mem_size; ++i)
			fin.read((char*)&memory[i], sizeof(node));
		fin.close();
		return 0;
	};
	int save_to_file() {
		char filename[30] = "start_table.dat";
		ofstream fout(filename, ios::binary);
		fout.write((char*)&mem_index, sizeof(int));
		for (int i = 0; i < mem_size; ++i)
			fout.write((char*)&memory[i], sizeof(node));
		fout.close();
		return 0;
	};
	int add_record_to_memory() {
		// Luu ban ghi tran dau vao bo nho
		int index = 0;
		int father_index = -1;
		bool flag = 0;
		if (memory[0].director == 0)
			flag = 1;
		for (int i = 1; i < 300; ++i) {
			if (record[i] == 0)
				break;
			while (memory[index].director != 0){
				if (memory[index].director == record[i]) {	// Tim thay
					break;
				}
				if (memory[index].right_sibling_index == -1) {	// node dang xet la node nam ngoai cung ben phai, thuc hien them node
					node tmp(record[i]);
					memory[mem_index] = tmp;
					memory[index].right_sibling_index = mem_index;
					index = mem_index;
					++mem_index;
					break;
				}
				// Tiep tuc tim kiem
				index = memory[index].right_sibling_index;
			}
			memory[index].director = record[i];
			father_index = index;
			if (flag) {
				++mem_index;
				flag = 0;
			}
			if (memory[index].left_child_index == -1) {
				node tmp;
				memory[mem_index] = tmp;
				memory[index].left_child_index = mem_index;
				index = mem_index;
				++mem_index;
			}
			else {
				index = memory[index].left_child_index;
			}
		}
		// Xoa node duoc tao cuoi cung
		if ((memory[index].director == 0) && (father_index != -1)) {
			memory[father_index].left_child_index = -1;
			memory[index].clear();
			--mem_index;
		}
		return 0;
	};
	int ai_search(int director) {	// Tra ve 0 = miss
		if (director == 0) {
			if (mem_index == 0)
				return 0;
			int res = memory[ai_index].director;
			ai_index = memory[ai_index].left_child_index;
			return res;
		}
		if (ai_index == -1)	//miss
			return 0;
		//Tim kiem director
		while (ai_index != -1) {
			if (memory[ai_index].director == director)	//find
				break;
			if (memory[ai_index].right_sibling_index == -1)	//miss
				return 0;
			//next
			ai_index = memory[ai_index].right_sibling_index;
		}
		//find
		if (memory[ai_index].left_child_index == -1)	//miss
			return 0;
		//hit
		ai_index = memory[ai_index].left_child_index;
		int res = memory[ai_index].director;
		ai_index = memory[ai_index].left_child_index;
		return res;
	};
	int memory_checker() {
		for (int i = 1; i < 10; ++i)
			cout << record[i] << endl;
		traverse(0);
		cout << mem_index << endl;
		cout << "checked" << endl;
		system("pause");
		return 0;
	};
};

start_table stttab;

int save_to_start_table() {	//Them tran dau vua danh vao start table
	stttab.load_from_file();
	//stttab.memory_checker();
	stttab.add_record_to_memory();
	stttab.save_to_file();
	//stttab.memory_checker();
	return 0;
}

int ai_start_table_init() {	// Goi ham nay truoc khi bat dau tran dau, de nap database
	stttab.load_from_file();
	if (stttab.get_mem_idx() > 60000) {
		cout << "start_table_mem_index = " << stttab.get_mem_idx() << endl;
		cout << "Press any key to continue..." << endl;
		system("pause");
	}
	return 0;
}

int ai_start_table_search(int director) {	// Tra ve 0 = Khong tim thay
	return stttab.ai_search(director);
}
