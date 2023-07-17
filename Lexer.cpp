#include "Lexer.hpp"
#include "Token.hpp"

#include <map>
#include <string>
#include <iostream>

using namespace std;

vtpl::TokenList vtpl::tokenize(std::istream& input) {
    // intanciate return object
    TokenList tokenList;

    // create and load dictionary
    map<char, TokenType> dictionary;
    dictionarySetup(dictionary);

    string line = "";
    string temp = "";

    int lineValue = 1;
    // error check boolean
    bool error_break = false;

    // get each line from the input stream
    while (getline(input, line))
    {
        // parse each line itterably character by character
        for (size_t i = 0; i < line.length(); i++)
        {
            char c = line[i];
            // exit line if commented
            if (c == '%')
                break;

            // checks if our character has ':-' if not throws error and breaks out of
            // entire loop
            else if (c == ':')
            {
                // checks if string is empty, if not it adds it to the token list before
                // addressing the new character
                stringUpdate(temp, lineValue, tokenList);
                if (i + 1 < line.length() && line[i + 1] == '-')
                {
                    tokenList.emplace_back(TokenType::IMP, lineValue);
                    i++;
                }
                else
                {
                    tokenList.emplace_back(TokenType::ERROR, lineValue);
                    error_break = true;
                    break;
                }
            }

            // checks if our character is an alphabet and adds to temporary string if so
            else if (isalpha(c))
            {
                temp += c;
            }

            else if (isdigit(c))
            {
                if (temp.empty())
                    tokenList.emplace_back(TokenType::ERROR, lineValue);
            }

            else if (isspace(c))
            {
                stringUpdate(temp, lineValue, tokenList);
            }

            // checks if our character is any of these: , . ( or )
            // if so returns it responding value and adds it to the
            // token list
            else if (dictionary.count(c))
            {
                stringUpdate(temp, lineValue, tokenList);
                tokenList.emplace_back(dictionary[c], lineValue);
            }
        }

        // breaks out of while loop when an error is encountered
        if (error_break)
            break;
        // add the last string in the line to the token list
        stringUpdate(temp, lineValue, tokenList);
        lineValue++;
    }

    return tokenList;
}

// builds dictionary
void vtpl::dictionarySetup(std::map<char, TokenType>& dictionary)
{
    // dictionary assignment
    dictionary['('] = TokenType::OPEN;
    dictionary[')'] = TokenType::CLOSE;
    dictionary['.'] = TokenType::END;
    dictionary[','] = TokenType::COMMA;
}


// checks if string is empty, if not it adds it to the token list
void vtpl::stringUpdate(std::string& temp, int& lineValue, TokenList& tokenList)
{
    if (!temp.empty())
    {
        tokenList.emplace_back(TokenType::STRING, lineValue, temp);
        temp = "";
    }
}