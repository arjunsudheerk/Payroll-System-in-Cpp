#include <iostream>
#include <cstdio>
#include <cstring>

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

int checkID(int id)
{
    FILE *fp;
    int found = 0;
    fp = fopen(EMP_FILE,"r");
    while ((fread (&e, sizeof(e), 1, fp)) == 1)
    {
		if(e.empid == id)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
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

class Employee
{
	private:
		int empID;
		char empName[100];
		int desigcode;
		char desig[20];
		Date DoJ;
		char empAddress[100];
		char empMail[50];
		char empContact[10];
		char bankAccount[16];
		float salary,basicPay,otherAllowance;
		
	public:
		void addEmp();
		void deleteEmp();
		void modifyEmp();
		void displayEmp();
		void searchEmp(int);
}e;

void Employee::addEmp()
{
	char choice='y';
	char save;
	int check, status;
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
				cin>>empID;
                check=checkID(e.empid); //checking if user already exists
                if(check)
                {
        	        cout<<"\n\t\t\tEmployee ID already exists!";
        	        system("clear");
                }
			}while(check);
			
			scanf("%c", (char *) stdin);
			do
			{
				cout<<"\n\n\t\tEnter Full Name of Employee: ";
				fgets(e.empname,100,stdin);
				status = isNameValid(e.empname);
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
				cin>>e.desigCode);
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
			
			scanf("%c", (char *) stdin);
			cout<<"\n\n\t\tEnter Employee Address : ";
			fgets(e.empAddress,100,stdin);
			//gets(e.empAddress);
						
			cout<<"\n\n\t\tEnter valid email ID : ");
			cin>>e.empMail;
			//gets(e.empMail);
			do
			{
				cout<<"\n\n\t\tEnter Contact number : ";
				cin>>e.empContact;
                check=strlem(e.empContact); //checking phone number validity
                if(check!=10)
                {
	                 cout<<"\n\t\t\tinvalid contact number ";
	                //MessageBox(0,"Invalid Contact number", "ERROR!" , MB_OK );
                }
			}while(check!=10);

			do
			{
				cout<<"\n\n\t\tEnter Bank account number: ";
				cin>>e.bankAccount);
				check=strlen(e.bankAccount);
				if(check!=16)
				{
					cout<<"\n\t\t\tInvalid bank account number ";
				}
			}while(check!=16);
			
			cout<<"\nDo you want to save this employee : ";
			cin>>save;
			if(save=='y')
			{
				fwrite(&e,sizeof(e),1,fp);
				cout<<"\nAdded employee";
			}
			cout<<"\nDo you want to add more employees : ";
			cin>>choice;
		}
	}	
	fclose(fp);	
}

void Employee::searchEmp(int id)
{
	fp = fopen(EMP_FILE,"r+");
	if(fp == NULL)
	{
		cout<<"Error opening file";
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
			cout<<"\n\n------------------------------------------------------------------------------------");
			cout<<"\n\t\tSearched Employee found";
			cout<<"\n\n------------------------------------------------------------------------------------");
			cout<<"\n\n\t\tEMPLOYEE ID     : "<<e.empID;
			cout<<"\n\n\t\tNAME            : "<<e.empName;
			cout<<"\n\n\t\tDESIGNATION     : "<<e.desig;
			cout<<"\n\n\t\tDATE OF JOINING : "<<e.DoJ.dd<<"/"<<e.DoJ.mm<<"/"<<e.DoJ.yyyy;
			cout<<"\n\n\t\tADDRESS         : "<<e.empAddress;
			cout<<"\n\n\t\tEMAIL ID        : "<<e.empMail);
			cout<<"\n\n\t\tCONTACT NO.     : "<<e.empContact);
			cout<<"\n\n\t\tBANK ACCOUNT NO : "<<e.bankAccount);
            cout<<"\n\n------------------------------------------------------------------------------------";
						
		}
		else
			cout<<"\n\nNot found";
		fclose(fp);
	}
	
}

void Employee::displayEmp()
{
	system("clear");
    cout<<"\n\t\t\tList  of Employees";
    int found = 0;
	fp = fopen(EMP_FILE,"r+");
	if(fp==NULL)
	{
		cout<<"Error opening file";
	}
	else
	{
		while((fread(&e,sizeof(e),1,fp))==1)
		{
			cout<<"\n\n------------------------------------------------------------------------------------");
			cout<<"\n\n\t\tEMPLOYEE ID     : "<<e.empID;
			cout<<"\n\n\t\tNAME            : "<<e.empName;
			cout<<"\n\n\t\tDESIGNATION     : "<<e.desig;
			cout<<"\n\n\t\tDATE OF JOINING : "<<e.DoJ.dd<<"/"<<e.DoJ.mm<<"/"<<e.DoJ.yyyy;
			cout<<"\n\n\t\tADDRESS         : "<<e.empAddress;
			cout<<"\n\n\t\tEMAIL ID        : "<<e.empMail);
			cout<<"\n\n\t\tCONTACT NO.     : "<<e.empContact);
			cout<<"\n\n\t\tBANK ACCOUNT NO : "<<e.bankAccount);
            cout<<"\n\n------------------------------------------------------------------------------------";
            found=1;
        }
	}
	fclose(fp);
    if(!found)
    {
        cout<<"\n\t\t\tNo Record !!!";
    }
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
				case 0: cout<<"\n\nThank you";
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

void menu::login()
{
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
       	amenu.showMenu();
    }
    else if((strcmp(usernm,empuser) == 0) && (strcmp(passwd,emppass) == 0))
    {
		emenu.showMenu();
	}
	else if((strcmp(usernm,accuser) == 0) && (strcmp(passwd,accpass) == 0))
	{
		cmenu.showMenu();
	}
    else
	{
		cout<<"\nWrong username and password";
	}
}

int main()
{
	menu m;
	m.welcome();
}
