#pragma once
#include <string>
using namespace std;

class Token
{
private:
	
	string type;
	string content;
	int lineNumber;
	

public:
	Token(void);
	~Token(void);
	
	//getters 
	string getContent()	{ return this->content; }
	int getLineNumber() { return this->lineNumber; }
	string getType()    { return this->type; }

	//setters
	void setLineNumber(int lineNumber)	{ this->lineNumber = lineNumber; }
	void setContent(string content)	    { this->content = content;	}
	void setType(string type)             { this->type = type; };
};

