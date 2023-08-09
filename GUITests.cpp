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
	void testSimpleKBCreation();
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

void GUITests::testSimpleKBCreation()
{
	QString KnowledgeBase = "friends(X,Y) :- likes(X,Z), likes(Y,Z). \n likes(bill, movies). \n likes(sally, movies). \nlikes(bob, pizza).";
	QString Query = "friends(sally,X).";
	auto editor = GUI.findChild<QPlainTextEdit*>("editor");
	auto query = GUI.findChild<QLineEdit*>("query");
	editor->insertPlainText(KnowledgeBase);
	query->setText(Query);
	QTest::keyClick(query, Qt::Key_Enter);
	auto results = GUI.findChild<QPlainTextEdit*>("results");
	QVERIFY(results->toPlainText().isEmpty() == false);

}

QTEST_MAIN(GUITests)
#include "GUITests.moc"

