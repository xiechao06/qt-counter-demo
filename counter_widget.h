#include "counter_view_model.h"
#include <QWidget>
#include <QtCore/qnamespace.h>
#include <QtCore/qobject.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qwidget.h>
#include <rpp/disposables/composite_disposable.hpp>
#include <rpp/disposables/disposable_wrapper.hpp>
#include <rpp/disposables/fwd.hpp>
#include <rpp/operators/subscribe.hpp>

class CounterWidget : public QWidget {
  Q_OBJECT

public:
  explicit CounterWidget(QWidget *parent = nullptr) : QWidget(parent) {
    setWindowTitle("Qt5 MVVM Counter Demo");
    resize(320, 180);

    auto label{new QLabel};
    auto incButton{new QPushButton{"increment"}};
    auto resetButton{new QPushButton{"reset"}};

    label->setAlignment(Qt::AlignCenter);

    auto layout{new QVBoxLayout{this}};
    layout->addWidget(label);
    layout->addWidget(incButton);
    layout->addWidget(resetButton);

    QObject::connect(incButton, &QPushButton::clicked, this,
                     [this]() { viewModel.increment(); });
    QObject::connect(resetButton, &QPushButton::clicked, this,
                     [this]() { viewModel.reset(); });

    subscription = viewModel.count_observable() |
                   rpp::operators::subscribe(rpp::composite_disposable_wrapper{},
                                             [label](int value) {
                                               label->setText(QString{"Count: %1"}.arg(value));
                                             });
  }

private:
  CounterViewModel viewModel;
  rpp::composite_disposable_wrapper subscription;
};