#include<iostream>
#include<iomanip>
#include<string>
#include<conio.h>
#include<cstring>
#include<cstdio>

#include"define.h"

using namespace std;

//全局磁盘变量
struct storage *root = new storage;
// 全局路径
char PATH[NAMESIZE*DIRNUM] = "";

char content[BSIZE*BNUM] = "";
// 根据路径名获取finode节点号

char* get_file_name( int ino, int i){
	return root->dir[root->fnode[ino].dir_no].direct[i].d_name;
}

int get_file_mode(int ino, int i){
	return root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_mode;
}

int get_file_size(int ino, int i){
	return root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size;
}

void add_new_fnode(int i, int mode){
	root->fnode[i].fi_mode = mode;
	root->fnode[i].fi_size = 0;
	root->fnode[i].fi_addr[0] = 0;
	root->fnode[i].fi_nlink = 1;
	root->fnode[i].double_addr = -1;
}

void add_file_to_direct(int ino, int i, int n_ino, char* fname){
	root->dir[root->fnode[ino].dir_no].direct[i].d_ino = n_ino ;
	strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, fname);
}

void update_file_size(int ino, int fsize){
	root->fnode[ino].fi_size = fsize;
}

int find_free_block(){
	for( int i = 0 ; i < BNUM; ++i){
		if( root->root.s_freeblock[i] != 1 )
			return i;
	}
}




