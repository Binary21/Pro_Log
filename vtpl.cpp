//== vtpl-ide.cpp - main driver for vtpl REPL TUI =============================//
// 
//
//============================================================================//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "KnowledgeBase.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <KnowledgeBaseUtility.hpp>

using namespace std;
using namespace vtpl;

int runREPL(vtpl::KnowledgeBase& kb) 
{
	string line;
	while (true)
	{
		cout << "vtpl> ";
		getline(cin, line);

		if (line == "quit")
		{
			return EXIT_FAILURE;
		}
		else if (line.substr(0, 5) == "trace") {
			if (line.size() > 5)
			{
				if (line.substr(6) == "on")
				{
					kb.setTrace(true);
				}
				else if (line.substr(6) == "off")
				{
					kb.setTrace(false);
				}
				else
					cerr << "Error: unknown command" << endl;
			}
		}
		else if (line.substr(0, 2) == "? ")
		{
			string queryString = line.substr(2);
			queryString.erase(queryString.find_last_not_of(" \n\r\t") + 1);
			if (queryString.back() != '.') {
				cerr << "Parse Error: query does not end with a period." << endl;
			}
			else
			{
				queryString.pop_back();
				pair<ParseError, ExpressionTreeNode> query = parseExpression(queryString);
				if (query.first.isSet())
					cerr << "Error: unable to parse because " << query.first.message() << endl;
				else
				{
					auto result = kb.ask(query.second);
					if (result.size() == 0)
						cerr << "false." << endl;
					else
					{
						if(kb.getTrace())
							cerr << kb.outputLogs << endl;
						for (Substitution subst : result)
						{
							ExpressionTreeNode application = apply(query.second, subst);
							cerr << application.toString() << endl;
						}
					}

				}
			}
		}
		else if (!line.empty())
			cerr << "Error: Unknown command" << endl;
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cerr << "Error: Incorrect number of arguments. Specify a single Prolog source code file." << endl;
		return EXIT_FAILURE;
	}

	string filename = argv[1];
	ifstream file(filename);

	if (!file.is_open())
	{
		cerr << "Error: Could not open file: " << filename << endl;
		return EXIT_FAILURE;
	}
	TokenList t1 = tokenize(file);
	tuple<ParseError, KnowledgeBase> kb;
	string line;
	try
	{
		kb = parseKnowledgeBase(t1);
		if (get<0>(kb).isSet())
		{
				cerr << "Error: " << get<0>(kb).message() << endl;
				return EXIT_FAILURE;
		}
	}
	catch (const exception& e)
	{
		cerr << "Error: " << e.what() << endl;
		return EXIT_FAILURE;
	}


	file.close();
	runREPL(get<1>(kb));

	return EXIT_SUCCESS;
}
