#include<iostream>
#include<cstring>

using namespace std;

void gotoxy(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}
class menu
{
	private:
		int choice;
		char usernm[10],passwd[10];
		char user[10]={"Admin"};
		char pass[10]={"Admin123"};
		char empuser[10]={"Employee"};
		char emppass[10]={"Emp123"};
		char accuser[10]={"Account"};
		char accpass[10]={"Acc123"};
	public:
		void welcome();
		void login();
		void adminMenu();
		void employeeMenu();
		void accountantMenu();
};

void menu::welcome()
{
	char option;
	gotoxy(30,5);
	cout<<"____________________________________________________________________"<<endl;
	gotoxy(30,6);		
	cout<<"|                                                                  |"<<endl;
	gotoxy(30,7);		
	cout<<"|             WELCOME TO EXCALIBUR ENTERPRISES PVT LTD             |"<<endl;//13 spaces		
	gotoxy(30,8);		
	cout<<"|__________________________________________________________________|"<<endl;		
	gotoxy(35,10);		
	cout<<"Enter"<<endl;
	gotoxy(40,12);                
	cout<<"L for Login"<<endl;
	gotoxy(40,14);
	cout<<"U to update credentials"<<endl;
	gotoxy(40,16);
	cout<<"E for exit "<<endl;
	gotoxy(35,18);
    cout<<"Enter your choice : "<<endl;	
    cin>>option;
    switch(option)
    {
		case 'L':login(); break;
		case 'E':exit(0); break;
		default: cout<<"\nWrong choice"<<endl;
	}
}

void menu::login()
{
	system("clear");
    gotoxy(40,10);
    cout<<"Enter your User Name\t: ";
    //cin.ignore();
    //gets(usernm);
	cin>>usernm;
	gotoxy(40,12);
    cout<<"Enter your password\t: ";
    //cin.ignore();
    //gets(passwd);
	cin>>passwd;
	if( (strcmp(usernm,user) == 0) && (strcmp(passwd,pass) == 0) )
    {
       	adminMenu();
    }
    else if((strcmp(usernm,empuser) == 0) && (strcmp(passwd,emppass) == 0))
    {
		employeeMenu();
	}
	else if((strcmp(usernm,accuser) == 0) && (strcmp(passwd,accpass) == 0))
	{
		accountantMenu();
	}
    else
	{
		cout<<"\nWrong username and password";
	}
}

void menu::adminMenu()
{
	system("clear");     
	gotoxy(50,5);
    cout<<"ADMIN MENU "<<endl;
    gotoxy(40,10);
    cout<<"1. Add Employee"<<endl;
    gotoxy(40,12);
    cout<<"2. Delete Employee"<<endl;
    gotoxy(40,14);
    cout<<"3. Modify Employee"<<endl;
    gotoxy(40,16);
    cout<<"4. Display Employee List"<<endl;
    gotoxy(40,18);
    cout<<"5. Search Record"<<endl;
    gotoxy(40,20);
    cout<<"6. Back to Login"<<endl;
    gotoxy(40,22);
    cout<<"0. EXIT"<<endl;
    gotoxy(40,26);
    cout<<"Enter Your Option :--> "<<endl;
    cin>>choice;
}

void menu::employeeMenu()
{
	system("clear");
	gotoxy(50,5);
	cout<<"EMPLOYEE MENU"<<endl;
	gotoxy(50,8);
	cout<<"Welcome Employee"<<endl;
	gotoxy(40,10);
    cout<<"1. View Profile"<<endl;
    gotoxy(40,12);
    cout<<"2. View Salary"<<endl;
    gotoxy(40,14);
    cout<<"3. Return to Login"<<endl;
    gotoxy(40,18);
    cout<<"0. EXIT"<<endl;
    gotoxy(40,20);
    cout<<"Enter Your Option :--> "<<endl;
    cin>>choice;
}

void menu::accountantMenu()
{
	system("clear");
    gotoxy(50,5);
	cout<<"ACCOUNTANT MENU"<<endl;
	gotoxy(40,10);
	cout<<"1. Calculate Salary"<<endl;
	gotoxy(40,12);
	cout<<"2. Print Salary"<<endl;
	gotoxy(40,14);
	cout<<"3. Back to Login"<<endl;
	gotoxy(40,18);
    cout<<"0. EXIT"<<endl;
    gotoxy(40,20);
    cout<<"Enter Your Option :--> "<<endl;
    cin>>choice;
}

int main()
{
	menu m;
	m.welcome();

}
