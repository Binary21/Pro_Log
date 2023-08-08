#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QLabel>
#include <QMenuBar>
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
    void setFileContents(string file);

    bool openFile(const std::string& filename);
    bool saveFile(const std::string& filename);

    void newDocument();
    void openDocument();
    void saveDocument();
    void saveAsDocument();
    void quitApplication();
    void parseDocument();
    void runQuery();

private:
    QLineEdit* fileNameLineEdit;
    QPlainTextEdit* fileContentsEdit;
    QPlainTextEdit* resultsEdit;
    QLineEdit* queryLineEdit;
    QCheckBox* toggleCheckBox;

    
    QLabel* myLabel;
    QPlainTextEdit* myContents;
    string fileName = "* No File Selected *";
    string fileContents;

    QMenuBar* menu;
};

#endif // GUI_HPP