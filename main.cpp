#include "mainwindow.h"

#include "counter_widget.h"
#include <QApplication>
#include <QLabel>
#include <QLocale>
#include <QPushButton>
#include <QTranslator>
#include <QVBoxLayout>
#include <QtCore/qnamespace.h>
#include <QtCore/qobject.h>
#include <QtWidgets/qpushbutton.h>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "counter_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }
  MainWindow w;
  w.setWindowTitle("Qt5 Counter Demo");
  w.resize(320, 180);

  auto central{new CounterWidget(&w)};
  w.setCentralWidget(central);
  w.show();
  return QApplication::exec();
}
