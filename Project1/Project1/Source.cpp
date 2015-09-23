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
	tokenAdd.setContent(content);
	tokenAdd.setLineNumber(lineCount);
	return tokenAdd;
}

string removeFromString(string content, int numToRemove)
{
	return content.erase(0, numToRemove);
}

Token genID(string &content, int &lineCount)
{
	string idContent = "";
	idContent.push_back(content[0]);
	int i = 1;
	while (isalnum(content[i]))
	{
		idContent.push_back(content[i]);
		i++;
	}

	content = removeFromString(content, idContent.length());
	return addToken("ID", idContent, lineCount);
}

bool isKeyword(string content, string toCompare)
{
	char lastCheck = 0; 

	if(content.size() < toCompare.size())
	{
		return false;
	}

	for(auto i = 0; i < toCompare.size(); i++)
	{
		if (toCompare[i] != content[i])
		{
			return false;
		}
		lastCheck = content[i+1];
	}

	if (!isspace(lastCheck))
	{
		return false;
	}
	

	return true;
}

Token genUndefToken(string content, int lineNumber)
{
	return addToken("UNDEFINED", content, lineNumber);
}

Token interpretString(string &content, int &lineCount)
{
	string stringResult = "\'";
	int stringStartLine = lineCount;
	int i = 1;
	while (true)
	{
		if (i == content.length())
		{
			content = "";
			return genUndefToken(stringResult, stringStartLine);
		}

 		if (content[i] == '\'')
		{
			if (content[i+1] != '\'')
			{
				stringResult.push_back(content[i]);
				break;
			}
			else
			{
				stringResult.push_back(content[i++]);
			}
		}

		if (content[i] == '\n')
		{
			lineCount++;
		}

		stringResult.push_back(content[i]);

		if (content[i] =='$')
		{
			return genUndefToken(stringResult, stringStartLine);
		}
				
		i++;
	}

	content = removeFromString(content, stringResult.length());
	return addToken("STRING", stringResult, stringStartLine);
}

Token genMultiLineComment(string &content, int &lineCount)
{
	string commentContent = "#|";
	int commentStartLineCount = lineCount;
	int i = 2;

	while (true)
	{
		if (content.size() == 0)
		{
			return genUndefToken(commentContent, commentStartLineCount);
		}

		if (content[i] == '|')
		{
			if (content[i+1] == '#')
			{
				commentContent.append("|#");
				break;
			}
		}

		if (content[i] =='\n')
		{
			lineCount++;
		}

		commentContent.push_back(content[i]);
		i++;
	}

	content = removeFromString(content, commentContent.size());
	return addToken("COMMENT", commentContent, commentStartLineCount);
}

Token genSingleLineComment(string &content, int lineCount)
{
	int i = 0;
	string comment;
	while (content[i] != '\n' && content[i] != '$')
	{
		comment.push_back(content[i]);
		i++;
	}
	content = removeFromString(content, i + 1);
	return addToken("COMMENT", comment, lineCount);
}

vector<Token> LexicalAnalysis(string content)
{
	vector<Token> result;
	int lineCount = 1;
	while (content.size() > 0)
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
			break;
		case ' ':
			toAdd = addToken("WHITESPACE", "\\s", lineCount);
			content = removeFromString(content, 1);
			break;
		case '\t':
			toAdd = addToken("WHITESPACE", "\\t", lineCount);
			content = removeFromString(content, 1);
			break;
		case 'F':
			if (isKeyword(content, "Facts"))
			{
				toAdd = addToken("FACTS", "Facts", lineCount);
				content = removeFromString(content, 5);
				result.push_back(toAdd);
			}
			else
			{
				toAdd = genID(content, lineCount);
				result.push_back(toAdd);
			}
			break;
		case 'S':
			if (isKeyword(content, "Schemes"))
			{
				toAdd = addToken("SCHEMES", "Schemes", lineCount);
				content = removeFromString(content, 7);
				result.push_back(toAdd);
			}
			else
			{
				toAdd = genID(content, lineCount);
				result.push_back(toAdd);
			}
			break;
		case 'Q':
			if (isKeyword(content, "Queries"))
			{
				toAdd = addToken("QUERIES", "Queries", lineCount);
				content = removeFromString(content, 7);
				result.push_back(toAdd);
			}
			else
			{
				toAdd = genID(content, lineCount);
				result.push_back(toAdd);
			}
			break;
		case 'R':
			if (isKeyword(content, "Rules"))
			{
				toAdd = addToken("RULES", "Rules", lineCount);
				content = removeFromString(content, 5);
				result.push_back(toAdd);
			}
			else
			{
				toAdd = genID(content, lineCount);
				result.push_back(toAdd);
			}
			break;
		case '#':
			if (content[1] != '|')
			{
				toAdd = genSingleLineComment(content, lineCount);
				result.push_back(toAdd);
				lineCount++;
			}
			else
			{
				toAdd = genMultiLineComment(content, lineCount);
				result.push_back(toAdd);
			}
			break;
		case '\'':
			toAdd = interpretString(content, lineCount);
			result.push_back(toAdd);
			break;
		default:
			if (isalpha(nextChar))
			{
				toAdd = genID(content, lineCount);
				result.push_back(toAdd);
			}
			else 
			{
				string undefinedChar = "";
				undefinedChar.push_back(nextChar);
				toAdd = genUndefToken(undefinedChar, lineCount);
				content = removeFromString(content, 1);
				result.push_back(toAdd);
			}
			break;
		}

	}

	Token toAdd = addToken("EOF", "", lineCount);
	result.push_back(toAdd);
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

	char line[256];
	while (fin.getline(line, 256))
	{
		content.append(line);
		content.push_back('\n');
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
