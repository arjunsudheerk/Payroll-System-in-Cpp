#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <termios.h>
#include "get.h"

#define EMP_FILE "EmployeeData.txt"

using namespace std;

struct Date //structure for date
{
        int dd;
        int mm;
        int yyyy;
};

FILE *fp;

void gotoxy(int x,int y) //function to set position on screen
{
        printf("%c[%d;%df",0x1B,y,x);
}

int isNameValid(const char *name) //checking whether name has only alphabets
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

int isValidDate(struct Date *validDate) //checking whether date is valid
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

void press() //function for prompt
{
	char con;
	cout<<"\n\n\t\tEnter any key to continue..........";
	cin>>con;
	system("clear");
}

int checkpass(const char *pswd) //checking password conditions
{
	int a,up=0,low=0,digi=0,i;
	a=strlen(pswd);
	if(a<6)
	{
		cout<<"\n\n\t\t\t\t\tERROR: Password should contain minimum 6 characters ";
	}
	else if(a>10)
	{
		cout<<"\n\n\t\t\t\t\tERROR: Password shouldn't exceed 10 characters ";
	}
	else
	{
		
		for(i=0;pswd[i]!='\0';i++)
		{
			if(pswd[i]>='A' && pswd[i]<='Z')
				up++;
			if(pswd[i]>='a' && pswd[i]<='z')
				low++;
			if(pswd[i]>='0' && pswd[i]<='9')
				digi++;
		}
		if(up!=0 && low!=0 && digi!=0 )
			return 1;
		else
		{
			if(up==0)
				cout<<"\n\n\t\t\t\t\tThere must be at least one Uppercase";
			if(low==0)
				cout<<"\n\t\t\t\t\tThere must be at least one Lowercase";
			if(digi==0)
				cout<<"\n\t\t\t\t\tThere must be at least one Digit";
		}
	}
}

void getPassword(char *pass)
{
    int c=0;
    char buff[30]={0},ch;
    int len=0;
    while((ch=getch())!='\n')
    {
        if(ch==0x7f)    // use 0x08 in turboc (WINDOWS)
        {
            if(len==0)  continue;
            printf("\b \b"); len--; continue;
        }
        printf("%c",'*');
        pass[len]=ch;
        len++;
    }
    pass[len]='\0';

}

class Employee //employee class
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
		void changeCredentials();
		void addEmp();
		void deleteEmp();
		void modifyEmp();
		void displayEmp();
		void searchEmp(int);
		void calculateSalary();
		void printPayslip();
		void viewSalary(int);
}e;

int Employee::checkID(int id) //checking whether id already exists in file
{
    FILE *fp;
    int found = 0;
    fp = fopen(EMP_FILE,"r"); //opening file
    while ((fread (&e, sizeof(e), 1, fp)) == 1) //checking every id
    {
		if(e.empID == id)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found; //returns 1 if id exists
}

void Employee::addEmp() //function to add employee details
{
	char choice='y';
    char save;
    Employee e;
    int check, status;
    system("clear");
    gotoxy(50,5);
    cout<<"ADD EMPLOYEE";
    gotoxy(48,7);
    cout<<"----------------------";
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
		fseek(fp,0,SEEK_END); //cursor to end of last line
        while(choice=='y') 
        {
			do
            {
				cout<<"\n\n\t\tEnter Employee ID : ";
                cin>>e.empID;
                check=e.checkID(e.empID); //checking if id already exists
                if(check) //if already exists
                {
                        cout<<"\n\t\t\tEmployee ID already exists!";
                }
			}while(check);

            cin.ignore();
            do
            {
				cout<<"\n\n\t\tEnter Full Name of Employee: ";
                fgets(e.empName,100,stdin);
                status = isNameValid(e.empName); //checking if name is valid
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
                switch(e.desigCode) //adding designation and basic pay corresponding to code
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
                cout<<"\n\n\t\tEnter date of Joiniing (dd/mm/yyyy): ";
                scanf("%d/%d/%d",&e.DoJ.dd,&e.DoJ.mm,&e.DoJ.yyyy);
				status = isValidDate(&e.DoJ); //check date validity
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
                check=strlen(e.empContact); 
                if(check!=10) //checking number of digits in phone no.
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
                if(check!=16) //checking number of digits in account no.
                {
					cout<<"\n\t\t\tInvalid bank account number ";
				}
			}while(check!=16);
			
			cout<<"\n\tDo you want to save this employee (y/n) "; //prompting user
            cin>>save;
            if(save=='y')
            {
				//setting default username and password
				cout<<"\n\t\t\tThe default USERNAME for you is set up as your EMPLOYEE ID";
                sprintf(e.empUser,"%d",e.empID);
                cout<<"\n\n\t\t\tThe default PASSWORD  for you is set up as 'newemp'!\n\t\t\t\tplease change it after login ";
                strcpy(e.empPass,"newemp");
                fwrite(&e,sizeof(e),1,fp); //saving to the file
                cout<<"\nAdded employee";
			}
			cout<<"\n\tDo you want to add more employees (y/n) "; //prompting the user
			cin>>choice;
		}
	}
    fclose(fp); //closing the pointer
}

void Employee::deleteEmp() //function to delete an employee
{
	Employee e;
	system("clear");
	gotoxy(50,5);
    cout<<"DELETE EMPLOYEE";
    gotoxy(48,7);
    cout<<"----------------------";
	int found=0, delId=0;
	char choice;
	FILE *fp=NULL;
	FILE *tmpfp=NULL;
	if((fp=fopen(EMP_FILE,"r+"))==NULL)
	{
		if((fp=fopen(EMP_FILE,"w+"))==NULL)
		{
			cout<<"\n\n\t\tError opening file";
			exit(0);
		}
	}
	tmpfp=fopen("temp.txt","w+"); //creating a temp file
	if(tmpfp==NULL)
	{
		fclose(fp);
		cout<<"\n\n\t\tError opening file";
		exit(0);
	}
	rewind(fp);
	cout<<"\n\n\t\tEnter ID of employee to delete : "; //prompt
	cin>>delId;
	while (fread (&e, sizeof(e), 1, fp)) //checking for id
    {
		if(e.empID == delId)
        {
			found = 1;
			break;
		}
	}
	if(found==1) //if id exists
	{
		cout<<"\n\n------------------------------------------------------------------------------------";
		cout<<"\n\n------------------------------------------------------------------------------------";
		cout<<"\n\n\t\tEMPLOYEE ID     : "<<e.empID;
		cout<<"\n\n\t\tNAME            : "<<e.empName;
		cout<<"\n\n\t\tDESIGNATION     : "<<e.desig;
		cout<<"\n\n\t\tDATE OF JOINING : "<<e.DoJ.dd<<"/"<<e.DoJ.mm<<"/"<<e.DoJ.yyyy;
		cout<<"\n\n------------------------------------------------------------------------------------";
         
		cout<<"\n\n\t\tDo you want to delete this employee? (y/n) : "; //prompting the user
		cin>>choice;
		
		if(choice == 'y')
		{
			rewind(fp);
			while(fread(&e,sizeof(e),1,fp)==1)
			{
				 //copying all details of employee to temp file except entered id
				if(e.empID != delId)
				{
					fwrite(&e,sizeof(e),1,tmpfp);
				}
				else //if entered id exists
				{
					found=1;
				}
			}
			if(found==1)
			{
				cout<<"\n\n\t\tRecord deleted successfully";
			}
			fclose(fp);
			fclose(tmpfp);
			remove(EMP_FILE); //removing old file
			rename("temp.txt",EMP_FILE); //renaming temp file
			press();
		}
		else //if user says no
		{
			cout<<"\n\n\t\tOperation terminated";
			press();
		}
	}
	else //if entered not found
	{
		cout<<"\n\n\t\tRecord not found";
		press();
	}
}

void Employee::modifyEmp() //function to modify details of an employee
{
	Employee e;
	system("clear");
	gotoxy(50,5);
    cout<<"MODIFY EMPLOYEE";
    gotoxy(48,7);
    cout<<"----------------------";
	int found=0, modId=0, check=0, status=0;
	char choice, save;
	FILE *fp=NULL;
	if((fp=fopen(EMP_FILE,"r+"))==NULL)
	{
		if((fp=fopen(EMP_FILE,"w+"))==NULL)
		{
			cout<<"\n\n\t\tError opening file";
			exit(0);
		}
	}
	
	rewind(fp);
	cout<<"\n\n\t\tEnter ID of employee to modify : "; //prompt
	cin>>modId;
	while (fread (&e, sizeof(e), 1, fp))
    {
		if(e.empID == modId) //searching for the entered id
        {
			found = 1;
            break;
        }
     }
     if(found==1) //if id found
     {
		 fseek(fp,-sizeof(e),SEEK_CUR); //moves the cursor back
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
		 
		 cout<<"\n\n\t\tDo you want to modify this employee? (y/n) : "; //prompt
         cin>>choice;
         
        if(choice == 'y')
        {
			cout<<"\n\nEnter New data for this employee";
			
			do
            {
				cout<<"\n\n\t\tEnter Employee ID : ";
                cin>>e.empID;
                if(e.empID != modId) //only same employee can be modified
                {
                        cout<<"\n\n\t\tCannot change Employee ID, enter the same";
                }
			}while(e.empID != modId);

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
				cout<<"\nEmployee record Modified";
				
				fwrite(&e,sizeof(e),1,fp);
                press();
			}
			 
		}
		else
		{
			cout<<"\n\n\t\tOperation terminated";
			press();
		}
	}
	else
	{
		cout<<"\n\n\t\tRecord not found";
		press();
	}
	fclose(fp);
}

void Employee::searchEmp(int id) //function to search an employee
{
	Employee e;
	int found=0;
	system("clear");
	gotoxy(50,5);
    cout<<"SEARCH AN EMPLOYEE";
    gotoxy(48,7);
    cout<<"----------------------";
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
		rewind(fp);
		while (fread (&e, sizeof(e), 1, fp))
		{
			if(e.empID == id) //searching for the id
			{
				found = 1;
				break;
			}
		}
		if(found==1) //if searched id is found
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

void Employee::displayEmp() //function to display all employees
{
	Employee e;
	FILE *fp;
	system("clear");
    gotoxy(50,5);
    cout<<"ALLL EMPLOYEES";
    gotoxy(48,7);
    cout<<"----------------------";
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
		while((fread(&e,sizeof(e),1,fp))==1) //traversing all the employees
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
	if(!found)
    {
        cout<<"\n\t\t\tNo Record !!!";
    }
    fclose(fp);
    press();
}

void Employee :: viewSalary(int id) //function to view salary details of an employee
{		
	Employee e;
	FILE *fp;
	system("clear");
	gotoxy(50,5);
    cout<<"SALARY DETAILS";
    gotoxy(48,7);
    cout<<"----------------------";
    int found;
	fp=fopen(EMP_FILE,"r+");
	if(fp==NULL)
	{
		cout<<"Error opening file";
	}
	else
	{
		rewind(fp);
		while(fread (&e, sizeof(e),1,fp))
		{
			if(e.empID==id) //getting the employee 
			{
				found=1;
				break;
			}
		}
		if(found==1)
		{
			if(e.salary==0) //if salary is not calculated
			{
				cout<<"\n\n\t\tSalary not generated!!\n\n";
			}
			else
			{
				
				cout<<"\n\n\t\t-------------------------------------------------------------------";
				cout<<"\n\n\t\t\t\tID              : "<<e.empID;
				cout<<"\n\n\t\t\t\tNAME            : "<<e.empName;
				cout<<"\n\n\t\t\t\tDESIGNATION     : "<<e.desig;
				cout<<"\n\n\t\t\t\tBASIC PAY       : "<<e.basicPay;
				cout<<"\n\n\t\t\t\tNET SALARY      : "<<e.salary;
				cout<<"\n\n\t\t-------------------------------------------------------------------";
			}
		}
		fclose(fp);
		press();
	}
}

void Employee::changeCredentials() //function to change username and password
{
	Employee e;
	FILE *fp;
	system("clear");
	int flag,check,i=3;
	char usernm[10],pass[10],newpass1[10],newpass2[10];
	
	gotoxy(50,5);
	cout<<"CHANGE CREDENTIALS";
	gotoxy(48,7);
    cout<<"------------------------";
	gotoxy(40,10);
	cout<<"Please login using default username and password ";//prompt
	gotoxy(40,12);
	cout<<"Enter your User Name\t : ";
	cin>>usernm;
	gotoxy(40,14);
	cout<<"Enter your password\t : ";
	cin.ignore();
	getPassword(pass);
	//cin>>pass;

	if((fp=fopen(EMP_FILE,"r+"))==NULL)
	{
		if((fp=fopen(EMP_FILE,"w+"))==NULL)
		{
			cout<<"\ncan't open file";
			exit(0);
		}
	}
	rewind(fp);
	while((fread(&e,sizeof(e),1,fp))==1) //fetching username and password
	{
		if(strcmp(pass,e.empPass) == 0 && strcmp(usernm,e.empUser) == 0)
		{
			flag=1;
			break;
		}
	}
	if(flag==1)//if found
	{
		fseek(fp,-sizeof(e),SEEK_CUR);
		system("clear");
		gotoxy(40,9);
		cout<<"---------------------------------------";
		gotoxy(50,10);
		cout<<"LOGGED IN ASS "<<e.empName;
		gotoxy(40,11);
		cout<<"---------------------------------------";
		gotoxy(41,14); //prompting for new username and password
		cout<<"Enter new username (max 10 charcters and no space): ";
		cin>>usernm;
		do
	    {
			cout<<"\n\t\t\t\t\t\tYou have "<<i<<" attempt(s) left!";
			cout<<"\n\n\t\t\t\t\tEnter new password : ";
			cin>>newpass1;
			//cin.ignore();
			//getPassword(newpass1);
			i--;
			check=checkpass(newpass1); //checking if password has all conditions
			if(check == 1)
				break;
	    }while(i > 0);	
        if(check == 1)
		{
			cout<<"\n\n\t\t\t\t\tConfirm password   : "; //prompting to confirm password
			cin>>newpass2;
			//getPassword(newpass2);
			//cin.ignore();
			check=strcmp(newpass1,newpass2); //comparing two passwords
			if(check==0) //if they are same
			{
				strcpy(e.empUser,usernm); //copy the new username to the employee object
				strcpy(e.empPass,newpass1); //copy the new password to the employee object
				fwrite(&e,sizeof(e),1,fp); //writing to the file
				system("clear");
				cout<<"\n\n\t\t----Username and Password change successfull----\n\n\n";
				press();
			}
            else //if second password is not same
			{
				cout<<"\n\n\t\t----Password Mismatch!!----\n\n\n";
				press();
				return;
			}
		}
		else //if attempts are finished
		{
			cout<<"\n\n\t\t----Credential change unsuccessfull----\n\n\n";
			press();
			return;
		}

	}
	else //if username and password doesnot match with file
	{
		gotoxy(50,30);
		cout<<"-----Wrong Username or Password------------";
		press();
		return;
	}

	fclose(fp);
}

class menu //base class for menu
{
	private:
		int choice;
		char usernm[10],passwd[10];
		char user[10]={"Admin"};
		char pass[10]={"Admin123"};
    public:
        void welcome();
		void login();
		void showMenu(); //function name common to all
};

void menu::welcome()
{
	Employee e; //object of class employee
	char option;
    system("clear"); //welcome screen
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
	cout<<"Enter your choice :\t"; //prompt
	cin>>option;
    switch(option)
    {
		case 'L':login(); //function of class menu
				break;
		case 'U':e.changeCredentials(); //functon of class employee
				break;
		case 'E':system("clear");
				gotoxy(50,10);
				cout<<"Thank you\n\n\n";
				exit(0);
				break;
		default: cout<<"\nWrong choice"<<endl;
	}
}

class adminMenu : public menu //derived class from menu
{
	private:
		int choice;
        int id;
    public:
        void showMenu() //function declared in base class
		{
			Employee e; //object of class employee
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
			cout<<"Enter Your Option :--> ";
			cin>>choice;
			switch(choice)
			{
				//calling functions of employee class
				case 0: system("clear");
						gotoxy(50,10);
						cout<<"Thank you\n\n\n";
						exit(0);
						break;
				case 1: system("clear");
						e.addEmp();
						break;
				case 2: system("clear");
						e.deleteEmp();
						break;
				case 3: system("clear");
						e.modifyEmp();
						break;		
				case 4: system("clear");
						e.displayEmp();
						break;
				case 5: system("clear");
						cout<<"\nEnter employee ID to search : "; //prompt
						cin>>id;
						e.searchEmp(id);
						break;
				case 6: welcome();
						break;
				default:cout<<"\n\n\t\tWrong choice";
			}
			showMenu();
		}
}amenu;

class employeeMenu : public menu //derived class from menu
{
	private:
		int choice;
		int loginId;
	public:
		//setter function to set employee id from emplogin function
		void setLoginid(int id)
		{
			loginId=id; //sets the id used to login
		}
		void showMenu()
		{
			Employee e;
			system("clear");
			gotoxy(50,5);
			cout<<"EMPLOYEE MENU"<<endl;
			gotoxy(50,8);
			cout<<"Welcome Employee "<<loginId<<endl;
			gotoxy(40,10);
			cout<<"1. View Profile"<<endl;
			gotoxy(40,12);
			cout<<"2. View Salary"<<endl;
			gotoxy(40,14);
			cout<<"3. Return to Login"<<endl;
			gotoxy(40,18);
			cout<<"0. EXIT"<<endl;
			gotoxy(40,20);
			cout<<"Enter Your Option :--> ";
			cin>>choice;
			switch(choice)
			{
				//calling functions of class employee
				case 0 :system("clear");
						gotoxy(50,10);
						cout<<"Thank you\n\n\n";
						exit(0);
						break;
				case 1 :e.searchEmp(loginId);
						break;
				case 2 :e.viewSalary(loginId);
						break;
				case 3 :welcome();
						break;
				default:cout<<"\n\n\tWrong choice";
			}
			showMenu();
		}
}emenu;

class accountantMenu : public menu //derived class from class menu
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
			cout<<"2. Print PaySlip"<<endl;
			gotoxy(40,14);
			cout<<"3. Back to Login"<<endl;
			gotoxy(40,18);
			cout<<"0. EXIT"<<endl;
			gotoxy(40,20);
			cout<<"Enter Your Option :-->  ";//prompt
			cin>>choice;
			switch(choice)
			{
				//calling functions of class employee
				case 0:system("clear");
						gotoxy(50,10);
						cout<<"Thank you\n\n\n";
						exit(0);
						break;
				case 1:e.calculateSalary();
						break;
				case 2:e.printPayslip();
						break;
				case 3:welcome();
						break;
				default:cout<<"\n\n\tWrong choice";
			}
			showMenu();
		}
}cmenu;

void Employee::empLogin(char *user, char *pass) //class employee function to login
{
	//this function is called if user is not admin
	Employee e;
	menu m; //object of menu class
	adminMenu amenu; //object of adminMenu class
	employeeMenu emenu; //object of employeeMenu class
	accountantMenu cmenu; //object of accountantMenu class
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
		//fetching username and password
		if(strcmp(user,e.empUser) == 0 && strcmp(pass,e.empPass) == 0)
		{
			flag=1;
			break;
		}
	}
	if(flag==1) //if found
	{
		gotoxy(50,5);
		cout<<"Logged in as "<<e.empName; //fetches the corresponding name of the id
		gotoxy(40,10);
		option=e.desigCode; //fetches and stores the corresponding designation code
		//designation code desides which menu to display
		switch(option)
		{	
			//if user is manager
			case 1: cout<<"\nYou have Admin Privileges!";
					amenu.showMenu();
					break;
			//if user is a normal employee
			case 2:
			case 3:
			case 4: emenu.setLoginid(e.empID);
					emenu.showMenu();
					break;
			//if user is an accountant
			case 5: cmenu.showMenu();
					break;
			default: cout<<"\nUNEXPECTED ERROR";
		}
	}
	else //if not found
	{
		system("clear");
		gotoxy(40,10);
		cout<<"Username or Password is incorrect";
		do
		{
			check=0;
			gotoxy(40,14);
			cout<<"Do you want to continue? ";
			gotoxy(40,16);
			cout<<"press L to return to Login \t\t\t press M to return to main menu ";
			cout<<"\n\n\t\t\t\t\tEnter your choice : ";
			cin>>choice;
			switch(choice)
			{
				//calling functions of menu class
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

void menu::login() //function of menu class to display login screen 
{
	Employee e;
    
	system("clear");
	gotoxy(50,5);
	cout<<"LOGIN TO YOUR PROFILE";
	gotoxy(48,7);
    cout<<"------------------------";
	gotoxy(40,10);
	cout<<"Enter your User Name\t: "; //prompt
	cin>>usernm;
	gotoxy(40,12);
	cout<<"Enter your Password\t : "; 
	//cin>>passwd;
	cin.ignore();
	getPassword(passwd); //accept password display stars
	//char *passwd=getpass("Enter your password\t: ");

	if( (strcmp(usernm,user) == 0) && (strcmp(passwd,pass) == 0) )
	{
		//if user is an admin
		amenu.showMenu();
	}
	else if( (strcmp(usernm,user) != 0 || strcmp(passwd,pass) != 0) )
	{
		//calls function login for employees in employee class
		e.empLogin(usernm,passwd);
	}
	else
	{
		cout<<"\n\n\t\tERROR!!!";
		exit(0);
	}
}

void Employee :: calculateSalary() //function to calculate the salary of an employee
{
	Employee e;
	FILE *fp;
	int empId;
	float hra,pf,da,other;
	system("clear");
	gotoxy(50,5);
	cout<<"CALCULATE SALARY";
	gotoxy(48,7);
    cout<<"---------------------";
	fp=fopen(EMP_FILE,"r+");
	if(fp==NULL)
	{
		cout<<"\nError opening file";
	}
	else
	{
		cout<<"\n\n\t\tEnter Employee ID : "; //prompt
		cin>>empId;
		int found;
		rewind(fp);
		while(fread (&e, sizeof(e),1,fp))
		{
			if(e.empID==empId) //fetching employee details
			{
				found=1;
				break;
			}
		}
		if(found==1)
		{
			if(e.salary != 0) //check if salary already generated
			{
				cout<<"\n\n\tSalary already generated";
			}
			else
			{
				system("clear");
				
				cout<<"\n-------------------------------------------------------------------------";
				cout<<"\n\n\t\tEMPLOYEE ID       : "<<e.empID;
				cout<<"\n\n\t\tNAME              : "<<e.empName;
				cout<<"\n\n\t\tDESIGNATION       : "<<e.desig;
				cout<<"\n\n\t\tEnter other allowance for this employee(%) : "; //prompt
				scanf("%f",&e.otherAllowance);
				pf=0.12*e.basicPay; //different allowance and deductions
				hra=0.1*e.basicPay;
				da=0.05*e.basicPay;
				other=e.basicPay*e.otherAllowance/100; 
				e.salary=e.basicPay+hra+da+other-pf; //calculating total salary
				fseek(fp,-sizeof(e),SEEK_CUR); //moving the cursor back
				fwrite(&e,sizeof(e),1,fp); //writing the salary and other allowance to file
				cout<<"\n-------------------------------------------------------------------------";
				cout<<"\n\n\t\tSalary for Employee for this month = "<<e.salary; //displaying salary
				
             }        
		}
		else //if id not found
		{
			cout<<"\n\n\t\t\tNo Record found";
		}
	}
        
	fclose(fp);
	press();
}

void Employee:: printPayslip() //function to print payslip of an employee
{
	Employee e;
	system("clear");
	FILE *fp;
	int found,empId;
	if((fp=fopen(EMP_FILE,"r+"))==NULL)
	{
		if((fp=fopen(EMP_FILE,"w+"))==NULL)
		{
			cout<<"\ncan't open file";
			exit(0);
		}
	}
	cout<<"\n \n\t\t please enter Employee ID : ";
	cin>>empId;
	
	while (fread (&e, sizeof(e), 1, fp))
    {
        if(e.empID == empId) //fetches id and details
        {
            found = 1;
            break;
        }
    }
    if(found==1)
    {
        system("clear");
        gotoxy(5,5);
        cout<<"\n\t--------------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(70,10);
        cout<<"----PAYSLIP----";
        gotoxy(10,12);
        cout<<"\tEMPLOYEE ID                : "<<e.empID;
        gotoxy(80,12);
        cout<<"\tNAME                       : "<<e.empName;
        gotoxy(10,13);
        cout<<"\tDoJ                        : "<<e.DoJ.dd<<"/"<<e.DoJ.mm<<"/"<<e.DoJ.yyyy;
        gotoxy(10,14);
        cout<<"\tBANK A/c NO                : "<<e.bankAccount;
        gotoxy(80,14);
        cout<<"\tDesignation                : "<<e.desig;

        gotoxy(10,15);
        cout<<"\tEmail id                   : "<<e.empMail;

        gotoxy(10,16);
        cout<<"\tContact No                 : "<<e.empContact;

        gotoxy(20,18);
        cout<<"\tEarnings ";
        gotoxy(90,18);
        cout<<"\tDeductions";
        gotoxy(10,19);
        cout<<"\tBASIC                     : Rs."<<(e.basicPay);
        gotoxy(80,19);
        cout<<"\tEPF                       : Rs."<<(e.basicPay*.12);
        gotoxy(10,20);
        cout<<"\tHRA                       : Rs."<<(e.basicPay*.1);

        gotoxy(10,21);
        cout<<"\tDA                        : Rs."<<(e.basicPay*0.5);
        gotoxy(10,22);
        cout<<"\tOther Allowances          : Rs."<<(e.basicPay*e.otherAllowance/100);
        gotoxy(10,25);
        cout<<"\tTotal Earnings            : Rs."<<(e.basicPay+.1*e.basicPay+(e.basicPay*e.otherAllowance/100)+e.basicPay*.5);
        gotoxy(80,25);
        cout<<"\tTotal Deductions          : Rs."<<.12*e.basicPay;
        cout<<"\n\t--------------------------------------------------------------------------------------------------------------------------------------------------";
        cout<<"\n\t--------------------------------------------------------------------------------------------------------------------------------------------------";
        gotoxy(50,28);
        cout<<"Net Salary - \tRs."<<e.salary;
        cout<<"\n\n\t--------------------------------------------------------------------------------------------------------------------------------------------------";
    }
	else
    {
        cout<<"\n\t\t\tNo Record found";
    }
	press();
    fclose(fp);
}

int main()
{
	menu m; //object of class menu
	m.welcome(); //calling welcome function of class menu
}
