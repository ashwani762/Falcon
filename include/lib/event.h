#pragma once

#include <functional>
#include <map>
#include <utility>
#include <vector>

namespace falcon {

template <typename... Params>
struct Message {
  using Callback = std::function<void(Params...)>;

  explicit Message(Callback func) noexcept : m_func(std::move(func)) {}

  void operator()(Params... params) const noexcept { m_func(params...); }

 private:
  Callback m_func;
};

class MessageManager {};

}  // namespace falcon