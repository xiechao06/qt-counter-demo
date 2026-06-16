#pragma once

#include "command_error.h"
#include <exception>
#include <functional>
#include <iostream>
#include <optional>
#include <rpp/rpp.hpp>
#include <system_error>
#include <tl/expected.hpp>

template <typename T>
using command_result = tl::expected<T, std::error_code>;

template <typename T> struct CommandSnapshot {
  bool running{false};
  bool completed{false};
  std::optional<command_result<T>> result = std::nullopt;

  void clear() {
    running = false;
    completed = false;
    result = std::nullopt;
  }
};

template <typename T> class Command0 {
public:
  using Action = std::function<command_result<T>()>;
  CommandSnapshot<T> state;

  explicit Command0(Action action)
      : action{std::move(action)}, state_subject{CommandSnapshot<T>{}} {}

  void execute() {
    std::cerr << "Command executed " << std::endl;
    if (state.running) {
      state.result = tl::make_unexpected(std::error_code{CommandError::Busy});
      publish();
      return;
    }
    state.running = true;
    publish();

    try {
      state.result = action();
      state.completed = true;
    } catch (std::exception &e) {
      state.result = tl::make_unexpected(
          std::error_code{CommandError::InternalFailure});
      state.completed = true;
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
