#ifndef GUI_HPP
#define GUI_HPP

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
#include <tuple>

using namespace std;

class Gui : public QWidget {
    Q_OBJECT 

public:
    Gui(QWidget* parent = nullptr);
    void setFileName(string filename);
    void setFileContents(string file);
    void setResultContents(string file);

    bool openFile(const std::string& filename);
    bool saveFile(const std::string& filename);

    void newDocument();
    void openDocument();
    void saveDocument();
    void saveAsDocument();
    void quitApplication();
    void parseDocument(string& input);

private slots:
    void onQueryEnterPressed();

private:
    QAction* saveAction;
    QLineEdit* fileNameLineEdit;
    QPlainTextEdit* fileContentsEdit;
    QPlainTextEdit* resultsEdit;
    QLineEdit* queryLineEdit;
    QCheckBox* toggleCheckBox;

    
    QLabel* myLabel;
    QPlainTextEdit* myContents;
    string fileContents;
    string resultContents;
    string fileName;
    QMenuBar* menu;

    tuple<ParseError, vtpl::KnowledgeBase> knowledgeBase;
    string currentFile;
    void clear();
};

#endif // GUI_HPP