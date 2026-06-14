#pragma once

#include "result.h"
#include <exception>
#include <functional>
#include <iostream>
#include <rpp/rpp.hpp>
#include <type_traits>

template <typename T> struct CommandSnapshot {
  bool running{false};
  bool completed{false};
  Result<T> result = Result<T>::success(T{});

  void clear() {
    running = false;
    completed = false;
    result = Result<T>::success(T{});
  }
};

template <> struct CommandSnapshot<void> {
  bool running{false};
  bool completed{false};
  Result<void> result = Result<void>::success();
};

template <typename T> class Command0 {
public:
  //   using Action = std::conditional_t<std::is_void_v<T>,
  //   std::function<void()>, std::function<T()>>;
  using Action = std::function<T()>;
  CommandSnapshot<T> state;

  explicit Command0(Action action)
      : action{std::move(action)}, state_subject{CommandSnapshot<T>{}} {}

  void execute() {
    std::cerr << "Command executed " << std::endl;
    if (state.running) {
      return;
    }
    state.running = true;
    publish();

    try {
      if constexpr (std::is_void_v<T>) {
        action();
        state.result = Result<void>::success();
      } else {
        state.result = Result<T>::success(action());
      }
      state.completed = true;
    } catch (std::exception &e) {
      state.completed = false;
    }

    state.running = false;
    publish();
  }

  void clear() { state.clear(); }

  auto states() const { return state_subject.get_observable(); }

private:
  Action action;
  rpp::subjects::behavior_subject<CommandSnapshot<T>> state_subject;

  void publish() { state_subject.get_observer().on_next(state); }
};