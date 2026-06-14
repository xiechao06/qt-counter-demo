#pragma once

#include <cstddef>
#include <exception>
#include <optional>

template <typename T> class Result {
public:
  static Result success(T value) { return Result(std::move(value), nullptr); }
  static Result failure(std::exception_ptr error) {
    return Result(std::nullopt, error);
  }

  bool has_value() const noexcept { return value.has_value; }
  bool has_error() const noexcept { return error != nullptr; }

  std::optional<T> value;
  std::exception_ptr error;

private:
  Result(std::optional<T> value, std::exception_ptr error)
      : value{std::move(value)}, error{error} {}
};

template <> class Result<void> {
public:
  std::exception_ptr error;

  static Result success() { return Result{nullptr}; }
  static Result failure(std::exception_ptr error) { return Result{error}; }

  bool has_value() const noexcept { return error == nullptr; }
  bool has_error() const noexcept { return error != nullptr; }

private:
  explicit Result(std::exception_ptr error) : error{error} {}
};