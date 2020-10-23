/*
\file:		bce.cpp
\author:	Bilal Al-Muhtadi
\date:		Mar 28, 2020
\version:	1.0
\brief:		a program that will encode a message based on a text from a book
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

/*
\fn:		generateBookCode()
\brief:		This function will generate the book code that will be used to encode the message
\param:		input stream that will have the book text file in it that will be read char by char and generate a specific code (as a string) for each char 
\return:	a vector of type string that will contain the whole book code in it
*/
vector<string> generateBookCode(ifstream& bookfile)
{
	//create counters to track line, page and column numbers
	int lineNum = 0; 
	int pageNum = 0; 
	int columnNum = 0;

	//some variables we will use when we generate a codes for the chars
	string lineNumStr;
	string pageNumStr;
	string columnNumStr;
	string coma = ",";
	string charCode;

	//create vector to save the book code in it 
	vector<string> bookCode;
	char ch1;
	while (bookfile.get(ch1))
	{

		pageNumStr = to_string(pageNum);
		lineNumStr = to_string(lineNum);
		columnNumStr = to_string(columnNum);

		charCode = pageNumStr + coma + lineNumStr + coma + columnNumStr;

		bookCode.push_back(charCode);

		columnNum++;

		if (ch1 == '\n')
		{
			lineNum++;
			columnNum = 0;
		}

		if (ch1 == '\f')
		{
			pageNum++;
			lineNum = 0;
			columnNum = 0;
		}

	}
	return bookCode;
}

/*
\fn:		generateBookTextVector()
\brief:		this function that will get the text from input stream and add it to a vector 
\param:		input stream that will have the book text file in it that will be read char by char 
\return:	a vector of type char that will contain every char in the book text file
*/
vector<char> generateBookTextVector(ifstream& bookfile)
{
	vector<char> booktext;
	char ch;
	while (bookfile.get(ch))
	{
		booktext.push_back(ch);
	}
	return booktext;
}



int main(int argc, char* argv[])
{

	ifstream bookfile(argv[1], ios::in );
	if (!bookfile)
	{
		cerr << "Input file opening failed\n";
		return EXIT_FAILURE;
	}

	ifstream message(argv[2], ios::in );
	if (!message)
	{
		cerr << "Message file opening failed\n";
		return EXIT_FAILURE;
	}

	ofstream codedMessage(argv[3], ios::out);
	if (!codedMessage)
	{
		cerr << "Coded Message file opening failed\n";
		return EXIT_FAILURE;
	}

	//call the generateBookCode() and pass it the book stream
	vector<string> bookCode = generateBookCode(bookfile);

	//restores condition states to their default
	bookfile.clear();
	bookfile.seekg(0, ios::beg);

	//call the generateBookTextVector() and pass it the book stream
	vector<char> booktext = generateBookTextVector(bookfile);


	string codedChar;
	char msgChar;
	int i;
	
	
	while (message.get(msgChar))
	{
	
		auto it = find(booktext.begin(), booktext.end(), msgChar);

		if (it != booktext.end())
		{
			i = distance(booktext.begin(), it);

			codedChar = bookCode[i];
			codedMessage << codedChar << " ";

			booktext.erase(booktext.begin() + i);
			bookCode.erase(bookCode.begin() + i);

		}
		else
		{
			cerr << "ERROR: The letter " << msgChar << " was not found in " << argv[1] << endl;
			return EXIT_FAILURE;
		}

	}


}