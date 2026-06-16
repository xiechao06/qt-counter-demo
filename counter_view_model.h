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
      : QObject(parent), increment_command([this]() { return increment(); }),
        reset_command([this]() { return reset(); }) {
    count_subject.get_observer().on_next(model.value);
  }

  Command0<int> increment_command;
  Command0<void> reset_command;

  auto count_observable() const { return count_subject.get_observable(); }

  auto count() { return model.value; }

private:
  CounterModel model;
  rpp::subjects::behavior_subject<int> count_subject{0};

  command_result<int> increment() {
    const auto newValue{model.increment()};
    qDebug() << "do increment with: " << newValue;
    count_subject.get_observer().on_next(newValue);
    return newValue;
  }
  command_result<void> reset() {
    model.reset();
    count_subject.get_observer().on_next(model.value);
    return command_result<void>{};
  };
};
