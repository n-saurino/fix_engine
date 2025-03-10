#pragma once
#include <cstring>
#include <iostream>

/* Defining macros for use through all modules */

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

inline auto ASSERT(bool condition, const std::string& msg) noexcept {
  if (UNLIKELY(!condition)) {
    std::cerr << msg << std::endl;
    // throw std::runtime_error(msg);
    exit(EXIT_FAILURE);
  }
}

inline auto FATAL(const std::string& msg) noexcept {
  std::cerr << msg << std::endl;
  exit(EXIT_FAILURE);
}
