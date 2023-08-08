#include "GUI.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QString>
#include <string>
#include <fstream>

using namespace vtpl;

void Gui::setFileName(string filename)
{
    fileName = filename;
    myLabel->setText(QString::fromLocal8Bit(filename.c_str()));
}

void Gui::setFileContents(string file)
{
    QString file_input(file.c_str());
    fileContentsEdit->setPlainText(file_input);
}


Gui::Gui(QWidget* parent) : QWidget(parent) {
    setWindowTitle("VTProlog IDE");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    menu = new QMenuBar(this);
    QMenu* fileMenu = menu->addMenu("File");
    fileMenu->addAction("New", this, &Gui::newDocument);
    fileMenu->addAction("Open", this, &Gui::openDocument);
    fileMenu->addAction("Save", this, &Gui::saveDocument);
    fileMenu->addAction("Save As", this, &Gui::saveAsDocument);
    fileMenu->addAction("Quit", this, &Gui::quitApplication);

    QHBoxLayout* fileLayout = new QHBoxLayout();
    mainLayout->addLayout(fileLayout);
    // Horizontal layout to contain both file contents and results
    QHBoxLayout* contentResultLayout = new QHBoxLayout();
   
    // Section for displaying file contents
    QVBoxLayout* fileContentLayout = new QVBoxLayout();
    QLabel* fileContentsLabel = new QLabel("File Contents:");
    fileContentsEdit = new QPlainTextEdit("string test");
    fileContentLayout->addWidget(fileContentsLabel);
    fileContentLayout->addWidget(fileContentsEdit);
    contentResultLayout->addLayout(fileContentLayout);

    // Section for showing results
    QVBoxLayout* resultLayout = new QVBoxLayout();
    QLabel* resultsLabel = new QLabel("Results:");
    QPlainTextEdit* resultsEdit = new QPlainTextEdit();
    resultsEdit->setReadOnly(true);  // Set this if you want results to be read-only
    resultLayout->addWidget(resultsLabel);
    resultLayout->addWidget(resultsEdit);
    contentResultLayout->addLayout(resultLayout);

    mainLayout->addLayout(contentResultLayout);  // Adding the combined layout to the main layout

    // Section for adding a query
    QLabel* queryLabel = new QLabel("Enter Query:");
    QLineEdit* queryLineEdit = new QLineEdit();
    mainLayout->addWidget(queryLabel);
    mainLayout->addWidget(queryLineEdit);

    // Toggle check box button
    QCheckBox* toggleCheckBox = new QCheckBox("Trace");
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();  // This pushes the checkbox to the right
    bottomLayout->addWidget(toggleCheckBox);
    mainLayout->addLayout(bottomLayout);

    // Set the main layout
    setLayout(mainLayout);
}

bool Gui::openFile(const std::string& filename) {
    ifstream file(filename);
    TokenList t1 = tokenize(file);
    tuple<ParseError, KnowledgeBase> kb;
    string fileContents;
    kb = vtpl::parseKnowledgeBase(t1);
    for (vtpl::Clause clause : std::get<1>(kb))
    {
        fileContents += clause.head.toString();
        if (clause.body.contents != "")
            fileContents += " :- " + clause.body.toString();
        fileContents += "\n";
    }
    setFileContents(fileContents);
    return true;
}

bool Gui::saveFile(const std::string& filename) {
    // Implement this
    return true;
}

void Gui::newDocument() {
    // Implement this
}

void Gui::openDocument() {
}


void Gui::saveDocument() {
    // Implement this
}

void Gui::saveAsDocument() {
    // Implement this
}

void Gui::quitApplication() {
    // Implement this
}

void Gui::parseDocument() {
    // Implement this
}

void Gui::runQuery() {
    // Implement this
}





