#pragma once

#include <mbgl/text/glyph_range.hpp>
#include <mbgl/util/font_stack.hpp>
#include <mbgl/util/rect.hpp>
#include <mbgl/util/traits.hpp>
#include <mbgl/util/image.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <map>

namespace mbgl {

typedef char16_t GlyphID;
typedef std::set<GlyphID> GlyphIDs;
    
// Note: this only works for the BMP
GlyphRange getGlyphRange(GlyphID glyph);

struct GlyphMetrics {
    explicit operator bool() const {
        return !(width == 0 && height == 0 && advance == 0);
    }

    // Glyph metrics.
    uint32_t width = 0;
    uint32_t height = 0;
    int32_t left = 0;
    int32_t top = 0;
    uint32_t advance = 0;

};

inline bool operator==(const GlyphMetrics& lhs, const GlyphMetrics& rhs) {
    return lhs.width == rhs.width &&
        lhs.height == rhs.height &&
        lhs.left == rhs.left &&
        lhs.top == rhs.top &&
        lhs.advance == rhs.advance;
}

struct Glyph {
    explicit Glyph() : rect(0, 0, 0, 0), metrics() {}
    explicit Glyph(Rect<uint16_t> rect_, GlyphMetrics metrics_)
        : rect(std::move(rect_)), metrics(std::move(metrics_)) {}

    explicit operator bool() const {
        return metrics || rect.hasArea();
    }

    const Rect<uint16_t> rect;
    const GlyphMetrics metrics;
};

typedef std::map<GlyphID, Glyph> GlyphPositions;

class PositionedGlyph {
public:
    explicit PositionedGlyph(GlyphID glyph_, float x_, float y_, float angle_)
        : glyph(glyph_), x(x_), y(y_), angle(angle_) {}

    GlyphID glyph = 0;
    float x = 0;
    float y = 0;
    float angle = 0;
};

enum class WritingModeType : uint8_t;

class Shaping {
    public:
    explicit Shaping() : top(0), bottom(0), left(0), right(0) {}
    explicit Shaping(float x, float y, WritingModeType writingMode_)
        : top(y), bottom(y), left(x), right(x), writingMode(writingMode_) {}
    std::vector<PositionedGlyph> positionedGlyphs;
    int32_t top;
    int32_t bottom;
    int32_t left;
    int32_t right;
    WritingModeType writingMode;

    explicit operator bool() const { return !positionedGlyphs.empty(); }
};

class SDFGlyph {
public:
    // We're using this value throughout the Mapbox GL ecosystem. If this is different, the glyphs
    // also need to be reencoded.
    static constexpr const uint8_t borderSize = 3;

    GlyphID id = 0;

    // A signed distance field of the glyph with a border (see above).
    AlphaImage bitmap;

    // Glyph metrics
    GlyphMetrics metrics;
};

enum class WritingModeType : uint8_t {
    None = 0,
    Horizontal = 1 << 0,
    Vertical = 1 << 1,
};

constexpr WritingModeType operator|(WritingModeType a, WritingModeType b) {
    return WritingModeType(mbgl::underlying_type(a) | mbgl::underlying_type(b));
}

constexpr WritingModeType& operator|=(WritingModeType& a, WritingModeType b) {
    return (a = a | b);
}

constexpr bool operator&(WritingModeType lhs, WritingModeType rhs) {
    return mbgl::underlying_type(lhs) & mbgl::underlying_type(rhs);
}

constexpr WritingModeType& operator&=(WritingModeType& lhs, WritingModeType rhs) {
    return (lhs = WritingModeType(mbgl::underlying_type(lhs) & mbgl::underlying_type(rhs)));
}

constexpr WritingModeType operator~(WritingModeType value) {
    return WritingModeType(~mbgl::underlying_type(value));
}

// TODO: Find a good place for these to live
typedef std::map<FontStack,GlyphIDs> GlyphDependencies;
typedef std::map<FontStack,GlyphRangeSet> GlyphRangeDependencies;
typedef std::map<FontStack,GlyphPositions> GlyphPositionMap;


} // end namespace mbgl
