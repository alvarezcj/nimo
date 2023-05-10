#pragma once
#include "assets/Asset.h"
#include "renderer/Texture.h"
#include <memory>
#include <map>
#include "glm/glm.hpp"

namespace nimo
{
    struct FontGlyph{
        std::unique_ptr<Texture> texture;
        glm::ivec2 size;
        glm::ivec2 bearing;
        int advance;
    };
    class Font : public Asset{
    public:
        Font(const std::string& file);
        ~Font();
        inline AssetType Type() const { return AssetType::Font; }
        inline static AssetType StaticType(){return AssetType::Font;}
        std::map<char, FontGlyph> m_glyphs;
    private:
    };
} // namespace nimo
