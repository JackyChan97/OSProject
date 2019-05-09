#define NUM 1024	//�ܽڵ���      1024
#define BNUM 15360	//���̿���  15M
#define BSIZE 1024 //���С	           1024
#define FSIZE 2048	//�������С	     
#define SSIZE 16756736 //�洢�ռ��С
#define NAMESIZE 16	//�ļ�������
#define DIRSIZE 16	//Ŀ¼��С
#define DIRNUM 32	//Ŀ¼����
#define FREEBYTE 16681920 //�ռ��ܴ�С
#define DIRMODE 0 //Ŀ¼����
#define FILEMODE 1	//�ļ�����

// ����״̬
enum STATUS
{
	SUCCESS, ERR_PATH_FAIL, ERR_FILE_EXIST, ERR_FILE_NOT_EXIST, ERR_FILE_FAIL, ERR_FILE_SIZE
};
//������
struct superblock
{
	unsigned int s_size;	//�ܴ�С
	unsigned int s_itsize;	//inode���С
	unsigned int s_freeinodesize;	//����i�ڵ������
	unsigned int s_nextfreeinode;	//��һ������i�ڵ�
	unsigned int s_freeinode[NUM];	//����i�ڵ����� 0/1
	unsigned int s_freeblocksize;	//���п������          
	unsigned int s_nextfreeblock;	//��һ�����п�
	unsigned char s_freeblock[BNUM];	//���п�����0/1  
	char blank[1000];
};
//һ�μ��Ѱַ
struct atime
{
	int fi_addr[256];
};
// �ļ��ڵ�
struct finode
{
	int fi_mode;	//���ͣ��ļ�/Ŀ¼
	int fi_nlink;		//����������������Ϊ0����ζ�ű�ɾ��
	int dir_no;		//Ŀ¼��   �����Ŀ¼�Ļ�
	long int fi_size;	//�ļ���С
	long int fi_addr[10];	//�ļ���һ��ָ�룬��δʵ�ֶ༶ָ��
	struct atime  *fi_atime_unused;	//����Ѱַ
	char blank[4];

};
//Ŀ¼��ṹ
struct direct
{
	char d_name[NAMESIZE];        //�ļ�����Ŀ¼������
	unsigned short d_ino;        //�ļ�����Ŀ¼��i�ڵ��
};
//Ŀ¼�ṹ
struct dire
{
	struct direct direct[DIRSIZE];    //������Ŀ¼������
	unsigned short size;        //������Ŀ¼���С
};

// ����
struct storage
{
	struct superblock root;
	struct finode fnode[NUM];
	struct dire dir[DIRNUM];
	char free[FREEBYTE];
};
