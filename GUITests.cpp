//== gui_tests.cpp - Qt GUI Tests  ===========================================//
// 
//
//============================================================================//
#include <QTest>
#include "Gui.hpp"
#include "test_config.hpp"

class GUITests : public QObject {
  Q_OBJECT

private slots:

  // TODO: define tests here
	void testFindByName();
	void testInitalUI();
	//void testFindByType();

private:
	Gui GUI;
};

// implement tests here

void GUITests::testFindByName()
{
	auto op = GUI.findChild<QPlainTextEdit*>("editor");
	QVERIFY2(op, "Could not find widget with name: editor");
}

void GUITests::testInitalUI()
{
	auto editor = GUI.findChild<QPlainTextEdit*>("editor");
	QVERIFY(editor->toPlainText().isEmpty() == true);

	auto result = GUI.findChild<QPlainTextEdit*>("results");
	QVERIFY(result->toPlainText().isEmpty() == true);
	QVERIFY(result->isReadOnly() == true);

	auto trace = GUI.findChild<QCheckBox *>("trace");
	QVERIFY(trace->isChecked() == false);
}

QTEST_MAIN(GUITests)
#include "GUITests.moc"

