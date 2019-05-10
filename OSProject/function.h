#pragma   once  

#include "util.h"
 


// 在指定目录下创建文件
STATUS touch(char *path, char* fname)
{
	cout << "fuck" << endl;
	int ino = getnode(path);
	if (ino == -1) {
		return ERR_PATH_FAIL;
	}
	
	int n_ino = add_new_fnode(FILEMODE);
	add_file_to_direct(ino, n_ino, fname);
	return SUCCESS;
}

// 在指定目录下创建文件
STATUS createFile(char *path, char* fname, int size_kb)
{
	cout << "what" << endl;
	int block_num = size_kb*1024/BSIZE;
	if( block_num < 0 ){
		cout << "Error: file size can not small than zero" << endl;
		return ERR_FILE_SIZE;
	}
	if( block_num > 266 ){ // 256+10
 		cout << "Error: file size is too large" << endl;
		return ERR_FILE_SIZE; 
	}
	
	int fsize = block_num*1024;
	int ino = getnode(path);
	if (ino == -1) {
		return ERR_PATH_FAIL;
	}
	cout << "what" << endl;
	int n_ino = add_new_fnode(FILEMODE);
		
	int direct_i = add_file_to_direct(ino, n_ino, fname);

	update_file_size(n_ino, fsize);
	
	int double_addr_block_id;
	cout << "what" << endl;
	if( block_num >= 10 ){
		int k = find_free_block();
		root->fnode[n_ino].double_addr = k;
		double_addr_block_id = k;
		root->root.s_freeblock[k] = 1;
		root->root.s_freeblocksize--;
		memset(root->free+k*BSIZE, 0, 1024);
	}
	
	for(int j = 0; j < block_num; j++){
		int k = find_free_block();
		root->root.s_freeblock[k] = 1;
		root->root.s_freeblocksize--;
		if( j < 10 ){
			root->fnode[n_ino].fi_addr[j] = k;
		}
		else{
			update_addr_in_double_addr_block(double_addr_block_id, j-10, k);
		}
		int start_addr = k * BSIZE;
		int end = 1024;
		cout << "start_addr " << start_addr << endl;
		for ( int l = 0; l < end; l++){
			root->free[start_addr+l] = 'a';	// random fill
		}
	}
	return SUCCESS;
}

// 创建目录
STATUS mkdir(char *path, char* pname)
{
	int ino = getnode(path);
	if (ino == -1)
	{
		return ERR_PATH_FAIL;
	}
	int n_ino;
	int d_ino;
	// 申请新目录
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (root->dir[i].size == 0)
		{
			root->dir[i].size = 1;
			d_ino = i;
			break;
			//root->dir[i].direct[0].d_ino = n_ino;
		}
	}
	//申请请finode节点
	for (int i = 0; i < NUM; i++)
		if (root->fnode[i].fi_nlink != 1)
		{
			n_ino = i;
			root->fnode[i].fi_mode = DIRMODE;
			root->fnode[i].fi_size = 0;
			root->fnode[i].dir_no = d_ino;
			root->fnode[i].fi_addr[0] = 0;
			root->fnode[i].fi_nlink = 1;
			break;
		}
	// 在父亲节点建立指针
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strlen(root->dir[root->fnode[ino].dir_no].direct[i].d_name) == 0)
		{
			root->dir[root->fnode[ino].dir_no].direct[i].d_ino = n_ino;
			root->dir[root->fnode[ino].dir_no].size++;
			strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, pname);
			break;
		}
	}
	return SUCCESS;
}
// 进入目录回退上一级目录
STATUS cd(char *topath)
{
	char path[NAMESIZE*DIRNUM] = "";
	//回退上一级目录
	if (!strcmp(topath, ".."))
	{
		//
		int len=1;
		//cout << strlen(PATH);

		for (int i = strlen(PATH)-1; i > 0; i--)
		{
			if (PATH[i] == '/')
			{
				len = i;
				break;
			}
			//cout << len;
		}
		
		strncpy(path, PATH, len);
		strcpy(PATH, path);
	}
	// 进入目录
	else
	{
		// if(check_path_exist(topath)){
		// 	strcpy(path, PATH);
		// 	strcat(path, "/");
		// 	strcat(path, topath);
		// 	if (getnode(path) == -1 || getnode(path) == 0)
		// 		cout << "Error: the input dir is error, enter faile" << endl;
		// 	else
		// 	{
		// 		strcpy(PATH, path);
		// 	}
		int e = check_path_exist(topath);
		if(e==-1){
			cout<<"Error: the input dir path is error"<<endl;
			return ERR_PATH_FAIL;
		}
		else
		{
			if(topath[0]=='/'){
				strcpy(PATH,topath);
			}
			else{
				if(strcmp(PATH,"/")){
					strcat(PATH,"/");
					strcat(PATH,topath);
				}
				else
				{
					strcat(PATH,topath);
				}
							
			}
		}
		

		//cout << path;
		
	}
	//cout << p;
	return SUCCESS;
}

// 列出目录
STATUS ls(char *path)
{
	int ino = getnode(path);
	cout << setw(10) << "NAME" << setw(5) << "type" << setw(6) << "size" << endl;
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strlen(root->dir[root->fnode[ino].dir_no].direct[i].d_name) != 0)
		{
			cout << setw(10) << root->dir[root->fnode[ino].dir_no].direct[i].d_name;
			if (root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_mode == DIRMODE)
			{
				cout << setw(5) << "DIR" << setw(6) << "-";

			}
			else
			{
				cout << setw(5) << "FILE" << setw(6) << root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size;
			}
			cout << endl;
		}
	}
	return SUCCESS;
}

// 文件改名
STATUS rename(char *path, char *cname, char *nname)
{
	int ino = getnode(path);
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strcmp(root->dir[root->fnode[ino].dir_no].direct[i].d_name, cname) == 0)
		{
			for (int j = 0; j < DIRSIZE; j++)
			{
				if (strcmp(root->dir[root->fnode[ino].dir_no].direct[j].d_name, nname) == 0)
				{
					cout << "Error: file name repeat" << endl;
					return ERR_FILE_EXIST;
				}
			}
			strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, nname);
		}
	}
	return SUCCESS;
}
// 文件移位
STATUS mv(char *path, char *file, char *npath)
{

	int ino = getnode(path);
	//从目录中删除节点
	int n_ino;
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strcmp(root->dir[root->fnode[ino].dir_no].direct[i].d_name, file) == 0)
		{
			strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, "");
			n_ino = root->dir[root->fnode[ino].dir_no].direct[i].d_ino;
			root->dir[root->fnode[ino].dir_no].size--;
		}
	}
	ino = getnode(npath);
	// 建立新节点表
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strlen(root->dir[root->fnode[ino].dir_no].direct[i].d_name) == 0)
		{
			root->dir[root->fnode[ino].dir_no].direct[i].d_ino = n_ino;
			strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, file);
			break;
		}
	}

	return SUCCESS;
}

// 删除文件
int rm(char *path, char *file)
{
	int ino = getnode(path);

	int n_ino;
	//从目录中移除节点
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strcmp(root->dir[root->fnode[ino].dir_no].direct[i].d_name, file) == 0)
		{
			strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, "");
			n_ino = root->dir[root->fnode[ino].dir_no].direct[i].d_ino;
			root->dir[root->fnode[ino].dir_no].size--;
		}
		else
		{
			cout << "Error: file is not exit" << endl;
			return ERR_FILE_NOT_EXIST;
		}
	}
	for (int i = 0; i < 1 + (root->fnode[n_ino].fi_size / BSIZE); i++)
	{
		root->root.s_freeblock[root->fnode[n_ino].fi_addr[i]] = 0;
		root->root.s_freeblocksize++;
	}
	root->fnode[n_ino].fi_nlink = 0;
	return SUCCESS;
}

// 查询空闲空间
STATUS free()
{
	cout << "[";
	int l = (int)40.0*(1.0*root->root.s_freeblocksize / BNUM);
	//cout << l;

	for (int i = 0; i < 40 - l; i++)
	{
		cout << "=";
	}
	for (int i = 0; i < l; i++)
	{
		cout << " ";
	}
	cout << "] ";
	cout << (int)(100.0*root->root.s_freeblocksize / BNUM) << "% free" << endl;
	return SUCCESS;
}

//读文件
STATUS cat(char *path, char *fname)
{
	int ino = getnode(path);
	
	int direct_i = get_file_direct_id_in_fnode(ino, fname);
	
	if ( get_file_mode(ino, direct_i) == DIRMODE)
	{
		cout << "This is a dir" << endl;
	}
	else
	{
		// 确定有多少块
		int fsize = get_file_size(ino, direct_i);
		int remainder = fsize%BSIZE;
		int block_num = remainder ? 1 + fsize/BSIZE : fsize/BSIZE;
		
		finode now_fnode = get_direct_fnode(ino, direct_i);
		
		for (int j = 0; j < block_num; j++)
		{
			int start_addr;
			if( j < 10 ) start_addr = BSIZE*now_fnode.fi_addr[j] ;
			else start_addr = BSIZE*get_double_addr_block_addr(now_fnode.double_addr,j-10);
			int end = (j == block_num-1) ? remainder : 1024 ;
			if( end == 0 ) end = 1024;
			for (int k = 0; k < end; k++){
				cout << root->free[start_addr+k];
			}
			cout << "j " << j << " start_addr " << start_addr << "  end " << end << endl;
		}
	}
	cout << endl;
	return SUCCESS;
}
// 写文件
STATUS vi(char *path, char *file, char *cont)
{
	int ino = getnode(path);
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strcmp(root->dir[root->fnode[ino].dir_no].direct[i].d_name, file) == 0)
		{
			if (root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_mode == DIRMODE)
			{
				cout << "This is a dir" << endl;
			}
			else
			{
				root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size = strlen(cont);
				// 确定有多少块
				for (int j = 0; j < 1 + strlen(cont) / BSIZE; j++)
				{
					// 寻找空闲块
					for (int k = 0; k < BNUM; k++)
					{

						if (root->root.s_freeblock[k] != 1)
						{
							// 写入空间块
							int l, m;
							root->root.s_freeblock[k] = 1;
							root->root.s_freeblocksize--;
							root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_addr[j] = k;
							for (l = k * BSIZE, m = j * BSIZE; l < k*BSIZE + BSIZE; m++, l++)
							{
								root->free[l] = cont[m];

							}
							break;
						}
					}
				}
			}
			break;
		}
	}
	return SUCCESS;
}

// 写出文件
STATUS writeout()
{
	FILE *fp;
	if ((fp = fopen("filesystem.dat", "w")) == NULL)
	{
		cout << "Error: fail to write to disk!" << endl;
		return ERR_FILE_FAIL;
	}

	if (fwrite(root, sizeof(struct storage), 1, fp) != 1)
	{
		cout << "Error: fail to write to disk!" << endl;
	}

	fclose(fp);
	return SUCCESS;
}
//读取文件
STATUS readin()
{

	FILE *fp;
	if ((fp = fopen("filesystem.dat", "r")) == NULL)
	{
		cout << "Error: fail to read os from disk!" << endl;
		return ERR_FILE_FAIL;
	}
	if (fread(root, sizeof(struct storage), 1, fp))
	{
		fclose(fp);
		return SUCCESS;
	}
	return SUCCESS;
}
// 系统初始化
void init()
{

	memset(root->fnode, '\0', FREEBYTE);
	root->dir[0].direct[0].d_ino = 0;
	root->dir[0].size = 1;
	strcpy(root->dir[0].direct[0].d_name, "/"); //设置根目录名
	root->fnode[0].fi_mode = DIRMODE;
	root->fnode[0].fi_nlink = 1;
	root->fnode[0].dir_no = 1;//1
	root->dir[1].size = 1;
	root->root.s_freeblocksize = BNUM;
	strcpy(PATH, "/");
}
