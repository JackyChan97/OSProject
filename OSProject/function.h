#ifndef FUNCTION_H
#define FUNCTION_H



#include "util.h"
 


// 在指定目录下创建文件
STATUS touch(char *path, char* fname)
{
	int ino = getnode(path);
	if (ino == -1) {
		return ERR_PATH_FAIL;
	}
	
	int n_ino = add_new_fnode(FILEMODE,-1);
	add_file_to_direct(ino, n_ino, fname);
	return SUCCESS;
}

// 在指定目录下创建文件
STATUS createFile(char *path, char* fname, int size_kb)
{
	int block_num = size_kb*1024/BSIZE;
	if( block_num < 0 ){
		cout << "Error: file size can not small than zero" << endl;
		return ERR_FILE_SIZE;
	}
	if( block_num > 266 ){ // 256+10
 		cout << "Error: file size is too large" << endl;
		return ERR_FILE_SIZE; 
	}
	
	if( root->root.s_freeinodesize == 0 ){
		cout << "Error: finode out of range" << endl;
		return ERR;
	}

	int fsize = block_num*1024;
	
	if( fname[0] == '/' ){
		
	}

	
	int ino = getnode(path);
	if (ino == -1) {
		return ERR_PATH_FAIL;
	}
	
	if( check_file_exist (ino, fname) ){
		cout << "file name already exist!" << endl;
		return ERR_FILE_EXIST;
	}
	
	int n_ino = add_new_fnode(FILEMODE,-1);
		
	int direct_i = add_file_to_direct(ino, n_ino, fname);

	update_file_size(n_ino, fsize);
	
	int double_addr_block_id;
	if( block_num >= 10 ){
		int k = find_free_block();
		root->fnode[n_ino].double_addr = k;
		double_addr_block_id = k;
		root->root.s_freeblock[k] = 1;
		root->root.s_freeblocksize--;
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
		for ( int l = 0; l < end; l++){
			root->free[start_addr+l] = 'a';	// random fill
		}
	}
	return SUCCESS;
}

// 创建目录
// STATUS mkdir(char *path, char* pname)
// {
// 	int ino = getnode(path);
// 	if (ino == -1)
// 	{
// 		return ERR_PATH_FAIL;
// 	}
// 	int n_ino;
// 	int d_ino;
// 	// 申请新目录
// 	for (int i = 0; i < DIRSIZE; i++)
// 	{
// 		if (root->dir[i].size == 0)
// 		{
// 			root->dir[i].size = 1;
// 			d_ino = i;
// 			break;
// 			//root->dir[i].direct[0].d_ino = n_ino;
// 		}
// 	}
// 	//申请请finode节点
// 	for (int i = 0; i < NUM; i++)
// 		if (root->fnode[i].fi_nlink != 1)
// 		{
// 			n_ino = i;
// 			root->fnode[i].fi_mode = DIRMODE;
// 			root->fnode[i].fi_size = 0;
// 			root->fnode[i].dir_no = d_ino;
// 			root->fnode[i].fi_addr[0] = 0;
// 			root->fnode[i].fi_nlink = 1;
// 			break;
// 		}
// 	// 在父亲节点建立指针
// 	for (int i = 0; i < DIRSIZE; i++)
// 	{
// 		if (strlen(root->dir[root->fnode[ino].dir_no].direct[i].d_name) == 0)
// 		{
// 			root->dir[root->fnode[ino].dir_no].direct[i].d_ino = n_ino;
// 			root->dir[root->fnode[ino].dir_no].size++;
// 			strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, pname);
// 			break;
// 		}
// 	}
// 	return SUCCESS;
// }

// 创建目录
STATUS create_dir(char *path, char* pname)
{
	if( pname[0] == '/' ){
		if(getnode(pname)!=-1){
			cout<<"DIR already exist!"<<endl;
			return ERR;
		}
		char tpath[NAMESIZE*DIRNUM]="";
		strcpy(tpath,pname);
		// cout<<"tpath:"<<tpath<<endl;
		char* fpath = strtok(tpath,"/");
		char tmp_path[NAMESIZE*DIRNUM]="";
		char * index =NULL;
		while(fpath != NULL){
			char new_path[NAMESIZE*DIRNUM];
			
			strcat(tmp_path,"/");
			strcpy(new_path, tmp_path);
			strcat(new_path, fpath);
			index = fpath + strlen(fpath) + 1;
			if(getnode(new_path)!=-1){
				//cout<<"f"<<endl;
				
				strcat(tmp_path,fpath);
				
			}
			else{
				// cout<<"y"<<endl;
				create_dir(tmp_path,fpath);
				// cout<<"yy"<<endl;
				// cout<<"tmp_path:"<<tmp_path<<endl;
				strcat(tmp_path,fpath);
			}
			fpath = strtok(index, "/");
			// cout<<"fpath: "<<fpath<<endl;
			
			// if(fpath==NULL)
				// cout<<"hi"<<endl;
		}

		
	}
	else{
		if(check_file_exist(getnode(path),pname)){
			cout<<"DIR already exist!"<<endl;
			return ERR;
		}
		int ino = getnode(path);
		if (ino == -1)
		{
			return ERR_PATH_FAIL;
		}
		int n_ino;
		int d_ino ;
		// 申请新目录
		for (int i = 0; i < DIRNUM; i++)
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
		n_ino=add_new_fnode(DIRMODE,d_ino);
		// 在父亲节点建立指针
		for (int i = 0; i < DIRSIZE; i++)
		{
			if (strlen(get_file_name(ino,i)) == 0)
			{
				root->dir[root->fnode[ino].dir_no].direct[i].d_ino = n_ino;
				root->dir[root->fnode[ino].dir_no].size++;
				strcpy(root->dir[root->fnode[ino].dir_no].direct[i].d_name, pname);
				break;
			}
		}
		return SUCCESS;
	}
}

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
	//cout<<"ino: "<<ino<<endl;
	cout << setw(10) << "NAME" << setw(5) << "type" << setw(6) << "size" <<setw(23) <<"CreatedTime"<<endl;
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strlen(get_file_name(ino,i)) != 0)
		{
			cout << setw(10) << get_file_name(ino,i);
			if (root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_mode == DIRMODE)
			{
				char tmp[30];
				strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",localtime(&(root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].createdTime)));
				cout << setw(5) << "DIR" << setw(6) << "-"<<setw(23)<<tmp;
				

			}
			else
			{
				char str_time[20];
				strftime(str_time,sizeof(str_time),"%Y-%m-%d %H:%M:%S",localtime(&(root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].createdTime)));
				cout << setw(5) << "FILE" << setw(6) << root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size<<setw(23)<<str_time;
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
int rm_file(char *path, char *fname)
{
	int ino = getnode(path);
	
	if( !check_file_exist(ino, fname) ){
		cout << "file not exist!" << endl;
		return ERR_FILE_NOT_EXIST;
	}
	
	int direct_i = get_file_direct_id_in_fnode(ino, fname);
	char name[NAMESIZE*DIRNUM] = "";
	update_direct_name(ino, direct_i, name);
	root->dir[root->fnode[ino].dir_no].size--;

	int fsize = get_file_size(ino, direct_i);
	int remainder = fsize%BSIZE;
	int block_num = fsize/BSIZE + (remainder ? 1:0);
	
	finode now_fnode = get_direct_fnode(ino, direct_i);
	
	for (int j = 0; j < block_num; j++)
	{
		int block_addr;
		if( j < 10 ) block_addr = BSIZE*now_fnode.fi_addr[j] ;
		else block_addr = get_double_addr_block_addr(now_fnode.double_addr,j-10);
		root->root.s_freeblock[block_addr] = 0 ;
		root->root.s_freeblocksize++;
	}
	root->root.s_freeinodesize++;
	root->root.s_freeinode[ino] = 0 ;
	return SUCCESS;
}

// int rm(char *path, char *fname)
// {
// 	int ino = getnode(path);

// 	int n_ino = get_file_direct_id_in_fnode(ino, fname);
// 	//从目录中移除节点
// 	int direct_i = get_file_direct_id_in_fnode(ino, fname);
// 	strcpy(root->dir[root->fnode[ino].dir_no].direct[direct_i].d_name, "");
// 	n_ino = root->dir[root->fnode[ino].dir_no].direct[direct_i].d_ino;
// 	root->dir[root->fnode[ino].dir_no].size--;

// 	for (int i = 0; i < 1 + (root->fnode[n_ino].fi_size / BSIZE); i++)
// 	{
// 		root->root.s_freeblock[root->fnode[n_ino].fi_addr[i]] = 0;
// 		root->root.s_freeblocksize++;
// 	}
// 	root->fnode[n_ino].fi_nlink = 0;
// 	return SUCCESS;
// }
int rm_dir(char *path, char *fname){
	if(fname[0]=='/'){
		if(!strncmp(path,fname,strlen(fname))){
			cout<<"Can not delete current path!"<<endl;
			return ERR;
		}
		int len =1;
		for (int i =strlen(fname)-1;i>0;i--){
			if(fname[i] =='/'){
				len = i;
				break;
			}
		}
		char new_path[NAMESIZE*DIRNUM] ="";
		char new_name[NAMESIZE*DIRNUM] ="";
		strncpy(new_path,fname,len);
		for(int i=len+1;i<strlen(fname);i++){
			new_name[i-len-1]=fname[i];
		}
		rm_dir(new_path,new_name);
	}
	else{
		char tmp[NAMESIZE*DIRNUM]="";
		if(strcmp(path,"/"))
			strcpy(tmp,path);
		strcat(tmp,"/");
		strcat(tmp,fname);
		int ino_of_dir=getnode(tmp);
		if(ino_of_dir==-1){
			cout<<"DIR do not exist!"<<endl;
			return ERR_FILE_NOT_EXIST;
		}
		if(root->fnode[ino_of_dir].fi_mode!=DIRMODE){
			cout<<"This is not a DIR"<<endl;
			return ERR;
		}
		int dir_no = root->fnode[ino_of_dir].dir_no;
		for(int i=0;i<DIRSIZE;i++){
			if(strlen(root->dir[dir_no].direct[i].d_name)!=0){
				int ino_of_inside = root->dir[dir_no].direct[i].d_ino;
				if(root->fnode[ino_of_inside].fi_mode==FILEMODE)
					rm_file(tmp,root->dir[dir_no].direct[i].d_name);
				else{
					rm_dir(tmp,root->dir[dir_no].direct[i].d_name);
				}
			}
		}
		int ino = getnode(path);
		rm_file(path,fname);
	}
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
		int block_num = fsize/BSIZE + (remainder ? 1:0);
		
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

STATUS cp(char *path, char* fname1, char* fname2)
{
	int ino = getnode(path);
	if (ino == -1) {
		return ERR_PATH_FAIL;
	}
	int old_direct_i = get_file_direct_id_in_fnode(ino, fname1);
	if (get_file_mode(ino, old_direct_i) == DIRMODE)
	{
		cout << "This is a dir" << endl;
		return ERR_FILE_FAIL;
	}
	int fsize = get_file_size(ino, old_direct_i);

	if (check_file_exist(ino, fname2)) {
		cout << "file name already exist!" << endl;
		return ERR_FILE_EXIST;
	}
	int block_num;
	if (fsize >= 1024)
		block_num = fsize / 1024 ;
	else
		block_num = 1;
	
	finode old_fnode = get_direct_fnode(ino, old_direct_i);

	int n_ino = copy_inode(old_fnode);


	int direct_i = add_file_to_direct(ino, n_ino, fname2);

	int double_addr_block_id;
	if (block_num >= 10) {
		int k = find_free_block();
		root->fnode[n_ino].double_addr = k;
		double_addr_block_id = k;
		root->root.s_freeblock[k] = 1;
		root->root.s_freeblocksize--;
	}

	for (int j = 0; j < block_num; j++) {
		int k = find_free_block();
		root->root.s_freeblock[k] = 1;
		root->root.s_freeblocksize--;
		if (j < 10) {
			root->fnode[n_ino].fi_addr[j] = k;
		}
		else {
			update_addr_in_double_addr_block(double_addr_block_id, j - 10, k);
		}
		int start_addr = k * BSIZE;
		int old_start_addr;
		if (j < 10)  old_start_addr = BSIZE*old_fnode.fi_addr[j];
		else old_start_addr = BSIZE*get_double_addr_block_addr(old_fnode.double_addr, j - 10);
		int end = 1024;
		for (int l = 0; l < end; l++) {
			char tmp = root->free[old_start_addr + l];
			root->free[start_addr + l] = tmp;
		}
	}
	return SUCCESS;
}

STATUS sum() {
	int used = 0;
	int unused = 0;
	for (int i = 0; i < BNUM; i++) {
		if (root->root.s_freeblock[i] == 1)
			used++;
	}
	unused = BNUM - used;
	cout << "Number of used blocks :" << used << endl;
	cout << "Number of unused blocks :" << unused << endl;
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
	root->root.s_freeinode[0] = 1;
	root->fnode[0].fi_nlink = 1;
	root->fnode[0].dir_no = 1;//1
	root->dir[1].size = 1;
	root->root.s_freeblocksize = BNUM;
	root->root.s_freeinodesize = NUM - 1;
	strcpy(PATH, "/");
}


#endif // !FUNCTION_H
