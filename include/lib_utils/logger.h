#pragma once

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>

#include <chrono>
#include <string_view>
#include <utility>

namespace falcon::log {

namespace {

template <typename... T>
void print(const fmt::text_style &ts, std::string_view level,
           std::string_view str_fmt, auto... args) {
  const auto now = fmt::localtime(std::chrono::system_clock::now());

  fmt::print(ts, "[{:%H:%M:%S}] {} ", now, level);
  fmt::print(fmt::runtime(str_fmt), args...);
  fmt::print("\n");
}

[[maybe_unused]] void debug(std::string_view fmt, auto &&...args) {
#if defined(DEBUG)
  falcon::log::print(fg(fmt::color::light_green) | fmt::emphasis::bold,
                     "[DEBUG]", fmt, args...);
#endif
}

void info(std::string_view fmt, auto &&...args) {
  falcon::log::print(fg(fmt::color::cadet_blue) | fmt::emphasis::bold, "[INFO]",
                     fmt, args...);
}

void warn(std::string_view fmt, auto &&...args) {
  falcon::log::print(fg(fmt::color::orange) | fmt::emphasis::bold, "[WARN]",
                     fmt, args...);
}

void error(std::string_view fmt, auto &&...args) {
  falcon::log::print(fg(fmt::color::red) | fmt::emphasis::bold, "[ERROR]", fmt,
                     args...);
}

void fatal(std::string_view fmt, auto &&...args) {
  falcon::log::print(fg(fmt::color::purple) | fmt::emphasis::bold, "[FATAL]",
                     fmt, args...);
}

}  // namespace
}  // namespace falcon::log