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
		//hello.setFileName(filename);
		//cout << "two inputs" << endl;
		hello.openFile(filename);
	}
	hello.show();
  return app.exec();
} 
