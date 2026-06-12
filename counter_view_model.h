#pragma once

#include "counter_model.h"
#include <QObject>
#include <QtCore/qobject.h>
#include <QtCore/qobjectdefs.h>
#include <rpp/rpp.hpp>
#include <rpp/subjects/behavior_subject.hpp>

class CounterViewModel : public QObject {
  Q_OBJECT
public:
  explicit CounterViewModel(QObject *parent = nullptr) : QObject(parent) {
    count_subject.get_observer().on_next(model.value);
  }

  void increment() {
    const auto newValue{model.increment()};
    count_subject.get_observer().on_next(newValue);
  }

  void reset() {
    model.reset();
    count_subject.get_observer().on_next(model.value);
  };

  auto count_observable() const { return count_subject.get_observable(); }

  auto count() { return model.value; }

private:
  CounterModel model;
  rpp::subjects::behavior_subject<int> count_subject{0};
};