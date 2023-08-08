#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QLabel>
#include "KnowledgeBase.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "KnowledgeBaseUtility.hpp"

using namespace std;

class Gui : public QWidget {
    Q_OBJECT  // This macro is required for any object that uses Qt's signal-slot system

public:
    Gui(QWidget* parent = nullptr);
    void setFileName(string filename);
    void setFileContents(ifstream file);


private:
    QLineEdit* fileNameLineEdit;
    QPlainTextEdit* fileContentsEdit;
    QPlainTextEdit* resultsEdit;
    QLineEdit* queryLineEdit;
    QCheckBox* toggleCheckBox;

    bool openFile(const string& filename);
    
    QLabel* myLabel;
    QPlainTextEdit* myContents;
    string fileName = "* No File Selected *";
    string fileContents;
};

#endif // GUI_HPP