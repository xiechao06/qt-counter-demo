#pragma once

#include <QtCore/qobject.h>
#include <system_error>
#include <type_traits>

enum class CommandError {
  InvalidInput,
  Busy,
  InternalFailure,
};

class CommandErrorCategory : public std::error_category {
public:
  const char *name() const noexcept override { return "CommandError"; }

  std::string message(int ev) const override {
    switch (static_cast<CommandError>(ev)) {
    case CommandError::InvalidInput:
      return "Invalid input";
    case CommandError::Busy:
      return "Command is busy";
    case CommandError::InternalFailure:
      return "Internal failure";
    }
    return "unknown";
  }
};

inline const std::error_category &command_error_category() {
  static CommandErrorCategory cat;
  return cat;
}

inline std::error_code make_error_code(CommandError e) {
  return {static_cast<int>(e), command_error_category()};
}

namespace std {
template <> struct is_error_code_enum<CommandError> : true_type {};
} // namespace std
