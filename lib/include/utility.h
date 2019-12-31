#ifndef PTABLE_UTILITY
#define PTABLE_UTILITY

#include <vector>
#include <cctype>
#include <string>
#include <fstream>
#include "method.h"

#define STR_PARAM const std::string &
#define VECTOR_STR std::vector<std::string>

namespace pTable
{
	/*
	 * void doNothing()
	 * ----------------
	 * Dummy function that does nothing
	 */
	void doNothing() {}
	void doNothing(PTABLE_METHOD_ARGS) {}

	/*
	 * bool checkstrnocase()
	 * ---------------------
	 * Checks string equality without case sensitivity
	 * Copied this over from https://thispointer.com/c-case-insensitive-string-comparison-using-stl-c11-boost-library/
	 */
	bool checkstrnocase(STR_PARAM str1, STR_PARAM str2)
	{
		return ((str1.size() == str2.size()) &&
	          std::equal(str1.begin(), str1.end(), str2.begin(),
	          	[](const char &c1, const char &c2) {return (c1 == c2 || std::toupper(c1) == std::toupper(c2));}
	        	));
	}

	/*
	 * std::vector<std::string> seperateString()
	 * -----------------------------------------
	 * Separate a string into a vector of substrings separated by whatever character
	 */
	VECTOR_STR seperateString(std::string str, std::string sepStr)
	{
		VECTOR_STR list;
		std::string word = "";

		// Get the args
		for (auto letter : str)
		{
			std::string strLetter(1, letter);
			if (strLetter.compare(sepStr) == 0)
			{
				list.push_back(word);
				word = "";
			}
			else
				word += letter;
		}
		list.push_back(word);

		return list;
	}

	/*
	 * std::vector<std::string> readCSV()
	 * ----------------------------------
	 * Stores every value of a .csv file into a vector of strings
	 * ----------------------------------
	 * PARAMS
	 * std::string filepath | The path of the file
	 * int reserve | How much elements the vector should reserve for (NULL / 0 if unsure)
	 */
	 VECTOR_STR readCSV(STR_PARAM filepath, int reserve)
	 {
		 VECTOR_STR data;
		 data.reserve(reserve);
		 std::ifstream rFile;

		 rFile.open(filepath);
		 if (rFile.is_open())
		 {
			 std::string output = "";
			 while (!rFile.eof())
			 {
				 std::string tempChar;
				 tempChar = rFile.get();
				 if (!tempChar.compare("\n") || !tempChar.compare(","))
				 {
					 data.push_back(output);
					 output = "";
				 }
				 else
				 {
					 if (tempChar.compare(" "))
					 	output += tempChar;
				 }
			 }
		 }
		 rFile.close();

		 return data;
	 }
}

#endif
