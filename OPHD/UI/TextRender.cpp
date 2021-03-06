#include "TextRender.h"

#include "../Cache.h"
#include "../Constants.h"

#include <NAS2D/Resources/Font.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/Utility.h>


void drawLabelAndValue(NAS2D::Point<int> position, const std::string& title, const std::string& text, NAS2D::Color color)
{
	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();

	const NAS2D::Font* FONT = &fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL);
	const NAS2D::Font* FONT_BOLD = &fontCache.load(constants::FONT_PRIMARY_BOLD, constants::FONT_PRIMARY_NORMAL);

	renderer.drawText(*FONT_BOLD, title, position, color);
	position.x += FONT_BOLD->width(title);
	renderer.drawText(*FONT, text, position, color);
}

void drawLabelAndValueLeftJustify(NAS2D::Point<int> position, int labelWidth, const std::string& title, const std::string& text, NAS2D::Color color)
{
	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();

	const NAS2D::Font* FONT = &fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL);
	const NAS2D::Font* FONT_BOLD = &fontCache.load(constants::FONT_PRIMARY_BOLD, constants::FONT_PRIMARY_NORMAL);

	renderer.drawText(*FONT_BOLD, title, position, color);
	position.x += labelWidth;
	renderer.drawText(*FONT, text, position, color);
}

void drawLabelAndValueRightJustify(NAS2D::Point<int> position, int labelWidth, const std::string& title, const std::string& text, NAS2D::Color color)
{
	auto& renderer = NAS2D::Utility<NAS2D::Renderer>::get();

	const NAS2D::Font* FONT = &fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL);
	const NAS2D::Font* FONT_BOLD = &fontCache.load(constants::FONT_PRIMARY_BOLD, constants::FONT_PRIMARY_NORMAL);

	renderer.drawText(*FONT_BOLD, title, position, color);
	position.x += labelWidth - FONT->width(text);
	renderer.drawText(*FONT, text, position, color);
}
