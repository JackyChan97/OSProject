#pragma   once  

#include<iostream>
#include<iomanip>
#include<string>
#include<conio.h>
#include<cstring>
#include<cstdio>

#include "define.h"

using namespace std;


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
	return -1;
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



