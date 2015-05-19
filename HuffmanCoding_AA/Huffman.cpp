/*	DESIGN & ANALYSIS OF ALGORITHMS
		MATTHEW DE NEEF
		212503024
		HUFFMAN CODING
	This program presents a method of optimal prefix coding
	known as Huffman coding. Please read the report for the 
	full theoretical background.

	This code also available on http://github.com/mattdn93/Algorithms_Huffman

	Compiled in VS2013 on Windows 10 10074
	PROGRAM I/O
	------------------------------------------------------
	Inputs: receives a text file located in the same directory as the app
			*Can be either compressed or uncompressed format
			*Filename may be specified
	Outputs: outputs a file with the result of the process:
			*if decompression, the decompressed file with "_decompressed" is saved
			*if compression, the compressed file with "_compressed" is saved
	Prerequisites:  *The source text file for compression/decompression 
					*Huffman.h header file
*/

#include "Huffman.h"

priority_queue< node, vector<node>, node > q;				//initialised priority queue used to store nodes

//--------------Menu System----------//

void progLoad()
{
	system("cls");
	cout << "\t\t\t| -------------------------------------------|\n" << endl;
	cout << "\t\t\t| Design & Analysis of Algorithms ENEL4AA    | \n" << endl;
	cout << "\t\t\t| Huffman Compression/Decompression          | \n" << endl;
	cout << "\t\t\t| -------------------------------------------| \n" << endl;
	cout << "\t\t\t| Matthew de Neef | 212503024\t	     | \n" << endl;
	cout << "\t\t\t| -------------------------------------------|\n" << endl;
	system("pause");
}


void showMenu(void)
{
	system("cls");
	cout << "\t\t\t| -------------------------------------------|\n" << endl;
	cout << "\t\t\t|\t\tMain Menu\t\t     | \n" << endl;
	cout << "\t\t\t| -------------------------------------------|\n" << endl;
	cout << "\t\t\t<Instruction:> Please choose an option below." << endl;
	cout << "\n\t\t\t   1) Compress data" << endl;
	cout << "\n\t\t\t   2) Decompress data" << endl;
	cout << "\n\t\t\t   (Note: Verify the file exists in the\n\t\t\t    current app directory!)" << endl;
	cout << "\n\t\t\t   3) Exit" << endl;

}

void menuSelection(void)
{
	int sel = 0;
	cout << "\t\t\t| -------------------------------------------|\n" << endl;
	cout << "\t\t\t| Your selection:\n\t\t\t" << endl;
	cin >> sel;

	switch (sel)
	{
	case 1: system("cls"); processResult = compress(); break;
	case 2: system("cls"); processResult = decompress(); break;
	case 3: cout << "Quitting..." << endl; system("pause"); exit(0);
	default: menuError(); break;
	}
	showMenu(); menuSelection();
}

void menuError()
{
	system("cls");
	cout << "\t\t\t| -------------------------------------------|\n" << endl;
	cout << "\t\t\t| ERROR!				     | \n" << endl;
	cout << "\t\t\t| Invalid option. Try again!		     | \n" << endl;
	cout << "\t\t\t| -------------------------------------------| \n" << endl;
	fflush(stdin);
	system("pause");
	system("cls");
}

//--------------------------Timing and file management------------------------//
//Reference https ://www.physicsforums.com/threads/measuring-time-elapsed-in-c.224989/
double diffclock(clock_t clock1, clock_t clock2) 
{ 
	double diffticks = clock1 - clock2;     
	double diffms = (diffticks /** 1000*/) / CLOCKS_PER_SEC;     
	return diffms; 
}



int fileOpen(int process)
{
	string input_filename;
	string input_f_name_ext;
	cout << system("cls");
	cout << "\t| --------------------------------------------------------|\n" << endl;
	cout << "\t| Verify the file exists in the current app directory.    | \n" << endl;
	cout << "\t| Note: DO NOT include the file extension (will be done atuomatically)!       | \n" << endl;
	cout << "\t| --------------------------------------------------------|\n" << endl;
	cout << "\t| Enter the filename: "; cin >> input_filename; cout << "\t\t\t|\n"<< endl;

	input_f_name_ext = input_filename + ".txt";								//reads in filename from the user and outputs it with "xxxx.txt"
	input_file.open(input_f_name_ext.c_str());
								

	if ( !input_file.is_open( ) )											//error checking
	{
		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| Error. File couldn't be opened.			  | \n" << endl;
		cout << "\t| Either check your spelling or\n\t\t\t  confirm the file exists in the directory.		  | \n" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl; 
	return COMPRESS_FAIL;
	}
	else if (input_file.is_open())												//if the file is still available
	{
		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| File opened OK.				      | \n" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
	
		if (process == COMPRESS)														//for compression we simply read all the plaintext in from the file
		{
			for (text_from_file; getline(input_file, text_from_file);)					//read the input from the file
				/*while (getline(input_file,text_from_file))  FOR SIMPLE CODE*/
			{
				//cout << text_from_file << '\n';										//TESTING: raw data output
				str += text_from_file;													//save to the appropriate string for modification
				/*str = text_from_file; FOR SIMPLE CODE*/
			}
		}
			
		if (process == DECOMPRESS)														//for decompression we read the header code table & binary separately
		{
			for (text_from_file; getline(input_file, text_from_file_bin);)				//read separate parts of the document at a time
			{
				getline(input_file, text_from_file); 
				getline(input_file, text_from_file);
				str += text_from_file;													//this string stores the huffman code table
				str_bin += text_from_file_bin;											//this string stores the binary compressed version of the file
			}
		}


	if (process == COMPRESS)														//different actions if compressing/decompressing
	{
		fileOpenOut(input_filename,COMPRESS);
		//open a file for writing so long
	}
	if (process == DECOMPRESS)														//different actions if compressing/decompressing
	{
		fileOpenOut(input_filename, DECOMPRESS);
		//open a file for writing so long
	}
	
  return COMPRESS_SUCCESS;														//return a success flag
	}
}

void fileOpenOut(string filename,int mode)
{

	if (output_file.is_open())												//if the file is already available
	{
		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| File opened for writing OK.				      | \n" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
	}
	else
	{
		if (mode == COMPRESS)												//appends "_compressed" to the file and creates the output file
		{
			text_to_file = filename + "_compressed.txt";
			output_file.open(text_to_file.c_str());
		}
		else if (mode == DECOMPRESS)										//appends "_decompressed" to the file and creates the output file
		{
			text_to_file = filename + "_decompressed.txt";
			output_file.open(text_to_file.c_str());
		}
		
	}
	

	if (!input_file.is_open())												//error checking
	{
		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| Error. File couldn't be opened for writing.  | \n" << endl;
		cout << "\t| Either check HDD space or\n\t\t\t  confirm permissions in the directory.		  | \n" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
	}

}
	
//---------------------------------COMPRESSION METHOD------------------------------------//

int compress()										
{
	int file_result=0;															
	file_result = fileOpen(COMPRESS);
	if (file_result == COMPRESS_FAIL)
	{
		system("pause");
		return COMPRESS_FAIL;													//return an error condition if file opening failed catastrophically
	}

	trav_type = TABULAR_OUT;													//reset this variable
	//FOR TESTING ONLY cin >> str;												
	cout << system("cls");
	cout << "\t| --------------------------------------------------------|\n" << endl;
	cout << "\t| Contents of this file:				 | \n" << endl;
	
	vector <char> input_chars(str.begin(), str.end());							//store the file input characters into a vector to access each character of the string separately

	cout << "\t\t";
	for (int i = 0; i < input_chars.size(); i++)								//List the file contents, from the new vector (also confirms vector stored correct chars)
	{
		cout << input_chars[i];
	}
	cout << "\n\t| --------------------------------------------------------|\n" << endl;
	input_file.close();															//file management, close the input file!

	in_char_global = input_chars;												

	cout << "\n" << endl;
	processResult = 0;															//clear potential previous error messages
	try{
		for (int i = 0; i <= 256; i++) 											//casts the ASCII character to its integer equivalent. We assume uppercase integers here!
		{																										
			c = (char)i;
			cnt = (int)count(str.begin(), str.end(), c);						//counts the occurrence of each alphabetic char A,B,C->Z in the input string
			if (cnt) 
			{
				q.push(huffNode(c, cnt));										//pushes the alphabetic char and its frequency to the Huffman Tree if found in input
				cnt = 0;														//reset for next char
			}
		}


		while (q.size() != 1) {													//if the tree is nonzero in size
			huffNode *left = new huffNode(q.top());								//create a left child of type HuffNode
			q.pop();															//populate the left child with the front value of the priority queue
			huffNode *right = new huffNode(q.top());							//create a right child of type HuffNode
			q.pop();															//populate the right child with the front value of the priority queue
			q.push(huffNode(left, right));										//push the new node onto the rear of the queue
		}

		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| Encoding Table with Huffman Compression Algorithm\n" << endl;
		cout << "\t| Symbol" << "\t" << "Frequency" << "\t" << "Assigned Binary Code" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
		trav_type = TABULAR_OUT;												//NOTIFIES TRAVERSAL METHOD TO OUTPUT TABLE

		//----------------------------resizing operation------------------------
		// Resizes a Nx2 matrix, where N = text size; 2 columns, 
		stored_comp_codes.resize(256);//input_chars.size());							//resize storage variable to input array size e.g n rows
		
		//for (int i = 0; i < input_chars.size(); i++)	
		//{
		//	stored_comp_codes[i].resize(2);										//n rows at 2 columns here created
		//}

		

		/*-----------------------------matching operation-----------------------//
		Iterates looking for each character in the input string, matching it to the appropriate one in the Huffman code vector
		and printing out the equivalent binary Huffman code for it . 
		Prerequisites: Requires populated Nx2 vector matrix containing each unique char and its associated Huffman Code
		Output: Outputs the compressed bits of the text and also to the file.
				Also outputs the ASCII chars with their associated code in comma separated form to the file for decompression later
				output_compressed = only the compressed bits generated (overwritten each time)
																					*/
		clock_t compress_begin_coding_table = clock();							//STARTS TIMING CALCULATION

		//Reference https ://www.physicsforums.com/threads/measuring-time-elapsed-in-c.224989/

		huffcode_csv.resize(input_chars.size());		//initially size the vector to the size of the input. This is inefficient ad will be resized later

		q.top().traverse();				//IMPORTANT: perform TREE TRAVERSAL AND CREATION
		
		huffcode_csv.resize(comp_char);	//resize the comma separated value array to the size of the values captured

		clock_t compress_end_coding_table = clock();								//ENDS TIMING CALCULATION FOR CODE TABLE
		system("pause");

		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t  Huffman Binary compressed text:\n" << endl;
		cout << "\t  ";

		clock_t compress_begin_encode_proc = clock();								//STARTS TIMING FOR ENCODING TO BINARY

		for (int i = 0; i < str.length(); i++)
		{
			cout << stored_comp_codes[(int)str[i]];									//display to screen and...
			output_file << stored_comp_codes[(int)str[i]];							//...write to file

		}
		cout << "\n\n";
		output_file << "\n\n";														//separate the binary encoded from the coded table in "~" delimiter form
		//output_file << ">>,";														//DELIMITER for the start of the code table [DEPRECATED]

		for (int i = 0; i < huffcode_csv.size(); i++)
		{
			cout << huffcode_csv[i];												//VALIDATION ONLY outputs CSV code values to screen
			output_file << huffcode_csv[i];											//prints each ASCII char and it's code to the file
		}
																					//output the Huffman code in comma separated form
		//output_file << "<<";														//DELIMITER for the end of the code table [DEPRECATED]

		clock_t compress_end_encode_proc = clock();									//ENED TIMING FOR BINARY ENCODING OF FILE

		//--------------------------TIMING RESULTS---------------------------------//
		/* Process: Each clock_t object generated starts a clock timer counting from that
					point in the execution. To find the elapsed time the difference between
					these clock objects is found.
		   Input: None
		   Output: Time in seconds to execute the applicable operation. 2 decimal precision.
			
																					*/

		cout << "\n\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| Check the '_compressed' text file for the compressed text.| \n" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| TIMING RESULTS:| \n" << endl;
		cout << "\t| Time to generate the coding table: " << double(diffclock(compress_end_coding_table, compress_begin_coding_table)) << " s." << endl;
		cout << "\t| Time to generate the compressed binary: " << double(diffclock(compress_end_encode_proc, compress_begin_encode_proc)) << " s." << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
		output_file.close();															//file management, close the output file!
		cout << "\n";
		system("pause");
		
	}
	catch (exception& ex)						//catch any exceptions from the standard C Libs
	{
		cout << "An error has occurred. The code is '" << ex.what() << "'. Exiting process..." << endl;
		return COMPRESS_FAIL;					//an error occurred somewhere
	}
	return COMPRESS_SUCCESS;					//process completed successfully
}

//-----------------------------------------DECOMPRESSION METHOD-----------------------------------------//

int decompress()
{

	cout << "\t| --------------------------------------------------------|\n" << endl;
	cout << "\t| Decompression.   | \n" << endl;
	cout << "\t| Choose a file to decompress here.	  | \n" << endl;
	cout << "\t| --------------------------------------------------------|\n" << endl;
	clock_t decompress_begin_coding_table = clock();
	processResult = fileOpen(DECOMPRESS);											//prompts user to enter the filename to open
	if (processResult == DECOMPRESS_FAIL)
	{
		cout << "\n\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| Error. Tabulated ASCII chars/code pairs not found.| \n" << endl;
		cout << "\t| Error. These are required to perform decompression.| \n" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
		return DECOMPRESS_FAIL;
	}

	/*-----------------------------Finding Assigned Codes-----------------------
	Checks the input file for the ~ separated char,code pairs showing each ASCII char and it's optimal code
	This is parsed and stored and used for the decompression when the user enters a compressed binary representation.

	Prerequisites: Requires populated  vector containing each unique char and its associated Huffman Code
	Output: Outputs the compressed bits of the text and also to the file.
	output_compressed = only the compressed bits generated (overwritten each time)
	*/
	processResult = 0;										//clear potential previous error messages

	decompress_input_csv.resize(input_chars.size());		//resize the input vector tentatively--it'll never be larger than the input!

	
	std::stringstream ss(str);								//create stringstream object to read in file contents
	string iter;
	while (getline(ss, iter,'~'))
		{
			c = iter[0];										//1st char ALWAYS the ASCII equivalent
			iter.erase(0,1);									//remove the delimiter char from the mapping
			s = iter;											//set the contents of the iterator to s
			mymap[s] = c;										// save the char and its associated code to the map
			decompress_input_csv.push_back(iter);
		}


		cout << "\t| --------------------------------------------------------|\n" << endl;
		cout << "\t| THIS TABLE WAS GENERATED FROM THE TEXT FILE VALUES\n" << endl;
		cout << "\t| Encoding Table with Huffman Compression Algorithm\n" << endl;
		cout << "\t| Symbol" << "\t" << "Frequency" << "\t" << "Assigned Binary Code" << endl;
		cout << "\t| --------------------------------------------------------|\n" << endl;
		for (int i = 0; i < mymap.size(); i++)
		{
			//cout << mymap << "\t\t" << endl;		//if for screen printing [DEPRECATED]
		}
		
		//------------------decode the binary equivalnt------------------------//
		clock_t decompress_end_coding_table = clock();

		clock_t decompress_begin_binparse = clock();

		/* Code Searching Method
			Process: Traverses the tree searching for leaf nodes, building up a binary code
					 as it goes, this binary code is compared with Huffman codes in the 
					 compressed document and a associated char is assigned if the code
					 is found.
			Input: None externally, uses internal header-file based vectors
			Output: "ans" string containing document plaintext
		
																						*/

	t = "";
	ans = "";
	for (int i = 0; i < str_bin.size(); i++) {			//for each binary received digit
		t += " ";										//t is the code to be "built up"
		t[t.size() - 1] = str_bin[i];					//the latest binary value is appended to the code
		it = mymap.find(t);								//the current iteration of the code searched in the map
		if (it != mymap.end()) {						//if found...
			c = mymap[t];								//append the associated char to the output
			ans += " ";
			ans[ans.size() - 1] = c;
			t = "";										//reset the codework lookup
		}												//if not found, keep building t til a valid code found
	}



	
	cout << "\n\t| --------------------------------------------------------|\n" << endl;
	cout << "\t| Plaintext output of the decompression:| \n" << endl;
	cout << ans << endl;													//output the decompressed ASCII
	output_file << ans;														//write to file
	clock_t decompress_end_binparse = clock();
	cout << "\n\t| --------------------------------------------------------|\n" << endl;

	//--------------------------TIMING RESULTS---------------------------------//
	/* Process: Each clock_t object generated starts a clock timer counting from that
	point in the execution. To find the elapsed time the difference between
	these clock objects is found.
	Input: None
	Output: Time in seconds to execute the applicable operation. 2 decimal precision.

	*/

	cout << "\t| --------------------------------------------------------|\n" << endl;
	cout << "\t| TIMING RESULTS:| \n" << endl;
	cout << "\t| Time to parse and generate the plaintext: " << double(diffclock(decompress_end_coding_table, decompress_begin_coding_table)) << " s." << endl;
	cout << "\t| Time to parse binary code table: " << double(diffclock(decompress_end_binparse, decompress_end_binparse)) << " s." << endl;
	cout << "\t| Check the '_decompressed' text file for the decompressed text.| \n" << endl;
	cout << "\t| --------------------------------------------------------|\n" << endl;
	output_file.close();													//file manager, close file safely
	//ans = "";
	system("pause");
	return DECOMPRESS_SUCCESS;
}

int main() {																//simply initiates application run

	progLoad();
	do{
		showMenu();
		menuSelection();
		if ((processResult == DECOMPRESS_FAIL) || (processResult == COMPRESS_FAIL))
		{
			system("cls");
			cout << "Some errors occurred. The operation could not be completed. Please try again." << endl;
		}
	} while (1);
}