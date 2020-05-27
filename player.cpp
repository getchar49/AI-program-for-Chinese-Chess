#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<algorithm>
#include"head.h"
using namespace std;

extern int field[11][10];
extern vector<int> valid_moves;
extern int record[300];
extern int chase_record[300];
extern int turns;
extern int change_view;

extern int deep_limit;
extern int cuttab_index;

int initiate_field(){
	/* Ma cac quan co
	Vua do 1,         Vua den 11
    Si do 2,          Si den 12
	Tuong do 3,       Tuong den 13
	Xe do 4,          Xe den 14
	Ma do 5,          Ma den 15
	Phao do 6,        Phao den 16
	Tot do 7,         Tot den 17 */
	int tmp[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,14,15,13,12,11,12,13,15,14,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0,16, 0, 0, 0, 0, 0,16, 0,
		0,17, 0,17, 0,17, 0,17, 0,17,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 7, 0, 7, 0, 7, 0, 7, 0, 7,
		0, 0, 6, 0, 0, 0, 0, 0, 6, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 5, 3, 2, 1, 2, 3, 5, 4,
	};
	for(int i = 0; i <= 10; ++i){
		for(int j = 0; j <= 9; ++j)
			field[i][j] = tmp[i][j];
	}
	turns = 0;
	return 0;
}

bool undo(){
	int temp = turns;
	initiate_field();
	while(turns < temp - 3){
		++turns;
		move(record[turns]);
	}
	turns++;
	return 0;
}

int players_turn(int color){
    cout << "Nhap toa do nuoc di theo cu phap nhu sau:[Row1][Col1][Row2][Col2]\n";
    cout << "Row mang gia tri tu 0 den 9, Col tu 1 den 9, viet lien ke nhau\n";
    cout << "Co the nhap quit de thoat, nhap undo de quay lai nuoc truoc\n> ";
	string inpu;
	cin >> inpu;
	if ((inpu.length() == 4) && (inpu[0] >= '0') && (inpu[0] <= '9') && (inpu[1] >= '1') && (inpu[1] <= '9') && (inpu[2] >= '0') && (inpu[2] <= '9') && (inpu[3] >= '1') && (inpu[3] <= '9')) {
		int cmd = 1000 * (inpu[0] - '0') + 100 * (inpu[1] - '0') + 10 * (inpu[2] - '0') + (inpu[3] - '0');
		int director = player_move(cmd);
		get_valid_moves(color, 0);
		vector<int>::iterator it = find(valid_moves.begin(), valid_moves.end(), director);
		if (it != valid_moves.end()) {
			return director;
		}
		else {
			cout << "Nuoc di khong hop le! Vui long nhap lai" << endl;
			return players_turn(color);
		}
	}
	else if ((inpu.length() == 4) && (inpu[0] == 'u') && (inpu[1] == 'n') && (inpu[2] == 'd') && (inpu[3] == 'o')) {
		undo();
		system("cls");
		print_field(field);
		return players_turn(color);
	}
	else if ((inpu.length() == 4) && (inpu[0] == 'q') && (inpu[1] == 'u') && (inpu[2] == 'i') && (inpu[3] == 't'))
		exit(0);
	else{
		cout << "Input khong hop le, vui long nhap lai" << endl;
		return players_turn(color);
	}
}
