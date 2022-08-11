#pragma once

#include <utility>

#define TOKEN_CONCAT_IMPL(x, y) x##y
#define TOKEN_CONCAT(x, y) TOKEN_CONCAT_IMPL(x, y)
#define ANONYMOUS_VARIABLE(prefix) TOKEN_CONCAT(prefix, __COUNTER__)

namespace falcon {

#define ON_SCOPE_EXIT                                     \
  [[maybe_unused]] auto ANONYMOUS_VARIABLE(SCOPE_EXIT_) = \
      ScopeGuardOnExit() + [&]()

template <typename F>
class ScopeFunctor {
 public:
  explicit constexpr ScopeFunctor(F func) : m_func(std::move(func)) {}

  ScopeFunctor(const ScopeFunctor &&) = delete;

  ~ScopeFunctor() { m_func(); }

 private:
  F m_func;
};

enum class ScopeGuardOnExit {};

template <typename F>
constexpr auto operator+(ScopeGuardOnExit, F &&f) -> ScopeFunctor<F> {
  return ScopeFunctor<F>(std::forward<F>(f));
}

}  // namespace falcon
