// SECURE WALLET
// Secure_Wallet_source.c
// Author: Siraj Qazi
// Secure Wallet
// Dated: January 10th, 2018 10:01PM


// Definitions
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define cls system("cls")
#define s system
#define pause { puts("\n <PRESS ANY KEY>"); _getch(); }


// Headers
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Structure declaration
struct user
{
	char uname[100], name[100], age[30];
	char dob[50], address[150], email[100];
	char ans[200];
};

// Declaration of structure variable usr of type struct user
struct user usr;
struct user u[100];

// FILE Pointer fp declaration
FILE *fp;

char fileName[10];

// users counter
int i = 0, k = 0;

//Encryption key
size_t key;

// Function prototypes
void welcome(void);
void dataEntry(void);
void encrypt(int, char*, char*, char*, char*, char*, char*, char*);
void decrypt(int, char*, char*, char*, char*, char*, char*, char*);
void encryptOne(int, char*);
void decryptOne(int, char*);
void pr_s(char*);
void login(void);
void credits();
void showData(void);

// main() function
void main(void)
{
	welcome();
	_getch();

}

// welcome() definition
void welcome()
{
	cls;
	s("color 0a");
	printf("\n #######################################################################################\n");
	printf("\n                                   SECURE WALLET(TM)\n");
	printf("                              Copyright(C) X-Corporation\n");
	printf("                               2017 All rights reserved.\n");
	printf("\n #######################################################################################\n");
	printf("\n Welcome to the Secure Wallet - A Safer Way to preserve data!\n");
	printf("\n Please PRESS 1,2 or 3 to proceed:\n\n");
	printf(" 1. New user\n");
	printf(" 2. Login / Manage your account\n");
	printf(" 3. Exit\n ");
	char c = _getch();
	if (c == '1')
		dataEntry();
	else if (c == '2')
		login();
	else if (c == '3')
		exit(1);
	else
	{
		pr_s("\n Please press 1, 2 or 3");
		_getch();
		welcome();
	}
}



// dataEntry() definition
void dataEntry()
{
	int key2;
	char charKey[5];
	// Increment number of users
	++i;

entry:
	cls;
	printf("\n #######################################################################################\n");
	printf("\n                                   SECURE WALLET(TM)\n");
	printf("                               Copyright(C) X-Corporation\n");
	printf("                               2017 All rights reserved.\n");
	printf("                                   DATA ENTRY MENU\n");
	printf("\n #######################################################################################\n");
	s("color 0b");
	printf("\n (Enter M to return to Main Menu\n");

	// Receive user data into structure elements
	printf("\n Please specify a username: ");
	scanf(" %[^\n]s", u[i].uname);
	
	// If the user enters 'M' or 'm', go back to Main Menu
	if (strcmp(u[i].uname, "m") == 0 || strcmp(u[i].uname, "M") == 0)
		welcome();

	// Check whether username is already taken or not
	fp = fopen("data1.txt", "r");
	if (!(fp == NULL))
	{
		fclose(fp);
		fp = fopen("sys.txt", "r");
		fscanf(fp, "%d", &k);
		fclose(fp);

		//Open each user data file one by one, and compare the username in the file with the entered username.
		for (int j = 1; j <= k; ++j)
		{
			sprintf(fileName, "data%d.txt", j);
			fp = fopen(fileName, "r");
			if (fp == NULL)
				break;
			fscanf(fp, "%s %s %s %s %s %s %s %s", usr.uname, usr.name, usr.age, usr.dob, usr.address, usr.email, usr.ans, charKey);
			decryptOne(261, charKey);   // Decrypt the character encryption key
			key2 = atoi(charKey);       // Convert the decrypted character key to integer key
			decryptOne(key2, usr.uname);

			if (strcmp(usr.uname, u[i].uname) == 0)
			{
				printf("\n This Username is already taken.\n Please enter a new user name.");
				pause;
				goto entry;
			}
		}
	}

	printf("\n Please enter Name: ");
	scanf(" %[^\n]s", u[i].name);

	printf("\n Please enter Age(in years): ");
	scanf(" %[^\n]s", u[i].age);

	printf("\n Please enter Date of Birth: ");
	scanf(" %[^\n]s", u[i].dob);

	printf("\n Please enter Home Address: ");
	scanf(" %[^\n]s", u[i].address);

	printf("\n Please enter Email Address: ");
	scanf(" %[^\n]s", u[i].email);

	printf("\n Answer this recovery question(incase you forget your PIN): \n What was the color of your first car?\n ");
	scanf(" %[^\n]s", u[i].ans);

	srand(time((NULL)));   // Generate a random 4-Digit PIN
	key = rand() % 10000;
	if (key < 9000)
		key += 1000;

	sprintf(fileName, "data%d.txt", i);
	fp = fopen(fileName, "w");
	if (fp == NULL)
	{
		printf("\n ERROR CREATING RECORD!.\n File record with specified parameters couldn't be created.\n Code: 00x0F221\n");
		_getch();
		main();
	}

	// Encrypt all of the user's data using the 4-Digit PIN generated above
	encrypt(key, u[i].uname, u[i].name, u[i].age, u[i].dob, u[i].address, u[i].email, u[i].ans);
	
	// Copy the integer key into a string encKey[5]
	char encKey[5];
	sprintf(encKey, "%d", key);

	// Encrypt the key
	encryptOne(261, encKey);

	// Write all of the users encrypted data and the encrypted user key to a file
	fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", u[i].uname, u[i].name, u[i].age, u[i].dob, u[i].address, u[i].email, u[i].ans, encKey);
	fclose(fp);
	fp = fopen("sys.txt", "w");
	if (fp == NULL)
	{
		printf("\n ERROR WRITING USER RECORD!\n user record count couldn't be saved.\n Code: 00x0D773\n");
		_getch();
		main();
	}

	// Write the number of users to the file sys.txt
	fprintf(fp, "%d", i);
	fclose(fp);


	cls;
	printf("\n The Data has been entered successfully!");

	// Display the username and PIN for the user
	decryptOne(key, u[i].uname);  // Decrypt the username(To display on the screen)
	printf("\n Username:  %s\n PIN:\t    %u\n", u[i].uname, key);
	printf("\n Please make a note of this PIN. It'll be used to login.\n");

	// Catch user's key press, and allow the user to return back to Main Menu or Exit
	printf(" Press R to Return back to Main Menu, E to exit ");
	char x = _getch();
	while (x != 'R' && x != 'e' && x != 'r' && x != 'E')
	{
		printf("\n Please press R (Return to Menu) or E (Exit)");
		x = _getch();
	}
	if (x == 'r' || x == 'R')
	{
		fflush(stdin);
		main();
	}
	else
		exit(1);
}

void login()
{
	char ANS[200];     // Array to hold user's answer to the security question
	char usr_name[100], c = ' ';   // Array to hold the entered username
	char pin[5], encKey[5];   // pin[5] array to hold the entered PIN
	cls;
	printf("\n #######################################################################################\n");
	printf("\n                                   SECURE WALLET(TM)\n");
	printf("                               Copyright(C) X-Corporation\n");
	printf("                               2017 All rights reserved.\n");
	printf("                                     ACCOUNT LOGIN\n");
	printf("\n #######################################################################################\n");
	s("color 0e");
	fp = fopen("sys.txt", "r");
	if (fp == NULL)
	{
		printf("\n ERROR ACCESSING DATABASE!\n No records have been saved.\n Code: 00x0E312\n");
		_getch();
		main();
	}
	fscanf(fp, "%d", &i);
	fclose(fp);
	printf(" (Enter M for Main Menu)\n\n Enter User Name : ");
	scanf("%s", usr_name);

	// If the user enters 'M' or 'm', go back to Main Menu
	if (strcmp(usr_name, "m") == 0 || strcmp(usr_name, "M") == 0)
		welcome();

	// Receive the PIN as hidden input (asterisks), using getch() function
	printf("\n Enter your 4-Digit PIN : ");
	int l = 0;
	while (l <= 3)
	{
		pin[l] = _getch();    // For each iteration, store the character read by getch() in the pin[] array
		printf("*");
		l++;
	}
	pin[4] = '\0';   // Terminate the array with the null character, making it a string
	int enteredPin = atoi(pin);   // Convert the character array PIN into an integer
	for (int j = 1; j <= i; ++j)
	{
		sprintf(fileName, "data%d.txt", j);
		fp = fopen(fileName, "r");
		if (fp == NULL)
		{
			printf("\n ERROR ACCESSING DATABASE!\n No records have been saved.\n Code: 00x0E312\n");
			_getch();
			main();
		}
		char encKey[] = "";

		// Read all of user's data from the file
		fscanf(fp, "%s %s %s %s %s %s %s %s", usr.uname, usr.name, usr.age, usr.dob, usr.address, usr.email, usr.ans, encKey);
		
		// Decrypt the encryption key
		decryptOne(261, encKey);
		int encKeyInt = atoi(encKey);

		// Decrypt all of user's data using the decrypted integer key
		decrypt(encKeyInt, usr.uname, usr.name, usr.age, usr.dob, usr.address, usr.email, usr.ans);

		// Compare entered username with decrypted username from the file, and login if successful
		if (strcmp(usr_name, usr.uname) == 0 && (enteredPin == encKeyInt))
		{
			cls;
			s("color 0d");
			printf("\n        REDIRECTING... PLEASE WAIT\n");
			printf("\n\n\n Signing in.");
			Sleep(300);
			printf(" .");
			Sleep(600);
			printf(" .");
			Sleep(600);
			printf(" .");
			cls;
			printf("\n        REDIRECTING... PLEASE WAIT\n");
			printf("\n\n\n Retrieving information.");
			Sleep(300);
			printf(" .");
			Sleep(600);
			printf(" .");
			Sleep(600);
			printf(" .");
			Sleep(500);
			cls;
			printf("\n        REDIRECTING... PLEASE WAIT\n");
			printf("\n\n\n Loading database.");
			Sleep(500);
			printf(" .");
			Sleep(500);
			printf(" .");

			showData();

			exit(1);
		}

		// If username is correct but PIN is wrong, allow for PIN recovery using security question
		else if (strcmp(usr_name, usr.uname) == 0 && enteredPin != encKeyInt)
		{
			printf("\n Forgot your PIN?\n\n Answer the recovery question:\n");
			printf("\n What was the color of your first car? ");
			scanf(" %[^\n]s", ANS);
			if (strcmp(ANS, usr.ans) == 0)
			{
				printf("\n Your PIN is %d\n Please login again.\n", encKeyInt);
				printf("\n <PRESS ANY KEY>");
				_getch();
				login();
			}
			else
			{
				printf("\n Incorrect answer! This account has been temporarily blocked.\n Please try later.\n");
				pause;
				login();
			}
		}


		fclose(fp);
	}
	pr_s("\n Invalid Username or Password.\n");
	_getch();
	login();

	_getch();
	main();
}

// Show User Details Function
void showData()
{
	cls;
	s("color 0d");
	printf("\n #######################################################################################\n");
	printf("\n                                   SECURE WALLET(TM)\n");
	printf("                               Copyright(C) X-Corporation\n");
	printf("                               2017 All rights reserved.\n");
	printf("                                   VIEW INFORMATION\n");
	printf("\n #######################################################################################\n");
	printf(" Information you previously entered:\n\n");
	printf(" 1. Username:\t\t @%s\n 2. Name:\t\t %s\n 3. Age:\t\t %s\n", usr.uname, usr.name, usr.age);
	printf(" 4. Date of Birth:\t %s\n 5. Address:\t\t %s\n 6. Email:\t\t %s\n", usr.dob, usr.address, usr.email);
erase:
	pr_s("\n Press R to return back to Main Menu, E to Exit ");
	char x = _getch();
	while (x != 'R' && x != 'e' && x != 'r' && x != 'E')
	{
		printf("\n Please press R (Return to Menu) or E (Exit)");
		x = _getch();
	}
	if (x == 'r' || x == 'R')
	{
		fflush(stdin);
		main();
	}
	else
		exit(1);
}


//  encrypt() function definition
void encrypt(int key, char *a, char *b, char *c, char *d, char *e, char *f, char* g)
{

	// Offset each character using the integer passed as the key argument
	for (; *a != '\0'; ++a)
		*a = *a + key;
	for (; *b != '\0'; ++b)
		*b = *b + key;
	for (; *c != '\0'; ++c)
		*c = *c + key;
	for (; *d != '\0'; ++d)
		*d = *d + key;
	for (; *e != '\0'; ++e)
		*e = *e + key;
	for (; *f != '\0'; ++f)
		*f = *f + key;
	for (; *g != '\0'; ++g)
		*g = *g + key;
}

// decrypt() function definition
void decrypt(int key, char *a, char *b, char *c, char *d, char *e, char *f, char* g)
{
	// De-Offset each character using the integer passed as the key argument
	for (; *a != '\0'; ++a)
		*a = *a - key;
	for (; *b != '\0'; ++b)
		*b = *b - key;
	for (; *c != '\0'; ++c)
		*c = *c - key;
	for (; *d != '\0'; ++d)
		*d = *d - key;
	for (; *e != '\0'; ++e)
		*e = *e - key;
	for (; *f != '\0'; ++f)
		*f = *f - key;
	for (; *g != '\0'; ++g)
		*g = *g - key;
}

void encryptOne(int key, char *a)
{
	// Keep offsetting characters of a string onto the screen until \0 is encountered
	for (; *a != '\0'; ++a)
		*a = *a + key;
}
void decryptOne(int key, char *b)
{
	// Keep de-offsetting characters of a string onto the screen until \0 is encountered
	for (; *b != '\0'; ++b)
		*b = *b - key;
}

void pr_s(char* a)
{
	// Keep printing characters of a string onto the screen until \0 is encountered
	for (; *a != '\0'; ++a)
		printf("%c", *a);
}


