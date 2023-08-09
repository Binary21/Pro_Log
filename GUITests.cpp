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
	void testFindByType();

private:
	Gui GUI;
};

// implement tests here

void GUITests::testFindByName()
{
	auto op = GUI.findChild<QPlainTextEdit*>("editor");
	QVERIFY2(op, "Could not find widget with name: editor");
}

QTEST_MAIN(GUITests)
#include "GUITests.moc"

