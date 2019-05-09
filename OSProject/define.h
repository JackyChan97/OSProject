#define NUM 1024	//总节点数      1024
#define BNUM 15360	//磁盘块数  15M
#define BSIZE 1024 //块大小	           1024
#define FSIZE 2048	//超级块大小	     
#define SSIZE 16756736 //存储空间大小
#define NAMESIZE 16	//文件名长度
#define DIRSIZE 16	//目录大小
#define DIRNUM 32	//目录个数
#define FREEBYTE 16681920 //空间总大小
#define DIRMODE 0 //目录类型
#define FILEMODE 1	//文件类型

// 返回状态
enum STATUS
{
	SUCCESS, ERR_PATH_FAIL, ERR_FILE_EXIST, ERR_FILE_NOT_EXIST, ERR_FILE_FAIL, ERR_FILE_SIZE
};
//超级块
struct superblock
{
	unsigned int s_size;	//总大小
	unsigned int s_itsize;	//inode表大小
	unsigned int s_freeinodesize;	//空闲i节点的数量
	unsigned int s_nextfreeinode;	//下一个空闲i节点
	unsigned int s_freeinode[NUM];	//空闲i节点数组 0/1
	unsigned int s_freeblocksize;	//空闲块的数量          
	unsigned int s_nextfreeblock;	//下一个空闲块
	unsigned char s_freeblock[BNUM];	//空闲块数组0/1  
	char blank[1000];
};
//一次间接寻址
struct atime
{
	int fi_addr[256];
};
// 文件节点
struct finode
{
	int fi_mode;	//类型：文件/目录
	int fi_nlink;		//链接数，当链接数为0，意味着被删除
	int dir_no;		//目录号   如果是目录的话
	long int fi_size;	//文件大小
	long int fi_addr[10];	//文件块一级指针，并未实现多级指针
	struct atime  *fi_atime_unused;	//二级寻址
	char blank[4];

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
	struct finode fnode[NUM];
	struct dire dir[DIRNUM];
	char free[FREEBYTE];
};
