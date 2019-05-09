#include "util.cpp"
// ����·������ȡfinode�ڵ��
int getnode(char *path)
{
	if (path[0] != '/')
	{
		return -1;	//·���Ƿ�
	}
	else
	{
		struct dire cdir = root->dir[0];
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
						return -1;	//���ļ��������ļ���
					}
				}

			}
			fpath = strtok(NULL, "/");
		}
		return ino;	//Ϊ���ڵ㡰/��
	}
	return -1;	//����
}

// ��ָ��Ŀ¼�´����ļ�
STATUS touch(char *path, char* fname)
{
	int ino = getnode(path);
	if (ino == -1) {
		return ERR_PATH_FAIL;
	}
	
	int n_ino;
	for (int i = 0; i < NUM; i++){
		if (root->fnode[i].fi_nlink != 1) {
			add_new_fnode(i, FILEMODE);
			n_ino = i;
			break;
		}	
	}
		
	for (int i = 0; i < DIRSIZE; i++) {
		if (strlen(get_file_name(ino, i)) == 0)	{
			add_file_to_direct(ino, i, n_ino, fname);
			break;
		}
	}
	return SUCCESS;
}

// ��ָ��Ŀ¼�´����ļ�
STATUS createFile(char *path, char* fname, int fsize)
{
	if( fsize < 0 ){
		cout << "Error: file size can not small than zero" << endl;
		return ERR_FILE_SIZE;
	}
	if( fsize > 266 ){ // 256+10
 		cout << "Error: file size is too large" << endl;
		return ERR_FILE_SIZE; 
	}
	fsize = fsize*1024;
	int ino = getnode(path);
	if (ino == -1) {
		return ERR_PATH_FAIL;
	}
	
	int n_ino;
	for (int i = 0; i < NUM; i++){
		if (root->fnode[i].fi_nlink != 1) {
			add_new_fnode(i, FILEMODE);
			n_ino = i;
			break;
		}	
	}
		
	int direct_i;
	for (int i = 0; i < DIRSIZE; i++) {
		if (strlen(get_file_name(ino, i)) == 0)	{
			add_file_to_direct(ino, i, n_ino, fname);
			direct_i = i; 
			break;
		}
	}
	
	update_file_size(n_ino, fsize);
	
	// ȷ���ж��ٿ�
	int block_num = fsize%BSIZE ? 1 + fsize/BSIZE : fsize/BSIZE;
	if( block_num >= 10 ){
		int k = find_free_block();
		root->fnode[n_ino].double_addr = k;
		root->root.s_freeblock[k] = 1;
		root->root.s_freeblocksize--;
	}
	
	for (int j = 0; j < block_num; j++){
		if( j < 10 ){
			// Ѱ�ҿ��п�
			int k = find_free_block();
			int l, m;
			root->root.s_freeblock[k] = 1;
			root->root.s_freeblocksize--;
			root->fnode[n_ino].fi_addr[j] = k;
			for (l = k * BSIZE, m = j * BSIZE; l < k*BSIZE + BSIZE; m++, l++){
				char tmp = 'A' ; // random fill
				root->free[l] = tmp;
			}
		}
		else{
			
		}
		
	}

	return SUCCESS;
}

// ����Ŀ¼
STATUS mkdir(char *path, char* pname)
{
	int ino = getnode(path);
	if (ino == -1)
	{
		return ERR_PATH_FAIL;
	}
	int n_ino;
	int d_ino;
	// ������Ŀ¼
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
	//������finode�ڵ�
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
	// �ڸ��׽ڵ㽨��ָ��
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
// ����Ŀ¼������һ��Ŀ¼
STATUS cd(char *topath)
{
	char path[NAMESIZE*DIRNUM] = "";
	//������һ��Ŀ¼
	if (!strcmp(topath, ".."))
	{
		//
		int len;
		//cout << strlen(PATH);
		for (int i = strlen(PATH); i >= 0; i--)
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
	// ����Ŀ¼
	else
	{
		strcpy(path, PATH);
		strcat(path, "/");
		strcat(path, topath);
		if (getnode(path) == -1 || getnode(path) == 0)
			cout << "Error: the input dir is error, enter faile" << endl;
		else
		{
			strcpy(PATH, path);
		}

		//cout << path;

	}
	//cout << p;
	return SUCCESS;
}

// �г�Ŀ¼
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

// �ļ�����
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
// �ļ���λ
STATUS mv(char *path, char *file, char *npath)
{

	int ino = getnode(path);
	//��Ŀ¼��ɾ���ڵ�
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
	// �����½ڵ��
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

// ɾ���ļ�
int rm(char *path, char *file)
{
	int ino = getnode(path);

	int n_ino;
	//��Ŀ¼���Ƴ��ڵ�
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

// ��ѯ���пռ�
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

//���ļ�
STATUS cat(char *path, char *file)
{
	int ino = getnode(path);
	for (int i = 0; i < DIRSIZE; i++)
	{
		if (strcmp( get_file_name(ino, i), file) == 0)
		{
			if ( get_file_mode(ino, i) == DIRMODE)
			{
				cout << "This is a dir" << endl;
			}
			else
			{
				// ȷ���ж��ٿ�
				int bNum = 1 +  get_file_size(ino, i) / BSIZE ;
				for (int j = 0; j < bNum; j++)
				{
//					cout << (BSIZE *root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_addr[j]) << endl;
//					cout << ((root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_addr[j] * BSIZE) + (
//						j>(root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size / BSIZE) ? BSIZE :
//						(root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size%BSIZE)));
//					cout << (root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size / BSIZE) << endl;;
					for (int k = (BSIZE *root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_addr[j]);
						k < ((root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_addr[j] * BSIZE) +
						(j > (root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size / BSIZE) ? BSIZE :
							(root->fnode[root->dir[root->fnode[ino].dir_no].direct[i].d_ino].fi_size%BSIZE))); k++)
					{
						//cout << k;
						cout << root->free[k];
					}
				}
			}
			break;
		}
	}
	cout << endl;
	return SUCCESS;
}
// д�ļ�
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
				// ȷ���ж��ٿ�
				for (int j = 0; j < 1 + strlen(cont) / BSIZE; j++)
				{
					// Ѱ�ҿ��п�
					for (int k = 0; k < BNUM; k++)
					{

						if (root->root.s_freeblock[k] != 1)
						{
							// д��ռ��
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

// д���ļ�
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
//��ȡ�ļ�
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
// ϵͳ��ʼ��
void init()
{

	memset(root->fnode, '\0', FREEBYTE);
	root->dir[0].direct[0].d_ino = 0;
	root->dir[0].size = 1;
	strcpy(root->dir[0].direct[0].d_name, "C"); //���ø�Ŀ¼��
	root->fnode[0].fi_mode = DIRMODE;
	root->fnode[0].fi_nlink = 1;
	root->fnode[0].dir_no = 1;
	root->dir[1].size = 1;
	root->root.s_freeblocksize = BNUM;
	strcpy(PATH, "/C");
}
