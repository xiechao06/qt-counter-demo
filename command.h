#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <rpp/rpp.hpp>

struct CommandState {
  bool running{false};
  bool completed{false};
  std::exception_ptr error;

  void clear() {
    running = false;
    completed = false;
    error = {};
  }
};

class Command0 {
public:
  using Action = std::function<void()>;
  CommandState state;

  explicit Command0(Action action)
      : action{std::move(action)}, state_subject{CommandState{}} {}

  void execute() {
    std::cerr << "Command executed " << std::endl;
    if (state.running) {
      return;
    }
    state.running = true;
    publish_state();

    try {
      action();
      state.completed = true;
      state.error = {};
    } catch (std::exception &e) {
      state.completed = false;
      state.error = std::current_exception();
    }

    state.running = false;
    publish_state();
  }

  void clear() { state.clear(); }

  auto states() const { return state_subject.get_observable(); }

private:
  Action action;
  rpp::subjects::behavior_subject<CommandState> state_subject;

  void publish_state() { state_subject.get_observer().on_next(state); }
};