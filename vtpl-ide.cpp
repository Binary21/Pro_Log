//== vtpl-ide.cpp - main driver for vtpl IDE GUI ==============================//
// 
//
//============================================================================//
#include <QApplication>
#include <QLabel>
#include "GUI.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <unordered_set>
#include "KnowledgeBase.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "KnowledgeBaseUtility.hpp"


using namespace std;
using namespace vtpl;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	Gui hello;

	if (argc == 2)
	{
		string filename = argv[1];
		hello.setFileName(filename);
		ifstream file(filename);
		if (!file.is_open())
		{
			cerr << "Error: Could not open file: " << filename << endl;
			return EXIT_FAILURE;
		}
		std::stringstream buffer;
		buffer << file.rdbuf();

		std::string content = buffer.str();
		//TokenList t1 = tokenize(content);
		tuple<ParseError, KnowledgeBase> kb;

		kb = vtpl::parseKnowledgeBase(content);
		/**for (vtpl::Clause clause : std::get<1>(kb))
		{
			fileContents += clause.head.toString();
			if (clause.body.contents != "")
				fileContents += " :- " + clause.body.toString();
			fileContents += "\n";
		}

		hello.setFileContents(fileContents);**/
	}
		
	
	hello.show();
  return app.exec();
} 
