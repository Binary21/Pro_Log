#include "GUI.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>

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

void Gui::setResultContents(string result)
{
    QString result_input(result.c_str());
    resultsEdit->setPlainText(result_input);
}

Gui::Gui(QWidget* parent) : QWidget(parent) {
    setWindowTitle("VTProlog IDE");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    menu = new QMenuBar(this);
    QMenu* fileMenu = menu->addMenu("File");
    fileMenu->addAction("New", this, &Gui::newDocument);
    fileMenu->addAction("Open", this, &Gui::openDocument);
    saveAction = fileMenu->addAction("Save", this, &Gui::saveDocument);
    fileMenu->addAction("Save As", this, &Gui::saveAsDocument);
    fileMenu->addAction("Quit", this, &Gui::quitApplication);

    QHBoxLayout* fileLayout = new QHBoxLayout();
    mainLayout->addLayout(fileLayout);
    // Horizontal layout to contain both file contents and results
    QHBoxLayout* contentResultLayout = new QHBoxLayout();
   
    // Section for displaying file contents
    QVBoxLayout* fileContentLayout = new QVBoxLayout();
    QLabel* fileContentsLabel = new QLabel("");
    fileContentsEdit = new QPlainTextEdit("string test");
    fileContentLayout->addWidget(fileContentsLabel);
    fileContentLayout->addWidget(fileContentsEdit);
    contentResultLayout->addLayout(fileContentLayout);

    // Section for showing results
    QVBoxLayout* resultLayout = new QVBoxLayout();
    QLabel* resultsLabel = new QLabel("");
    resultsEdit = new QPlainTextEdit();
    resultsEdit->setReadOnly(true);  // Set this if you want results to be read-only
    resultLayout->addWidget(resultsLabel);
    resultLayout->addWidget(resultsEdit);
    contentResultLayout->addLayout(resultLayout);

    mainLayout->addLayout(contentResultLayout);  // Adding the combined layout to the main layout

    // Section for adding a query
    QLabel* queryLabel = new QLabel("Enter Query:");
    queryLineEdit = new QLineEdit();
    mainLayout->addWidget(queryLabel);
    mainLayout->addWidget(queryLineEdit);
    connect(queryLineEdit, &QLineEdit::returnPressed, this, &Gui::onQueryEnterPressed);

    // Toggle check box button
    toggleCheckBox = new QCheckBox("Trace");
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();  // This pushes the checkbox to the right
    bottomLayout->addWidget(toggleCheckBox);
    mainLayout->addLayout(bottomLayout);

    // Set the main layout
    setLayout(mainLayout);
}

bool Gui::openFile(const std::string& filename) {
    clear();
    ifstream file(filename);
    if (!file.is_open())
    {
        QMessageBox::critical(this, "Error", "Parsing the file failed.");
        return false;
    }
    TokenList t1 = tokenize(file);
    
    string fileContents;
    knowledgeBase = vtpl::parseKnowledgeBase(t1);
    for (vtpl::Clause clause : std::get<1>(knowledgeBase))
    {
        fileContents += clause.head.toString();
        if (!clause.body.children.empty())
            fileContents += " :- " + clause.body.toString();
        fileContents += ".\n";
    }
    setFileContents(fileContents);
    currentFile = filename;
    return true;
}

bool Gui::saveFile(const std::string& filename) {
    ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    QString contents = fileContentsEdit->toPlainText();
    file << contents.toStdString();
    file.close();
    return true;
}

void Gui::newDocument() {
    saveAction->setEnabled(false);
    clear();
}

void Gui::clear()
{
    tuple<ParseError, KnowledgeBase> empty;
    knowledgeBase = empty;
    resultContents = "";
    queryLineEdit->clear();
    toggleCheckBox->setChecked(false);
    setResultContents(resultContents);
    setFileContents("");
}

void Gui::openDocument() {
    QString qFileName = QFileDialog::getOpenFileName(this, tr("Open Knowledge Base"), "", tr("Text Files (*.txt);;All Files (*)"));

    if (!qFileName.isEmpty())
    {
        fileName = qFileName.toStdString();
        if (!openFile(fileName))
        {
            QMessageBox::critical(this, "Error", "Could not parse the knowledge base");
        }
    }
    saveAction->setEnabled(true);

}


void Gui::saveDocument() {
    if (currentFile.empty())
    {
        QString qFilename = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");

        if (qFilename.isEmpty()) {  // User canceled the dialog
            return;
        }

        currentFile = qFilename.toStdString();
    }
    saveFile(currentFile);
}

void Gui::saveAsDocument() {
    QString qFilename = QFileDialog::getSaveFileName(this, "Save As");

    if (!qFilename.isEmpty())
    {
        QFile file(qFilename);

        if (file.exists())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "File exists", "The file already exists. Do you want to overwrite it?", QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::No)
                return;
        }
        saveFile(qFilename.toStdString());
    }
}

void Gui::quitApplication() {
    this->close();
}

void Gui::parseDocument(string& input) {
    istringstream file(input);
    TokenList t1 = tokenize(file);
    
    string fileContents;
    knowledgeBase = vtpl::parseKnowledgeBase(t1);
}

void Gui::runQuery() {
    // Implement this
}

void Gui::onQueryEnterPressed()
{
    QString queryText = queryLineEdit->text();
    QString contents = fileContentsEdit->toPlainText();
    string content = contents.toStdString();
    string queryString = queryText.toStdString();
    pair<ParseError, ExpressionTreeNode> query = parseExpression(queryString);

    content.pop_back();
    parseDocument(content);
    resultContents = "";

    
    if (!query.first.isSet())
    {
        auto result = std::get<1>(knowledgeBase).ask(query.second);
        if (toggleCheckBox->isChecked())
            resultContents += get<1>(knowledgeBase).outputLogs;

        for (Substitution subst : result)
        {
            ExpressionTreeNode application = apply(query.second, subst);
            resultContents += application.toString() + "\n";
        }
        setResultContents(resultContents);
    }
}





