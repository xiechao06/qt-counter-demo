#pragma once

#include "command.h"
#include "counter_model.h"
#include <QDebug>
#include <QObject>
#include <QtCore/qobject.h>
#include <QtCore/qobjectdefs.h>
#include <rpp/rpp.hpp>
#include <rpp/subjects/behavior_subject.hpp>

class CounterViewModel : public QObject {
  Q_OBJECT
public:
  explicit CounterViewModel(QObject *parent = nullptr)
      : QObject(parent), increment_command([this]() { increment(); }),
        reset_command([this]() { reset(); }) {
    count_subject.get_observer().on_next(model.value);
  }

  Command0 increment_command;
  Command0 reset_command;

  auto count_observable() const { return count_subject.get_observable(); }

  auto count() { return model.value; }

private:
  CounterModel model;
  rpp::subjects::behavior_subject<int> count_subject{0};

  void increment() {
    const auto newValue{model.increment()};
    qDebug() << "do increment with: " << newValue;
    count_subject.get_observer().on_next(newValue);
  }
  void reset() {
    model.reset();
    count_subject.get_observer().on_next(model.value);
  };
};