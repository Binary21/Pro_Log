#ifndef GUI_H
#define GUI_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QCheckBox>

class Gui : public QWidget {
    Q_OBJECT  // This macro is required for any object that uses Qt's signal-slot system

public:
    Gui(QWidget* parent = nullptr);

private:
    QLineEdit* fileNameLineEdit;
    QPlainTextEdit* fileContentsEdit;
    QPlainTextEdit* resultsEdit;
    QLineEdit* queryLineEdit;
    QCheckBox* toggleCheckBox;
};

#endif // GUI_HPP