#define _CRT_SECURE_NO_WARNINGS
#include"function.h"


storage*root = new storage;
// 
char PATH[NAMESIZE*DIRNUM]="";
char content[BSIZE*BNUM]="";

void copyright(){
	cout << "*****************************************************************************" << endl;
	cout << "*                   information of our groups                               *" << endl;
	cout << "*                 Zheng BinXiong : 201630588474                             *" << endl;
	cout << "*                 Chen ZhuoJie   : 201630588245                             *" << endl;
	cout << "*                 Ye RuiZhi      : 201630588450                             *" << endl; 
	cout << "*****************************************************************************" << endl;

}

void help(){
	cout << "*****************************************************************************" << endl;
	cout << "*                             OS-Operation Guide                            *" << endl;
	cout << "*                                                                           *" << endl;
	cout << "*                  1. copyright                                             *" << endl;
	cout << "*                  2. createFile <file> <size> --Create a file              *" << endl;
	cout << "*                  3. deleteFile <file>  --Delete a file                    *" << endl;
	cout << "*                  4. createDir <dir>   --Create a directory                *" << endl;
	cout << "*                  5. deleteDir <dir>  --Delete a directory                 *" << endl;
	cout << "*                  6. cd <src> --Change current working direcotry           *" << endl;
	cout << "*                  7. dir <src> --List all the files and sub-directories    *" << endl;
	cout << "*                  8. cp <file1> <file2> --Copy a file                      *" << endl;
	cout << "*                  9. sum --Display the usage of storage space              *" << endl;
	cout << "*                  10.cat <file> --Print out the file contents              *" << endl;
	cout << "*                  11.readin  --Read from disk                              *" << endl;
	cout << "*                  12.writeout  --Save to disk                              *" << endl;
	cout << "*                  13.format  --Format the system                           *" << endl;
	cout << "*                  14.help                                                  *" << endl;
	cout << "*****************************************************************************" << endl;
}

int main()
{
	copyright();
	help();
	string s;
	char arg1[NAMESIZE] = "";
	char arg2[NAMESIZE] = "";
	memset(root, '\0', sizeof(struct storage));
	init();
	while (1)
	{
		cin >> s;
		if (s == "createDir")
		{
			cin >> arg1;
			create_dir(PATH, arg1);
			ls(PATH);
		}
		else if (s == "format")
		{
			memset(root, '\0', sizeof(struct storage));
			init();
		}
		else if (s == "createFile")
		{
			int size_kb;
			cin >> arg1 >> size_kb;
			createFile(PATH, arg1, size_kb); 
		}
		else if (s == "deleteFile"){
			cin >> arg1;
			rm_file(PATH, arg1);
		}
		else if (s == "deleteDir")
		{
			cin >> arg1;
			rm_dir(PATH, arg1);
		}
		else if (s == "cat")
		{
			cin >> arg1;
			cat(PATH, arg1);
		}
		else if (s == "cd")
		{
			cin >> arg1;
			cd(arg1);
		}
		else if (s == "pwd")
		{
			cout << PATH << endl;
		}
		else if (s == "dir")
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
		else if (s == "cp") 
		{
			cin >> arg1 >> arg2;
			cp(PATH, arg1, arg2);
		}
		else if (s == "sum") 
		{
			sum();
		}
		else if (s == "help" )
		{
			help();	
		}
		else if(s == "copyright" )
		{
			copyright();
		}
		else if (s == "exit")
		{
			break;
		}
		else{
			cout << "order is no exit! Please try again!" << endl;
		}	
	}
	return 0;
}
