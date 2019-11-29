#ifndef UTILITY
#define UTILITY

#include <vector>
#include <cctype>
#include <string>

/*
 * void doNothing()
 * ----------------
 * Dummy function that does nothing
 */
void doNothing() {}
void doNothing(std::vector<std::string>) {}

/*
 * bool checkstrnocase()
 * ---------------------
 * Checks string equality without case sensitivity
 * Copied this over from https://thispointer.com/c-case-insensitive-string-comparison-using-stl-c11-boost-library/
 */
bool checkstrnocase(std::string & str1, std::string &str2)
{
	return ((str1.size() == str2.size()) &&
          std::equal(str1.begin(), str1.end(), str2.begin(),
          [](char & c1, char & c2) {return (c1 == c2 || std::toupper(c1) == std::toupper(c2));}
        ));
}

/*
 * std::vector<std::string> seperateString()
 * -----------------------------------------
 * Separate a string into a vector of substrings separated by whatever character
 */
std::vector<std::string> seperateString(std::string str, std::string sepStr)
{
	std::vector<std::string> list;
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

#endif
