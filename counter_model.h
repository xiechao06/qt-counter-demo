#pragma once

struct CounterModel {
  int value = 0;
  int increment() noexcept { return ++value; }
  void reset() noexcept { value = 0; }
};