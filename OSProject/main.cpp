#include"function.cpp"

void menu()
{
	cout << "******************************************************************************" << endl;
	cout << "*                   information of our groups                                *" << endl;
	cout << "*                     ֣���� �� 201630588474                                 *" << endl;
	cout << "*                     ��׿�� �� 201630588474                                 *" << endl;
	cout << "*                     Ҷ���� �� 201630588474                                 *" << endl; 
	cout << "******************************************************************************" << endl;
	cout << "******************************************************************************" << endl;
	cout << "*                             �ļ�ϵͳ�����ֲ�                               *" << endl;
	cout << "*                                                                            *" << endl;
	cout << "*                  1�� mkdir <dir>  --����Ŀ¼                               *" << endl;
	cout << "*                  2�� touch <file>  --�����ļ�                              *" << endl;
	cout << "*                  3�� cat <file>  --��ȡ�ļ�                                *" << endl;
	cout << "*                  4�� vi <file>   --�༭�ļ�                                *" << endl;
	cout << "*                  5�� rm <file>  --ɾ��Ŀ¼/�ļ�                            *" << endl;
	cout << "*                  6�� rename <src> <dest>  --������                         *" << endl;
	cout << "*                  7�� mv <file> <dir>  --�ƶ��ļ�                           *" << endl;
	cout << "*                  8�� cd <dir>  --��Ŀ¼                                  *" << endl;
	cout << "*                  9�� cd ..  --������һ��Ŀ¼                               *" << endl;
	cout << "*                  10��pwd  --�鿴��ǰ·��                                   *" << endl;
	cout << "*                  11��ls  --���ֵ�ǰĿ¼                                    *" << endl;
	cout << "*                  12��free  --��ʾ���̿��ÿռ�                              *" << endl;
	cout << "*                  13��writeout  --д�뵽����                                *" << endl;
	cout << "*                  14��readin  --�Ӵ��̶�ȡ�ļ�ϵͳ                          *" << endl;
	cout << "*                  15��format  --��ʽ������                                  *" << endl;
	cout << "*                  16��help  --��ʾ������Ϣ                                  *" << endl;
	cout << "*                  17��exit  --�˳��ļ�ϵͳ                                  *" << endl;
	cout << "******************************************************************************" << endl;
}



int main()
{
	menu();
	string s;
	char arg1[NAMESIZE] = "";
	char arg2[NAMESIZE] = "";
	
	init();
	while (1)
	{
		cin >> s;
		if (s == "mkdir")
		{
			//cout << "mkdir";
			cin >> arg1;
			mkdir(PATH, arg1);
			ls(PATH);

		}
		else if (s == "format")
		{
			memset(root, '\0', sizeof(struct storage));
			init();
		}
		else if (s == "touch")
		{
			cin >> arg1;
			touch(PATH, arg1);
			//ls(PATH);
		}
		else if (s == "rm")
		{
			cin >> arg1;
			rm(PATH, arg1);
			//ls(PATH);
		}
		else if (s == "mv")
		{
			cin >> arg1 >> arg2;
			mv(PATH, arg1, arg2);
			//ls(PATH);
		}
		else if (s == "rename")
		{
			cin >> arg1 >> arg2;
			rename(PATH, arg1, arg2);
			//ls(PATH);
		}
		else if (s == "cat")
		{
			cin >> arg1;
			cat(PATH, arg1);
			//ls(PATH);
		}
		else if (s == "vi")
		{
			cin >> arg1;
			cout << endl << "������Ҫд���ļ������ݣ�" << endl;
			cin >> content;
			vi(PATH, arg1, content);
			//ls(PATH);
		}
		else if (s == "cd")
		{
			cin >> arg1;
			cd(arg1);
			//ls(PATH);
		}
		else if (s == "pwd")
		{
			cout << PATH << endl;
		}
		else if (s == "ls")
		{
			ls(PATH);
		}
		else if (s == "writeout")
		{
			writeout();
			cout << "��д���� filesystem.dat" << endl;
			// д���� filesystem.dat
		}
		else if (s == "readin")
		{
			readin();
			cout << "�Ѵ� filesystem.dat �ж����ļ�ϵͳ" << endl;
		}
		else if (s == "exit")
		{
			break;
		}
		else if (s == "free")
		{
			free();
		}
		else
		{
			cout << "����������������룡" << endl;
		}
	}
	return 0;
}
