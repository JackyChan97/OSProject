#ifndef UTIL_H
#define UTIL_H


#include<iostream>
#include<iomanip>
#include<string>
#include<conio.h>
#include<cstring>
#include<cstdio>

#include "define.h"

using namespace std;

// 根据路径名获取finode节点号
int getnode(char *path)
{
	//cout<<"*path:"<<path<<endl;
	if(!strcmp(path,"/"))
		return 0;
	if (path[0] != '/')
	{
		return -1;	//路径非法
	}
	else
	{
		struct dire cdir = root->dir[1];
		int ino = 0;
		char tpath[NAMESIZE*DIRNUM] = "";
		strcpy(tpath, path);
		char *fpath = strtok(tpath, "/");
		//cout << fpath << endl;
		int match = 0;
		while (fpath != NULL)
		{
			match = 0;
			//cout << fpath<<endl;
			for (int i = 0; i < DIRNUM; i++)
			{
				if (!strncmp(fpath, cdir.direct[i].d_name, strlen(fpath)))
				{

					//cout << cdir.direct[i].d_name << endl;
					if (root->fnode[cdir.direct[i].d_ino].fi_mode == DIRMODE)
					{
						
						ino = cdir.direct[i].d_ino;
						cdir = root->dir[root->fnode[ino].dir_no];

						match = 1;
						break;
					}
					else
					{
						return -1;	//是文件，而非文件名
					}
				}

			}
			fpath = strtok(NULL, "/");
		}
		if(!match)
			return -1;
		return ino;	//为根节点“/”
	}
	return -1;	//错误
}

char* get_file_name( int ino, int i){
	return root->dir[root->fnode[ino].dir_no].direct[i].d_name;
}

int get_file_mode(int ino, int i){
	return root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_mode;
}

int get_file_size(int ino, int i){
	return root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size;
}

int get_file_direct_ino(int ino, int id){
	return root->dir[root->fnode[ino].dir_no].direct[id].d_ino;
}

bool check_file_exist(int ino, char* fname){
	for (int i = 0; i < DIRSIZE; i++) {
		if (strcmp(get_file_name(ino, i), fname) == 0)	{
			return 1;
		}
	}
	return 0;
}

void init_new_fnode(int i, int mode,int dir_no){
	if(mode==DIRMODE)
		root->fnode[i].dir_no = dir_no;
	root->fnode[i].fi_mode = mode;
	root->fnode[i].fi_size = 0;
	root->fnode[i].fi_addr[0] = 0;
	root->fnode[i].fi_nlink = 1;
	root->fnode[i].double_addr = -1;
	root->fnode[i].createdTime = time(NULL);
	//cout<<root->fnode[i].createdTime<<endl;
}

int add_new_fnode(int mode,int dir_no){
	for (int i = 0; i < NUM; i++){
		if (root->root.s_freeinode[i]==0) {			
			root->root.s_freeinode[i] = 1 ;
			root->root.s_freeinodesize--;
			init_new_fnode(i, mode,dir_no);
			return i;
		}	
	}
	cout << "no more fnode, please delete something" << endl;
	return -1;
}

int copy_inode(finode old_i) {
	for (int i = 0; i < NUM; i++) {
		if (root->root.s_freeinode[i]==0) {
			root->fnode[i].fi_mode = old_i.fi_mode;
			root->fnode[i].fi_size = old_i.fi_size;
			root->fnode[i].fi_addr[0] = 0;
			root->fnode[i].fi_nlink = 1;
			root->fnode[i].double_addr = -1;
			root->fnode[i].createdTime = time(NULL);
			return i;
		}
	}
}

void init_file_to_direct(int ino, int i, int n_ino, char* fname){
	root->dir[root->fnode[ino].dir_no].direct[i].d_ino = n_ino ;
	strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, fname);
}

int add_file_to_direct( int ino, int n_ino, char* fname){
	for (int i = 0; i < DIRSIZE; i++) {
		if (strlen(get_file_name(ino, i)) == 0)	{
			init_file_to_direct(ino, i, n_ino, fname);
			root->dir[root->fnode[ino].dir_no].size++;
			return i;
		}
	}
}

void update_file_size(int ino, int fsize){
	root->fnode[ino].fi_size = fsize;
}

int find_free_block(){
	for( int i = 0 ; i < BNUM; ++i){
		if( root->root.s_freeblock[i] != 1 )
			return i;
	}
	cout << "the free block is not enogh" << endl;
	return -1;
}

int get_double_addr_block_id( int ino ){
	return root->fnode[ino].double_addr;
}

void update_addr_in_double_addr_block(int id, int pos, int addr){ 
	// id is block_addr, pos in addr_block
	// 3 char as a 24-bits address
	addr += 1 ;
	root->free[id*BSIZE+pos*3+2] = addr%2^8;
	addr = addr/2^8;
	root->free[id*BSIZE+pos*3+1] = addr%2^8;
	addr = addr/2^8;
	root->free[id*BSIZE+pos*3] = addr%2^8;
}

int get_double_addr_block_addr( int id, int j){
	int addr = int(root->free[id*BSIZE+j*3]);
	addr = addr*2^8 + int(root->free[id*BSIZE+j*3+1]);
	addr = addr*2^8 + int(root->free[id*BSIZE+j*3+2]);
	return addr-1;
}

int get_file_direct_id_in_fnode( int ino, char *fname){
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strcmp( get_file_name(ino, i), fname) == 0)
		{
			return i;
		}
	}
	cout << "not exist " << fname << endl;
	return -1;
}

finode get_direct_fnode( int ino, int direct_i){
	return root->fnode[root->dir[root->fnode[ino].dir_no].direct[direct_i].d_ino];
}

void update_direct_name(int ino, int id, char *fname){
	strcpy(root->dir[root->fnode[ino].dir_no].direct[id].d_name, fname);
}

// 如果不存在返回-1 否则返回inodenumber
 int check_path_exist(char *topath){
	 if(topath[0]=='/'){
		 int i = getnode(topath);
		 return i;	
	 }
	 else{
		char cpath[NAMESIZE*DIRNUM] = "";
		strcpy(cpath,PATH);
		if(strcmp(PATH,"/")){
			strcat(cpath,"/");
			strcat(cpath,topath);
		}
		else
			strcat(cpath,topath);
		//cout<<"cp:"<<cpath<<endl;
		int i =getnode(cpath);
		//cout<<i<<endl;
		return i;
	 }
 }

#endif // !UTIL_H


