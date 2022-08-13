#pragma once
#include <imgui_freetype.h>

namespace falcon::views {

class View {
 public:
  static ImFontAtlas *getFontAtlas() { return m_fontAtlas; }
  static void setFontAtlas(ImFontAtlas *atlas) { m_fontAtlas = atlas; }

 private:
  static ImFontAtlas *m_fontAtlas;
};
}  // namespace falcon::views