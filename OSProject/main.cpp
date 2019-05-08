#include"function.cpp"

void menu()
{
	cout << "******************************************************************************" << endl;
	cout << "*                   information of our groups                                *" << endl;
	cout << "*                     郑滨雄 ： 201630588474                                 *" << endl;
	cout << "*                     陈卓杰 ： 201630588474                                 *" << endl;
	cout << "*                     叶锐智 ： 201630588474                                 *" << endl; 
	cout << "******************************************************************************" << endl;
	cout << "******************************************************************************" << endl;
	cout << "*                             文件系统操作手册                               *" << endl;
	cout << "*                                                                            *" << endl;
	cout << "*                  1、 mkdir <dir>  --创建目录                               *" << endl;
	cout << "*                  2、 touch <file>  --创建文件                              *" << endl;
	cout << "*                  3、 cat <file>  --读取文件                                *" << endl;
	cout << "*                  4、 vi <file>   --编辑文件                                *" << endl;
	cout << "*                  5、 rm <file>  --删除目录/文件                            *" << endl;
	cout << "*                  6、 rename <src> <dest>  --重命名                         *" << endl;
	cout << "*                  7、 mv <file> <dir>  --移动文件                           *" << endl;
	cout << "*                  8、 cd <dir>  --打开目录                                  *" << endl;
	cout << "*                  9、 cd ..  --返回上一级目录                               *" << endl;
	cout << "*                  10、pwd  --查看当前路径                                   *" << endl;
	cout << "*                  11、ls  --列现当前目录                                    *" << endl;
	cout << "*                  12、free  --显示磁盘可用空间                              *" << endl;
	cout << "*                  13、writeout  --写入到磁盘                                *" << endl;
	cout << "*                  14、readin  --从磁盘读取文件系统                          *" << endl;
	cout << "*                  15、format  --格式化磁盘                                  *" << endl;
	cout << "*                  16、help  --显示帮助信息                                  *" << endl;
	cout << "*                  17、exit  --退出文件系统                                  *" << endl;
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
			cout << endl << "请输入要写入文件的内容：" << endl;
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
			cout << "已写出到 filesystem.dat" << endl;
			// 写出到 filesystem.dat
		}
		else if (s == "readin")
		{
			readin();
			cout << "已从 filesystem.dat 中读入文件系统" << endl;
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
			cout << "命令错误，请重新输入！" << endl;
		}
	}
	return 0;
}
