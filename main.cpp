#include "mainwindow.h"

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

  auto central{new QWidget(&w)};
  w.setCentralWidget(central);

  auto layout{new QVBoxLayout(central)};

  auto label{new QLabel("0")};

  label->setAlignment(Qt::AlignCenter);

  auto incButton{new QPushButton("Increment")};
  auto resetButton{new QPushButton("Reset")};

  auto font{label->font()};
  font.setPointSize(24);
  label->setFont(font);

  auto counter{0};

  QObject::connect(incButton, &QPushButton::clicked, [&counter, label]() {
    ++counter;
    label->setText(QString::number(counter));
  });
  QObject::connect(resetButton, &QPushButton::clicked, [&counter, label]() {
    counter = 0;
    label->setText(QString::number(counter));
  });

  layout->addStretch();
  layout->addWidget(label);
  layout->addWidget(incButton);
  layout->addWidget(resetButton);
  layout->addStretch();

  w.show();
  return QApplication::exec();
}
