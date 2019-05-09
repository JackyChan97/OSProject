#include"function.cpp"

void menu()
{
	cout << "******************************************************************************" << endl;
	cout << "*                   information of our groups                                *" << endl;
	cout << "*                 Zheng BinXiong : 201630588474                             *" << endl;
	cout << "*                 Chen ZhuoJie   : 201630588474                             *" << endl;
	cout << "*                 Ye RuiZhi      : 201630588474                             *" << endl; 
	cout << "******************************************************************************" << endl;
	cout << "******************************************************************************" << endl;
	cout << "*                             OS-Operation Guide                             *" << endl;
	cout << "*                                                                            *" << endl;
	cout << "*                  1. mkdir <dir>  --create dir                             *" << endl;
	cout << "*                  2. touch <file>  --create file                           *" << endl;
	cout << "*                  3. cat <file>  --read file                               *" << endl;
	cout << "*                  4. vi <file>   --edit file                               *" << endl;
	cout << "*                  5. rm <file>  --delete dir/file                          *" << endl;
	cout << "*                  6. rename <src> <dest>  --rename                         *" << endl;
	cout << "*                  7. mv <file> <dir>  --move dir/file                      *" << endl;
	cout << "*                  8. cd <dir>  --enter dir                                 *" << endl;
	cout << "*                  9. cd ..  --return parent's dir                          *" << endl;
	cout << "*                  10.pwd  --show present path                              *" << endl;
	cout << "*                  11.ls  --list present dir's file                         *" << endl;
	cout << "*                  12.free  --show free space of disk                       *" << endl;
	cout << "*                  13.writeout  --write to disk                             *" << endl;
	cout << "*                  14.readin  --read os from disk                           *" << endl;
	cout << "*                  15.format  --format the disk                             *" << endl;
	cout << "*                  16.help  --show help information                         *" << endl;
	cout << "*                  17.exit  --exit the os                                   *" << endl;
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
		else if (s == "createFile")
		{
			int fsize;
			cin >> arg1 >> fsize;
			 
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
			cout << endl << "Please input the content of the file:" << endl;
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
			cout << "success write to filesystem.dat" << endl;
		}
		else if (s == "readin")
		{
			readin();
			cout << "success read os from filesystem.dat" << endl;
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
			cout << "order is no exit! Please try again!" << endl;
		}
	}
	return 0;
}
