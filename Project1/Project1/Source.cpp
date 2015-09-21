#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Token.h"
using namespace std;

Token addToken(string type, string content, int lineCount)
{
	Token tokenAdd;
	tokenAdd.setType(type);
	tokenAdd.setContent(",");
	tokenAdd.setLineNumber(lineCount);
	return tokenAdd;
}


string removeFromString(string content, int numToRemove)
{
	return content.erase(0, numToRemove);
}

vector<Token> LexicalAnalysis(string content)
{
	vector<Token> result;
	int lineCount = 1;
	while (content.size() != 0)
	{
		Token toAdd;
		char nextChar = content[0];


		switch (nextChar)
		{
		case ',':
			toAdd = addToken("COMMA", ",", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case '.':
			toAdd = addToken("Period", ".", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case '?':
			toAdd = addToken("Q_MARK", "?", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case '(':
			toAdd = addToken("LEFT_PAREN", "(", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case ')':
			toAdd = addToken("RIGHT_PAREN", ")", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case ':':
			if (content[1] == '-')
			{
				toAdd = addToken("COLON_DASH", ":-", lineCount);
				content = removeFromString(content, 2);
				result.push_back(toAdd);
				break;
			}
			else
			{
				toAdd = addToken("COLON", ":", lineCount);
				content = removeFromString(content, 1);
				result.push_back(toAdd);
				break;
			}
		case '*':
			toAdd = addToken("MULTIPLY", "*", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case '+':
			toAdd = addToken("ADD", "+", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case '\n':
			toAdd = addToken("WHITESPACE", "\\n", lineCount);
			content = removeFromString(content, 1);
			lineCount++;
			result.push_back(toAdd);
			break;
		case '\s':
			toAdd = addToken("WHITESPACE", "\\s", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		case '\t':
			toAdd = addToken("WHITESPACE", "\\t", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		default:
			toAdd = addToken("FIX", "FIX", lineCount);
			content = removeFromString(content, 1);
			result.push_back(toAdd);
			break;
		}

	}
	return result;
	
}

string readFile(string filename)
{
	cout << "Filename: " << filename;

	ifstream fin(filename);

	if (fin.fail())
	{
		cout << "please enter a valid filename and restart program";
		return "";
	}

	string content;

	while (!fin.eof())
	{
		char line[256];	
		fin.getline(line, 256);
		content.append(line);
		content.append("\n");
	}

	cout << content;

	return content;
}


void displayOutput(vector<Token> toDisplay)
{
	int size = toDisplay.size();
	for (auto i = 0; i < size; i++)
	{
		cout << "(" << toDisplay[i].getType() << ",\"" << toDisplay[i].getContent() << "\"," << toDisplay[i].getLineNumber() << ")" << endl;
	}

	cout << "Total Tokens: " << size;
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Please include a filename" << endl;
	}

	Token token;
	string content = "";
	content = readFile(argv[1]);
	if (content == "")
	{
		char finish;
		cin >> finish;
		return 0;
	}

	vector<Token> output;

	output = LexicalAnalysis(content);

	displayOutput(output);

	char finish;
	cin >> finish;

	return 0;
}
