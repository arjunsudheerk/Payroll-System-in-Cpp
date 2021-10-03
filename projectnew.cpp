#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <unistd.h>

#define EMP_FILE "EmployeeData.txt"

using namespace std;

struct Date
{
	int dd;
	int mm;
	int yyyy;
};

FILE *fp;

void gotoxy(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}

int isNameValid(const char *name)
{
    int validName = 1;
    int len = 0;
    int index = 0;
    len = strlen(name);
    for(index =0; index <len ; ++index)
    {
        if(!(isalpha(name[index])) && (name[index] != '\n') && (name[index] != ' '))
        {
            validName = 0;
            break;
        }
    }
    return validName;
}

int isValidDate(struct Date *validDate)
{
    //check range of year,month and day
    if (validDate->yyyy > 2022 || validDate->yyyy < 1900)
        return 0;
    if (validDate->mm < 1 || validDate->mm > 12)
        return 0;
    if (validDate->dd < 1 || validDate->dd > 31)
        return 0;
    //Handle feb days in leap year
    if (validDate->mm == 2)
    {
        if((validDate->yyyy %4 == 0) && (validDate->yyyy %100 != 0) || (validDate->yyyy %400 == 0))
            return (validDate->dd <= 29);
        else
            return (validDate->dd <= 28);
    }
    //handle months which has only 30 days
    if (validDate->mm == 4 || validDate->mm == 6 || validDate->mm == 9 || validDate->mm == 11)
        return (validDate->dd <= 30);
    return 1;
}

void press()
{
	int con;
	cout<<"\n\n\t\tEnter 1 to continue..........";
	cin>>con;
	system("clear");	
}

class Employee
{
	private:
		int empID;
		char empName[100];
		int desigCode;
		char desig[20];
		Date DoJ;
		char empAddress[100];
		char empMail[50];
		char empContact[11];
		char bankAccount[17];
		float salary,basicPay,otherAllowance;
		char empUser[10];
		char empPass[10];
		
	public:
		int checkID(int);
		void empLogin(char *,char *);
		void addEmp();
		void deleteEmp();
		void modifyEmp();
		void displayEmp();
		void searchEmp(int);

}e;

int Employee::checkID(int id)
{
    FILE *fp;
    int found = 0;
    fp = fopen(EMP_FILE,"r");
    while ((fread (&e, sizeof(e), 1, fp)) == 1)
    {
		if(e.empID == id)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

void Employee::addEmp()
{
	char choice='y';
	char save;
	Employee e;
	int check, status;
	system("clear");
	fp=fopen(EMP_FILE,"r+");
	if(fp == NULL)
    {
    	if((fp=fopen(EMP_FILE,"w+"))==NULL)
        {
    	    cout<<"\ncan't open file";
            exit(0);
        }
    }
	else
	{
		fseek(fp,0,SEEK_END);
		
		while(choice=='y')
		{
			
			do
			{
				cout<<"\n\n\t\tEnter Employee ID : ";
				cin>>e.empID;
                check=e.checkID(e.empID); //checking if user already exists
                if(check)
                {
        	        cout<<"\n\t\t\tEmployee ID already exists!";
        	        system("clear");
                }
			}while(check);
			
			cin.ignore();
			do
			{
				cout<<"\n\n\t\tEnter Full Name of Employee: ";
				fgets(e.empName,100,stdin);
				status = isNameValid(e.empName);
				if (!status)
				{
            		cout<<"\n\t\t\tName contain invalid character. Please enter again.";
            	}
            } while(!status);
            
            do
			{
				status=0;
				cout<<"\n\n\t1. Manager 2. Senior Executive 3. Junior Executive 4. Trainee 5. Accountant ";
				cout<<"\n\t\tEnter Designation code: ";
				cin>>e.desigCode;
				switch(e.desigCode) //adding designation and basic pay corresponding to id
                {
					case 1: strcpy(e.desig,"Manager");
                            e.basicPay=30000;
                            break;
                    case 2: strcpy(e.desig,"Senior Executive");
							e.basicPay=25000;
                    		break;
                    case 3: strcpy(e.desig,"Junior Executive");
							e.basicPay=20000;
							break;
                    case 4: strcpy(e.desig,"Trainee");
                            e.basicPay=15000;
                            break;
            		case 5: strcpy(e.desig,"Accountant");
                    		e.basicPay=20000;
                    		break;
                   default: cout<<"\n\t\t\tinvalid choice ";
                     		status=1;
                }
            }while(status);
            
			do
			{
				//get date year,month and day from user
				cout<<"\n\n\t\tEnter date of Joiniing (dd/mm/yyyy): ";
				scanf("%d/%d/%d",&e.DoJ.dd,&e.DoJ.mm,&e.DoJ.yyyy);
				//check date validity
				status = isValidDate(&e.DoJ);
				if (!status)
				{
					cout<<"\n\t\t\tPlease enter a valid date.";
				}
			}while(!status);
			
			cin.ignore();
			cout<<"\n\n\t\tEnter Employee Address : ";
			fgets(e.empAddress,100,stdin);
			//gets(e.empAddress);
						
			cout<<"\n\n\t\tEnter valid email ID : ";
			cin>>e.empMail;
			//gets(e.empMail);
			
			do
			{
				cout<<"\n\n\t\tEnter Contact number : ";
				cin>>e.empContact;
				cin.ignore();
                check=strlen(e.empContact); //checking phone number validity
                if(check!=10)
                {
	                 cout<<"\n\t\t\tinvalid contact number ";
	                //MessageBox(0,"Invalid Contact number", "ERROR!" , MB_OK );
                }
			}while(check!=10);
			
			do
			{
				cout<<"\n\n\t\tEnter Bank account number: ";
				cin>>e.bankAccount;
				cin.ignore();
				check=strlen(e.bankAccount);
				if(check!=16)
				{
					cout<<"\n\t\t\tInvalid bank account number ";
				}
			}while(check!=16);
			
			cout<<"\n\tDo you want to save this employee (y/n) ";
			cin>>save;
			if(save=='y')
			{
				cout<<"\n\t\t\tThe default USERNAME for you is set up as your EMPLOYEE ID";
				sprintf(e.empUser,"%d",e.empID);
				cout<<"\n\n\t\t\tThe default PASSWORD  for you is set up as 'newemp'!\n\t\t\t\tplease change it after login ";
				strcpy(e.empPass,"newemp");
				fwrite(&e,sizeof(e),1,fp);
				cout<<"\nAdded employee";
			}
			cout<<"\n\tDo you want to add more employees (y/n) ";
			cin>>choice;
		}
	}	
	fclose(fp);	
}

void Employee::searchEmp(int id)
{
	Employee e;
	system("clear");
	fp = fopen(EMP_FILE,"r+");
	if(fp == NULL)
    {
    	if((fp=fopen(EMP_FILE,"w+"))==NULL)
        {
    	    cout<<"\ncan't open file";
            exit(0);
        }
    }
	else
	{
		int found;
		rewind(fp);
		while (fread (&e, sizeof(e), 1, fp))
		{
			if(e.empID == id)
			{
				found = 1;
				break;
			}
		}
		if(found)
		{
			cout<<"\n\n------------------------------------------------------------------------------------";
			cout<<"\n\t\tSearched Employee found";
			cout<<"\n\n------------------------------------------------------------------------------------";
			cout<<"\n\n\t\tEMPLOYEE ID     : "<<e.empID;
			cout<<"\n\n\t\tNAME            : "<<e.empName;
			cout<<"\n\n\t\tDESIGNATION     : "<<e.desig;
			cout<<"\n\n\t\tDATE OF JOINING : "<<e.DoJ.dd<<"/"<<e.DoJ.mm<<"/"<<e.DoJ.yyyy;
			cout<<"\n\n\t\tADDRESS         : "<<e.empAddress;
			cout<<"\n\n\t\tEMAIL ID        : "<<e.empMail;
			cout<<"\n\n\t\tCONTACT NO.     : "<<e.empContact;
			cout<<"\n\n\t\tBANK ACCOUNT NO : "<<e.bankAccount;
            cout<<"\n\n------------------------------------------------------------------------------------";
						
		}
		else
			cout<<"\n\nNot found";
		fclose(fp);
	}
	press();
}

void Employee::displayEmp()
{
	Employee e;
	system("clear");
    cout<<"\n\t\t\tList  of Employees";
    int found = 0;
	fp = fopen(EMP_FILE,"r+");
	if(fp == NULL)
    {
    	if((fp=fopen(EMP_FILE,"w+"))==NULL)
        {
    	    cout<<"\ncan't open file";
            exit(0);
        }
    }
	else
	{
		while((fread(&e,sizeof(e),1,fp))==1)
		{
			cout<<"\n\n------------------------------------------------------------------------------------";
			cout<<"\n\n\t\tEMPLOYEE ID     : "<<e.empID;
			cout<<"\n\n\t\tNAME            : "<<e.empName;
			cout<<"\n\n\t\tDESIGNATION     : "<<e.desig;
			cout<<"\n\n\t\tDATE OF JOINING : "<<e.DoJ.dd<<"/"<<e.DoJ.mm<<"/"<<e.DoJ.yyyy;
			cout<<"\n\n\t\tADDRESS         : "<<e.empAddress;
			cout<<"\n\n\t\tEMAIL ID        : "<<e.empMail;
			cout<<"\n\n\t\tCONTACT NO.     : "<<e.empContact;
			cout<<"\n\n\t\tBANK ACCOUNT NO : "<<e.bankAccount;
            cout<<"\n\n------------------------------------------------------------------------------------";
            found=1;
        }
	}
	fclose(fp);
    if(!found)
    {
        cout<<"\n\t\t\tNo Record !!!";
    }
    press();
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
		void showMenu();
};

void menu::welcome()
{
	char option;
	system("clear");
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
    cout<<"Enter your choice : \t\t";	
    cin>>option;
    switch(option)
    {
		case 'L':	login();
					break;
		case 'E': 	system("clear");
					gotoxy(50,10);
					cout<<"Thank you";
					exit(0);
					break;
		default: cout<<"\nWrong choice"<<endl;
	}
}

class adminMenu : public menu
{
	private:
		int choice;
		int id;
		Employee e;
	public:
		void showMenu()
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
			switch(choice)
			{
				case 0: system("clear");
						gotoxy(50,10);
						cout<<"Thank you\n\n\n";
						exit(0);
						break;
				case 1: system("clear");
						e.addEmp();
						break;
				case 4: system("clear");
						e.displayEmp();
						break;
				case 5: system("clear");
						cout<<"\nEnter employee ID to search : ";
						cin>>id;
						e.searchEmp(id);
						break;
				case 6: welcome();
						break;
				default:cout<<"\n\n\t\tWrong choice";
						return;
			}
			showMenu();
		}
}amenu;

class employeeMenu : public menu
{
	private:
		int choice;
	public:
		void showMenu()
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
}emenu;

class accountantMenu : public menu
{
	private:
		int choice;
	public:
		void showMenu()
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
}cmenu;

void Employee::empLogin(char *user, char *pass)
{
	Employee e;
	menu m;
	adminMenu amenu;
	employeeMenu emenu;
	accountantMenu cmenu;
	int flag=0,check=0, option;
	char choice;
	if((fp=fopen(EMP_FILE,"r+"))==NULL)
        {
            if((fp=fopen(EMP_FILE,"w+"))==NULL)
            {
            	cout<<"\ncan't open file";
                exit(0);
            }
        }
		rewind(fp);
        while((fread(&e,sizeof(e),1,fp))==1)
        {
			if(strcmp(user,e.empUser) == 0 && strcmp(pass,e.empPass) == 0)
            {
				flag=1;
                break;
            }
        }
        if(flag)
        {
			gotoxy(50,5);
            cout<<"Logged in as "<<e.empName;
            gotoxy(40,10);
            option=e.desigCode;
            switch(option)
            {
				case 1: cout<<"\nYou have admin Privileges!";
						amenu.showMenu();
                    	break;
                case 2: 
                case 3:
                case 4: emenu.showMenu();
						break;
				case 5: cmenu.showMenu();
						break;
            	default: cout<<"\nUNEXPECTED ERROR";
            }
        }
		if(!flag)
        {
			system("clear");
            gotoxy(40,10);
            cout<<"\nUsername or Password is incorrect";
            do
            {
				check=0;
                gotoxy(40,14);
            	cout<<"Do you want to continue? ";
                gotoxy(40,16);
            	cout<<"press L to return to Login \t\t\t press M to return to main menu ";
            	cout<<"\n\n\tEnter your choice : ";
            	cin>>choice;
            	switch(choice)
            	{
					case 'L':m.login();
							break;
                	case 'M':m.welcome();
							break;
                	default:gotoxy(20,15);
                        	cout<<"\n\n\t\tWrong choice ";
                        	system("clear");
                        	check=1;
                }
            }while(!check);
        }
	
}

void menu::login()
{
	Employee e;
	
	system("clear");
	gotoxy(40,10);
    cout<<"Enter your User Name\t: ";
	cin>>usernm;
	gotoxy(40,12);
	cout<<"Enter your Password\t : ";
	cin>>passwd;
    //char *passwd=getpass("Enter your password\t: ");
    
	if( (strcmp(usernm,user) == 0) && (strcmp(passwd,pass) == 0) )
    {
       	amenu.showMenu();
    }
    else if( (strcmp(usernm,user) != 0 || strcmp(passwd,pass) != 0) )
    {
		e.empLogin(usernm,passwd);
	}
	else
	{
		cout<<"\n\n\t\tERROR!!!";
		exit(0);
	}
}

int main()
{
	menu m;
	m.welcome();
}
