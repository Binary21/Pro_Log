#include "GUI.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>

Gui::Gui(QWidget* parent) : QWidget(parent) {
    setWindowTitle("VTProlog IDE");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Section for file name
    QLabel* fileLabel = new QLabel("File Name:");
    QLabel* fileNameDisplayLabel = new QLabel("Some File");
    QHBoxLayout* fileLayout = new QHBoxLayout();
    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(fileNameDisplayLabel);
    mainLayout->addLayout(fileLayout);

    // Horizontal layout to contain both file contents and results
    QHBoxLayout* contentResultLayout = new QHBoxLayout();

    // Section for displaying file contents
    QVBoxLayout* fileContentLayout = new QVBoxLayout();
    QLabel* fileContentsLabel = new QLabel("File Contents:");
    QPlainTextEdit* fileContentsEdit = new QPlainTextEdit();
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