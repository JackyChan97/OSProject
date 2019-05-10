#pragma once
#define NUM 15360	//总节点数      15*1024
#define BNUM 15360	//磁盘块数  15M
#define BSIZE 1024 //块大小	           1024
//#define FSIZE 2048	//超级块大小	     
#define SSIZE 15728640 //存储空间大小
#define NAMESIZE 16	//文件名长度
#define DIRSIZE 16	//目录大小
#define DIRNUM 100	//目录个数
#define FREEBYTE 16746472 //空间总大小
#define DIRMODE 0 //目录类型
#define FILEMODE 1	//文件类型

#include<time.h>
// 返回状态
enum STATUS
{
	SUCCESS, ERR_PATH_FAIL, ERR_FILE_EXIST, ERR_FILE_NOT_EXIST, ERR_FILE_FAIL, ERR_FILE_SIZE, ERR_FNODE_NOT_ENOUGH
};
//超级块
struct superblock
{
	unsigned int s_size;	//总大小
	unsigned int s_itsize;	//inode表大小
	unsigned int s_freeinodesize;	//空闲i节点的数量
	unsigned int s_nextfreeinode;	//下一个空闲i节点
	bool s_freeinode[NUM];	//空闲i节点数组 0/1
	unsigned int s_freeblocksize;	//空闲块的数量          
	unsigned int s_nextfreeblock;	//下一个空闲块
	bool s_freeblock[BNUM];	//空闲块数组0/1  
};

// 文件节点
struct finode
{
	int fi_mode;	//类型：文件/目录
	int fi_nlink;		//链接数，当链接数为0，意味着被删除
	int dir_no;		//目录号   如果是目录的话
	long int fi_size;	//文件大小
	long int fi_addr[10];	//文件块一级指针，并未实现多级指针
	time_t createdTime;
	int double_addr;	//二级寻址
};
//目录项结构
struct direct
{
	char d_name[NAMESIZE];        //文件或者目录的名字
	unsigned short d_ino;        //文件或者目录的i节点号
};
//目录结构
struct dire
{
	struct direct direct[DIRSIZE];    //包含的目录项数组
	unsigned short size;        //包含的目录项大小
};

// 磁盘
struct storage
{
	struct superblock root;
	char blank0[1000];
	struct finode fnode[NUM];
	struct dire dir[DIRNUM];
	char blank[4792];
	char free[SSIZE];
};
extern struct storage *root;
extern char PATH[NAMESIZE*DIRNUM];
extern char content[BSIZE*BNUM] ;
//superblock [0,30]
// finode[31,930]
// dir[931,1023]
//free[1024,15*1024-1]
