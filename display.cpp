#include<iostream>
#include<stdio.h>
#include<stdlib.h>
//#include<windows.h>
using namespace std;

extern int field[11][10];
extern int turns;
extern int view;
extern int change_view;

void color_cout(const int color1){
}

int print_field(const int field[][10]){
	system("cls");
	cout << "Game co tuong. Luot: " << turns << endl << endl;
	//color_cout(7);
	int print_field[20][18] = {
		18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
		18,23,21,28,21,28,21,28,21,28,21,28,21,28,21,28,21,24,
		18,22,18,22,18,22,18,22,19,22,20,22,18,22,18,22,18,22,
		18,27,21,31,21,31,21,31,21,31,21,31,21,31,21,31,21,29,
		18,22,18,22,18,22,18,22,20,22,19,22,18,22,18,22,18,22,
		18,27,21,36,21,31,21,31,21,31,21,31,21,31,21,36,21,29,
		18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,
		18,32,21,31,21,36,21,31,21,36,21,31,21,36,21,31,21,34,
		18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,
		18,27,21,30,21,30,21,30,21,30,21,30,21,30,21,30,21,29,
		18,22,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,22,
		18,27,21,28,21,28,21,28,21,28,21,28,21,28,21,28,21,29,
		18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,
		18,32,21,31,21,36,21,31,21,36,21,31,21,36,21,31,21,34,
		18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,18,22,
		18,27,21,36,21,31,21,31,21,31,21,31,21,31,21,36,21,29,
		18,22,18,22,18,22,18,22,19,22,20,22,18,22,18,22,18,22,
		18,27,21,31,21,31,21,31,21,31,21,31,21,31,21,31,21,29,
		18,22,18,22,18,22,18,22,20,22,19,22,18,22,18,22,18,22,
		18,25,21,30,21,30,21,30,21,30,21,30,21,30,21,30,21,26
	};
	for(int i = 1; i <= 10; ++i){
		for(int j = 1; j <= 9; ++j){
			if(field[i][j])
				print_field[2 * i - 1][2 * j - 1] = field[i][j];
		}
	}
	int k = 0;
    cout << " ������������������������������������" << endl;
	for(int i = 0; i < 20; ++i){
		if(i % 2){
			++k;
			if(view)
				cout << ((11 - k) % 10);
			else
				cout << (k % 10);
		}
		else
			cout << " ";
		for(int j = 0; j < 18; ++j){
			switch(print_field[i][j]){
				case 1: color_cout(12); printf("\033[31m˧\033[0m"); color_cout(7); break;
				case 2: color_cout(12); printf("\033[31m��\033[0m"); color_cout(7); break;
				case 3: color_cout(12); printf("\033[31m��\033[0m"); color_cout(7); break;
				case 4: color_cout(12); printf("\033[31m��\033[0m"); color_cout(7); break;
				case 5: color_cout(12); printf("\033[31m��\033[0m"); color_cout(7); break;
				case 6: color_cout(12); printf("\033[31m��\033[0m"); color_cout(7); break;
				case 7: color_cout(12); printf("\033[31m��\033[0m"); color_cout(7); break;
				case 11: color_cout(10); printf("\033[32m��\033[0m"); color_cout(7); break;
				case 12: color_cout(10); printf("\033[32mʿ\033[0m"); color_cout(7); break;
				case 13: color_cout(10); printf("\033[32m��\033[0m"); color_cout(7); break;
				case 14: color_cout(10); printf("\033[32m��\033[0m"); color_cout(7); break;
				case 15: color_cout(10); printf("\033[32m��\033[0m"); color_cout(7); break;
				case 16: color_cout(10); printf("\033[32m��\033[0m"); color_cout(7); break;
				case 17: color_cout(10); printf("\033[32m��\033[0m"); color_cout(7); break;
				case 18: cout << "  "; break;
				case 19: cout << "\\ "; break;
				case 20: cout << "/ "; break;
				case 21: cout << "�� "; break;
				case 22: cout << "�� "; break;
				case 23: cout << "�� "; break;
				case 24: cout << "�� "; break;
				case 25: cout << "�� "; break;
				case 26: cout << "�� "; break;
				case 27: cout << "�� "; break;
				case 28: cout << "�� "; break;
				case 29: cout << "�� "; break;
				case 30: cout << "�� "; break;
				case 31: cout << "�� "; break;
				case 32: cout << "�� "; break;
				case 34: cout << "�� "; break;
				case 36: cout << "�� "; break;
			}
		}
		cout << endl;
	}
	cout << endl;
	return 0;
}
