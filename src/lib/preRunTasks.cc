#include <fonts/Font_opensans.h>
#include <fonts/IconsCodicon.h>
#include <fonts/IconsFontAwesome6.h>
#include <imgui.h>
#include <imgui_freetype.h>
#include <preRunTasks.h>
#include <views.h>

namespace falcon::preTasks {

bool loadFonts() {
  auto fonts = IM_NEW(ImFontAtlas)();

  ImFontConfig cfg = {};

  ImVector<ImWchar> ranges;

  ImFontGlyphRangesBuilder glyphRangesBuilder;
  glyphRangesBuilder.AddRanges(fonts->GetGlyphRangesDefault());
  glyphRangesBuilder.BuildRanges(&ranges);

  ImWchar fontAwesomeRange[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
  ImWchar codiconsRange[] = {ICON_MIN_VS, ICON_MAX_VS, 0};
  ImWchar opensansRange[] = {0x0100, 0xFFF0, 0};

  fonts->Clear();
  cfg.OversampleH = cfg.OversampleV = 1;
  cfg.PixelSnapH = true;
  cfg.SizePixels = fontSize;
  fonts->AddFontDefault(&cfg);

  cfg.MergeMode = true;

  fonts->AddFontFromMemoryCompressedTTF(fa_compressed_data, fa_compressed_size,
                                        fontSize, &cfg, fontAwesomeRange);
  fonts->AddFontFromMemoryCompressedTTF(codicons_compressed_data, codicons_compressed_size,
                                        fontSize, &cfg, codiconsRange);
  fonts->AddFontFromMemoryCompressedTTF(opensans_compressed_data, opensans_compressed_size,
                                        fontSize, &cfg, opensansRange);

  ImGuiFreeType::BuildFontAtlas(fonts);

  views::View::setFontAtlas(fonts);

  return true;
};

std::vector<tasks::Task> tasks() {
  return {
      {"Load fonts", loadFonts},
  };
}
};  // namespace falcon::preTasks
