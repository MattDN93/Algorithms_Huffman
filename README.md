# Algorithms_Huffman
A University project to compress/decompress a text file using Huffman Coding.

# DESIGN & ANALYSIS OF ALGORITHMS README

# List of Files 
The project consists of 2 source code files, 1 executable and 2 sample text file sets:
•	Huffman.h: Huffman Code node definitions and prototypes
o	The struct to define a node as well as overloadable methods are contained in this file. Also, the traversal method, a “member” method of the node struct is included which performs the action of traversing the tree.
o	Function prototypes, flag definitions and vector, string and array variable definitions are also included here.
•	Huffman.cpp Main Huffman Compression Application
o	Performs the main program operation, presents the menus, and the user prompts, and provides end-user feedback as well as creating and managing files.
o	Compressed files are shown by the “_compressed” suffix. 
o	Decompressed files are shown by the “_decompressed” suffix.
•	HuffmanCoding_AA.exe Executable file
o	The main Windows executable application. Runs in a console or PowerShell terminal by double-clicking the exe.
o	Important: The executable must have the source text files in the same directory as itself. This allows the user to enter the filename without directory listing. If the file does not exist, the program will display a message informing the user.
•	Sample text files
o	Two sample files from the list of given test files, namely “japsafe” and “3wash” are included to test the program functionality.
o	To view previous outcomes of successful operation their compressed and decompressed variants are also given. All the processed files are available on the GitHub link.

# Usage
Usage is as follows:
1.	User starts the application up and is presented with the splash screen and menu:
 
Compression or Decompression may be chosen here. Entering Quit, closes the application.
2.	Once either option is chosen, the user is asked to enter the filename. The “txt” extension does not need to be added, the program does this automatically.
 
3.	Once the file name has been entered the application confirms it exists. If this is not the case:
a.	For Compression and decompression, the source file must exist. User will be prompted to re-enter the filename.
b.	Once compression/decompression begins a new file is created in the program path with the output of the process.
4.	Compression: the plaintext from the file is parsed and shown. The Huffman code table and binary equivalent is shown (left). Decompression: The plaintext is shown once the file is decompressed. In both cases, a timer is also shown.

# Build Environment and Variables
The application was built on Visual Studio 2013 Update 2 in C++. The Application has been tested to work on Windows 7, 8, 8.1 and 10 Preview. Functionality is not guaranteed on other operating systems. Only standard Unicode-encoded text files in “.txt” format are supported.

# Bugs
Occasionally only a single type of operation will work per program session, e.g. compression of ‘x’ files will work in a single session, but decompression in that session will fail. The solution is simply to restart the application and decompress again.

