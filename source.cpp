//Following are the header files used in our project
#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<string.h>
#include<fstream>

using namespace std;

#define total_alphabet (26)		//Alphabet size (# of symbols)
#define char_to_index(c) ((int)c - (int)'a')		//Macro definition to Find the index of a particular english letter

struct TrieNode
{
	struct TrieNode *children[total_alphabet];
	char meaning[50];		//To store meaning of the word
	bool isEOW;		//Non zero if word is completed
};

typedef TrieNode TrieNode;

TrieNode * newnode()	//// Returns new trie node (initialized to NULLs)
{
	int i=0;
	TrieNode *newnode = (TrieNode*)malloc(sizeof(TrieNode));
	newnode->isEOW = false;
	for(i = 0;i<total_alphabet;i++)
		newnode->children[i] = NULL;
	memset (newnode->meaning,'\0',50);
	return newnode;
}

struct history		//History node use to implement stack data structure for Recent history function
{
	char data[50];
	struct history *next;
}*top=NULL;

void insert(TrieNode* root)		//If not present, inserts key into trie If the key is prefix of trie node, just marks leaf node
{
	ofstream file;
	file.open("C:\\Users\\Achyut Tripathi\\Desktop\\dictionary.txt", ios::app);		//Opening file to Write Words and meanings		
	xy2:
	char key[50], ch, mean[50];
	memset (key,'\0',50);
	memset (mean,'\0',50);
	cout<<"\n\nEnter a word: ";
	gets(key);		//Getiing Word
	fflush(stdin);
	cout<<"\n\nEnter Meaning: ";		//Getting Meaning
	gets(mean);
	fflush(stdin);
	int i, j, k;
	j = strlen(key);
	TrieNode* temp = root;
	for(i = 0; i < j; i++)		//Traversing through trie node
	{
		k = char_to_index(key[i]);
		if(!temp->children[k])
			temp->children[k] = newnode();
		temp = temp->children[k];
	}
	strcpy(temp->meaning, mean);	
	temp->isEOW = true;		// mark last node as leaf
	{
		file.write(key,(strlen(key)));		//Writing word on file
		file<<"\n";
		file.write(mean,strlen(mean));		//Writing meaning on file
		file<<"\n";
		file<<"\n";
	}
	cout<<"\n\nDo you want to add more words?(y/n): ";
	cin >> ch;
	fflush(stdin);
	if(ch == 'y')
		goto xy2;
	file.close();
}

void delette_file(TrieNode* root, char * key)		//Deleting word and meaning from file
{

    char word[50];
    int temp = 0, temp1=0;
	fstream fil;
    ofstream o;
    o.open("C:\\Users\\Achyut Tripathi\\Desktop\\tempdictionary.txt",ios::app);
    fil.open("C:\\Users\\Achyut Tripathi\\Desktop\\dictionary.txt",ios::in);
    if(!fil)
    {
	    cout<<"File is not found";
	    exit(0);
	}
	else
	{
	    while(!fil.eof())
	    {
	        fil>>word;
			if(strcmp(key, word)==0)
	        {
	            fil>>word;
	            temp1=1;
	        }
	        else
	        {
				o.write(word,strlen(word));
	        	temp++;
	        	if(temp%2==0)
					o<<"\n\n";
				else
					o<<"\n";
	        }
	    }
	    if(temp1==0)
	    	cout<<"\n\nWord NOT Found !!!!";
	}
	o.close();
	fil.close();
	remove("C:\\Users\\Achyut Tripathi\\Desktop\\dictionary.txt");
	rename("C:\\Users\\Achyut Tripathi\\Desktop\\tempdictionary.txt","C:\\Users\\Achyut Tripathi\\Desktop\\dictionary.txt");
}
void delette_tree(TrieNode* root, char * key)		//Deleting word an meaning from trie tree
{	
	int i, j, k;
	j = strlen(key);
	TrieNode* temp1 = root;
	for(i = 0; i < j; i++)
	{
		k = char_to_index(key[i]);
		if(!temp1->children[k])
		{
			cout<<"\n\nWord NOT Found !!!!";
			break;
		}
		temp1 = temp1->children[k];
	}
	if(temp1 != NULL && temp1->isEOW)
	{
		temp1->isEOW = false;
	}
	else if(temp1==NULL)
    {
        free(temp1);
    }
}

void push(char * key)		//Pushing Recent Searched Item in Stack 
{

	struct history *newhistory;
	newhistory=(struct history*)malloc(sizeof(struct history));
	strcpy(newhistory->data,key);
	if(top==NULL)
	{
		newhistory->next=NULL;
	}
	else
		newhistory->next=top;
	top=newhistory;
}

void search(TrieNode* root)			//Function to search a word in the file and trie tree 
{
    ifstream file;
	file.open("C:\\Users\\Achyut Tripathi\\Desktop\\dictionary.txt", ios::in);
	xy3:
	char key[50], ch,word[50],mean[50];
	int temp=0;
	cout<<"\n\nEnter Word to Search: ";
	gets(key);
	fflush(stdin);
	push(key);
	while(!file.eof())
    {
        file.getline(word,50);
        if(strcmp(key,word)==0)
        {
            temp=1;
            break;

        }

    }
    file.getline(mean,50);
    cout<<"\n\nMeaning: ";
    puts(mean);
    if(temp==0)		//Search in the trie tree
    {

    int i, j, k;
	j = strlen(key);
	TrieNode* temp = root;
	for(i = 0; i < j; i++)
	{
		k = char_to_index(key[i]);
		if(!temp->children[k])
		{
			cout<<"\n\nWord NOT Found !!!!";
			break;
		}
		temp = temp->children[k];
	}
	if(temp != NULL && temp->isEOW)
	{
		cout<<"\n\nMeaning: ";
		puts(temp->meaning);
	}
    }

	cout<<"\n\n\nDo you want to search more words?(y/n): ";
	cin >> ch;
	fflush(stdin);
	if(ch == 'y')
		goto xy3;
    file.close();
}

void display_history()		//Function to display the recent history
{
	if(top==NULL)
		cout<<"\n\n\n\n\n\n\t\t\t\t\t\t    Stack history is empty!"<<"\n";
	else
	{
		struct history *temp=top;
		while(temp->next!=NULL)
		{
			cout<<"\n"<<temp->data<<"\n";
			temp=temp->next;
		}
	
	cout<<"\n"<<temp->data;
	}
}

void SetColor(int ForgC)			//Function to set the font colour
 {
	WORD wColor;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
	    wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
	    SetConsoleTextAttribute(hStdOut, wColor);
	}
}

void ClearConsoleToColors(int ForgC, int BackC)		//Function to clear the console and for colouring the console
{
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = {0, 0};
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SetConsoleTextAttribute(hStdOut, wColor);
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
	    FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
	    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
	    SetConsoleCursorPosition(hStdOut, coord);
	}
	return;
}

void dec()		//Display word DICTIONARY in the top
{
	ClearConsoleToColors(0,11);
	SetColor(0);
	printf("\n\t\t\t  ****   *****   **** ***** *****   ****   **    *     ***     ****  *     *");
	printf("\n\t\t\t  *   *    *    *       *     *    *    *  * *   *    *   *    *   *  *   *");
	printf("\n\t\t\t  *    *   *   *        *     *   *      * *  *  *   *******   ****    ***");
	printf("\n\t\t\t  *   *    *    *       *     *    *    *  *   * *  *       *  *   *    *");
	printf("\n\t\t\t  ****   *****   ****   *   *****   ****   *    ** *         * *    *   *");
}

int main()
{
	char ch,st[50];
	TrieNode* root = newnode();
	xy1:
	dec();
	SetColor(0);
	printf("\n\n\n\n\n\n\t\t\t\t\t\t1. Add a Word in the Dictionary\n\n\t\t\t\t\t\t2. Search a Word\n\n\t\t\t\t\t\t3. Delete a word from Dictionary\n\n\t\t\t\t\t\t4. Display Search History\n\n\t\t\t\t\t\t5. Exit\n\n\nYour Choice: ");
	scanf("%c", &ch);
	fflush(stdin);
	switch(ch)
	{
		case '1':
			dec();
			printf("\n\n\n\t\t\t\t\t\t--------Insert Word--------");
			insert(root);
			break;
		case '2':
			dec();
			printf("\n\n\n\t\t\t\t\t\t--------SEARCH WORD--------");
			search(root);
			break;
		case '3':
			dec();
			printf("\n\n\n\t\t\t\t\t\t--------DELETE WORD--------");
			cout<<"\n\n\nEnter a Word to delete: ";
			gets(st);
			fflush(stdin);
			delette_tree(root, st);
			cout<<"\n\nPress Enter for Delete from File";
			getchar();
			delette_file(root, st);			
			break;
		case '4':
			dec();
			printf("\n\n\n\t\t\t\t\t\t-------- SEARCH HISTORY-------- ");
			display_history();
			break;
		case '5':
			exit(1);
				
	}
	printf("\n\n\nDo you want to do more operations ? (y/n): ");
	scanf("%c", &ch);
	fflush(stdin);
	if(ch == 'y')
		goto xy1;
	return 0;
}


