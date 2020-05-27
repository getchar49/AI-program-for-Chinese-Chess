#include<iostream>
#include<fstream>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include"head.h"
#include <locale.h>
using namespace std;

int field[11][10]; //Luu trang thai hien tai cua game, bat dau tu (1,1)
int record[300]; // Luu du lieu tran dau
int chase_record[300];	// Dung de kiem tra xem co tinh huong bat quan co xay ra khong, dung de xac dinh long catch
int turns = 0;
extern int deep_limit;
int view = 0;
int change_view = 0;
vector<int> valid_moves;

int main(){
	initiate_field(); // Khoi tao trang thai ban co
	cout << "Nhap che do choi (1: Nguoi choi di truoc, 2: AI di truoc):\n> ";
	int mode = 0;
	for ( int i = 0 ; i < 300 ; ++ i) {
		record[i] = 0 ;
		chase_record[i] = 0 ;
	}
	cin >> mode; //  1: Doi thu cam quan do, 2: Doi thu cam quan den
	ai_start_table_init();
	bool start_table_hit_flag = 1;
	int director = 0;
	system("cls");
	print_field(field);
	while (turns < 298) {
        ++turns;
        if (mode==1) {
            director = players_turn(1);
        }
        else if (start_table_hit_flag) {
            director = ai_start_table_search(director);
            if (!director) {
                director = AI(1);
                start_table_hit_flag =  0;
            }
        }
        else director = AI(1);
        if (move_with_info(director)==20) chase_record[turns] = 1;
        record[turns] = director;
        print_field(field);
        ++turns;
        if (mode==2) {
            director = players_turn(0);
        }
        else if (start_table_hit_flag) {
            director = ai_start_table_search(director);
            if (!director) {
                director = AI(0);
                start_table_hit_flag = 0;
            }
        }
        else director = AI(0);
        if (move_with_info(director)==20) chase_record[turns] = 1;
        record[turns] = director;
        print_field(field);
	}
	system("pause");
	return 0;
}
