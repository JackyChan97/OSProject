#include<iostream>
#include<iomanip>
#include<string>
#include<conio.h>
#include<cstring>
#include<cstdio>

#include"define.h"

using namespace std;

//ȫ�ִ��̱���
struct storage *root = new storage;
// ȫ��·��
char PATH[NAMESIZE*DIRNUM] = "";

char content[BSIZE*BNUM] = "";
// ����·������ȡfinode�ڵ��

char* get_file_name( int ino, int i){
	return root->dir[root->fnode[ino].dir_no].direct[i].d_name;
}

int get_file_mode(int ino, int i){
	return root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_mode;
}

int get_file_size(int ino, int i){
	return root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size;
}



