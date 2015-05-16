/*	DESIGN & ANALYSIS OF ALGORITHMS
			MATTHEW DE NEEF
				212503024
			 HUFFMAN CODING
This program presents a method of optimal prefix coding
known as Huffman coding. Please read the report for the
full theoretical background.

This file is the header with declararions and flags etc.

Compiled in VS2013 on Windows 10 10074*/

#include <iostream>
#include <queue>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

//--------------Flags for status--------------//
#define DECOMPRESS_SUCCESS	1
#define DECOMPRESS_FAIL		-1
#define COMPRESS_SUCCESS	2
#define COMPRESS_FAIL		-2

#define LINEAR_OUT			3
#define TABULAR_OUT			4

#define COMPRESS			5
#define DECOMPRESS			6

//--------------Function Prototypes----------//
void progLoad();
void showMenu();
void menuSelection();
void menuError();

int fileOpen(int);
void fileOpenOut(string,int);

int compress();
int decompress();

double diffclock();
//------------File Opening Vars-------------//
ifstream input_file;
string text_from_file;
string text_from_file_bin;

ofstream output_file;
string text_to_file="";
//----------------Compression Vars----------//
int processResult;
vector <char> input_chars;					//vector for individual chars from input
vector <char> in_char_global;				
vector <string> stored_comp_codes;
vector <string> huffcode_csv;				//comma separated huffcode mapping so decoder can use it
string str;									//string holding input ASCII from file
int comp_char=0;
int cnt;
char c;

//------------Decompression Vars-------------//
int i, j, k, a, b, n;
string in_csv;						//input csv string
string s, t, ans;
string str_bin;						//binary input string stream
vector <string> decompress_input_csv;		//header part for the encodng table
vector <string> decompress_input_binary;	//actual binary data
map <string, char> mymap;			//key value pairs for symbols and their code
map <string, char> ::iterator it;


//----Struct Declarations/Functions----------//
int trav_type=TABULAR_OUT;				//type of traversal (linear, tabular)

typedef struct node {					//struct to hold the node
	int freq;
	char ch;
	node* left;
	node* right;

	node(char c = ' ', int f = -1)		//initializer with optional parameters
	{
		ch = c;
		freq = f;
		left = NULL;
		right = NULL;
	}

	node(node* c1, node* c2)
	{
		ch = ' ';
		freq = c1->freq + c2->freq;
		left = c1;
		right = c2;
	}

	bool operator ()(const node& l, const node &r)
	{
		return l.freq >r.freq;
	}

	void traverse(string code = "") const;

}huffNode;

void node::traverse(string code) const
{	
	if (left) {				
		left->traverse(code + '0');
		right->traverse(code + '1');
	}
	else if (trav_type == TABULAR_OUT) {							//codes[i][0] = char; codes[i][1] = code
		cout << "\t ";
		cout << ch << "\t\t" << freq << "\t\t" << code << endl;		//if for screen printing

		stored_comp_codes[(int)ch] = code;
		//stored_comp_codes[comp_char][0] = (int)ch;						//stores <code>	<xxxx>
		//stored_comp_codes[comp_char][1] = code;						//stores [char] <code>
		huffcode_csv[comp_char].append(ch + code);					//stores each char and code in CSV format for decoding later
		huffcode_csv[comp_char].append("~");						//REPLACED ',' in form "<char><code>,"
		comp_char++;												//increment counter
	}
	else if (trav_type == LINEAR_OUT){		
		for (int i = 0; i < in_char_global.size(); i++)
		{
			if (in_char_global[i]==ch)
			{

				cout << code;
				break;
			}
		}
		//cout << code;											//prints out encoded form only
	}
}

