// ==================================================================================
// = NAS2D+GUI
// = Copyright © 2008 - 2020, Leeor Dicker
// ==================================================================================
// = This file is part of the NAS2D+GUI library.
// ==================================================================================

#include "TextField.h"

#include "../../Cache.h"
#include "../../Constants.h"

#include <NAS2D/Utility.h>
#include <NAS2D/Renderer/Renderer.h>
#include <NAS2D/MathUtils.h>

#include <locale>


using namespace NAS2D;


namespace {
	constexpr int fieldPadding = 4;
	constexpr int cursorBlinkDelay = 250;
}


TextField::TextField() :
	mFont{fontCache.load(constants::FONT_PRIMARY, constants::FONT_PRIMARY_NORMAL)},
	mSkinNormal{
		imageCache.load("ui/skin/textbox_top_left.png"),
		imageCache.load("ui/skin/textbox_top_middle.png"),
		imageCache.load("ui/skin/textbox_top_right.png"),
		imageCache.load("ui/skin/textbox_middle_left.png"),
		imageCache.load("ui/skin/textbox_middle_middle.png"),
		imageCache.load("ui/skin/textbox_middle_right.png"),
		imageCache.load("ui/skin/textbox_bottom_left.png"),
		imageCache.load("ui/skin/textbox_bottom_middle.png"),
		imageCache.load("ui/skin/textbox_bottom_right.png")
	},
	mSkinFocus{
		imageCache.load("ui/skin/textbox_top_left_highlight.png"),
		imageCache.load("ui/skin/textbox_top_middle_highlight.png"),
		imageCache.load("ui/skin/textbox_top_right_highlight.png"),
		imageCache.load("ui/skin/textbox_middle_left_highlight.png"),
		imageCache.load("ui/skin/textbox_middle_middle_highlight.png"),
		imageCache.load("ui/skin/textbox_middle_right_highlight.png"),
		imageCache.load("ui/skin/textbox_bottom_left_highlight.png"),
		imageCache.load("ui/skin/textbox_bottom_middle_highlight.png"),
		imageCache.load("ui/skin/textbox_bottom_right_highlight.png")
	}
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &TextField::onMouseDown);
	Utility<EventHandler>::get().keyDown().connect(this, &TextField::onKeyDown);
	Utility<EventHandler>::get().textInput().connect(this, &TextField::onTextInput);

	hasFocus(true);
	Utility<EventHandler>::get().textInputMode(true);

	height(mFont.height() + fieldPadding * 2);
}


TextField::~TextField()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &TextField::onMouseDown);
	Utility<EventHandler>::get().keyDown().disconnect(this, &TextField::onKeyDown);
	Utility<EventHandler>::get().textInput().disconnect(this, &TextField::onTextInput);
}


void TextField::resetCursorPosition()
{
	mCursorPosition = 0;
}


/**
 * When set, will only allow numbers to be entered into the TextField.
 * 
 * \param _b True or False.
 */
void TextField::numbers_only(bool _b)
{
	mNumbersOnly = _b;
}


/**
 * Sets the maximum number of characters allowed in the text field.
 * 
 * \param	count	Number of characters allowed in the field.
 * 
 * \note	Calling this with \c 0 will clear character limit.
 */
void TextField::maxCharacters(std::size_t count)
{
	mMaxCharacters = count;
}


int TextField::textAreaWidth() const
{
	return mRect.width - fieldPadding * 2;
}


void TextField::editable(bool editable)
{
	mEditable = editable;
}


bool TextField::editable() const
{
	return mEditable;
}


/**
 * Sets border visibility.
 */
void TextField::border(BorderVisibility visibility)
{
	mBorderVisibility = visibility;
}


/**
 * Handles text input events.
 */
void TextField::onTextInput(const std::string& newTextInput)
{
	if (!hasFocus() || !visible() || !editable() || newTextInput.empty()) { return; }

	if (mMaxCharacters > 0 && text().length() == mMaxCharacters) { return; }

	auto prvLen = text().length();

	std::locale locale;
	if (mNumbersOnly && !std::isdigit(newTextInput[0], locale)) { return; }

	mText = mText.insert(mCursorPosition, newTextInput);

	if (text().length() - prvLen != 0u)
	{
		onTextChanged();
		mCursorPosition++;
	}
}


void TextField::onKeyDown(EventHandler::KeyCode key, EventHandler::KeyModifier /*mod*/, bool /*repeat*/)
{
	if (!hasFocus() || !editable() || !visible()) { return; }

	switch(key)
	{
		// COMMAND KEYS
		case EventHandler::KeyCode::KEY_BACKSPACE:
			if (!text().empty() && mCursorPosition > 0)
			{
				mCursorPosition--;
				mText.erase(mCursorPosition, 1);
				onTextChanged();
			}
			break;

		case EventHandler::KeyCode::KEY_HOME:
			mCursorPosition = 0;
			break;

		case EventHandler::KeyCode::KEY_END:
			mCursorPosition = text().length();
			break;

		case EventHandler::KeyCode::KEY_DELETE:
			if (text().length() > 0)
			{
				mText = mText.erase(mCursorPosition, 1);
				onTextChanged();
			}
			break;

		// ARROW KEYS
		case EventHandler::KeyCode::KEY_LEFT:
			if (mCursorPosition > 0)
				--mCursorPosition;
			break;

		case EventHandler::KeyCode::KEY_RIGHT:
			if (mCursorPosition < text().length())
				++mCursorPosition;
			break;

		// KEYPAD ARROWS
		case EventHandler::KeyCode::KEY_KP4:
			if ((mCursorPosition > 0) && !Utility<EventHandler>::get().query_numlock())
				--mCursorPosition;
			break;

		case EventHandler::KeyCode::KEY_KP6:
			if ((mCursorPosition < text().length()) && !Utility<EventHandler>::get().query_numlock())
				++mCursorPosition;
			break;

		// IGNORE ENTER/RETURN KEY
		case EventHandler::KeyCode::KEY_ENTER:
		case EventHandler::KeyCode::KEY_KP_ENTER:
			break;

		// REGULAR KEYS
		default:
			break;
	}
}


/**
 * Mouse down even handler.
 */
void TextField::onMouseDown(EventHandler::MouseButton /*button*/, int x, int y)
{
	hasFocus(mRect.contains(Point{x, y})); // This is a very useful check, should probably include this in all controls.

	if (!enabled() || !visible() || !hasFocus()) { return; }

	int relativePosition = x - mRect.x;

	// If the click occured past the width of the text, we can immediatly
	// set the position to the end and move on.
	if (mFont.width(text()) < relativePosition)
	{
		mCursorPosition = text().size();
		return;
	}


	// Figure out where the click occured within the visible string.
	std::size_t i = 0;
	const auto scrollOffset = static_cast<std::size_t>(mScrollOffset);
	while(i <= text().size() - scrollOffset)
	{
		std::string cmpStr = text().substr(scrollOffset, i);
		int strLen = mFont.width(cmpStr);
		if (strLen > relativePosition)
		{
			mCursorPosition = i - 1;
			break;
		}

		i++;
	}
}


/**
 * Draws the insertion point cursor.
 */
void TextField::drawCursor()
{
	if (hasFocus() && editable())
	{
		if (mShowCursor)
		{
			// updateCursor() should be called only on events relating to the cursor so this is temporary.
			updateCursor();
			auto& renderer = Utility<Renderer>::get();
			const auto startPosition = NAS2D::Point{mCursorX, mRect.y + fieldPadding};
			const auto endPosition = NAS2D::Point{mCursorX, mRect.y + mRect.height - fieldPadding - 1};
			renderer.drawLine(startPosition + NAS2D::Vector{1, 1}, endPosition + NAS2D::Vector{1, 1}, NAS2D::Color::Black);
			renderer.drawLine(startPosition, endPosition, NAS2D::Color::White);
		}

		if (mCursorTimer.accumulator() > cursorBlinkDelay)
		{
			mCursorTimer.reset();
			mShowCursor = !mShowCursor;
		}
	}
}


void TextField::updateCursor()
{
	int cursorX = mFont.width(text().substr(0, mCursorPosition));

	// Check if cursor is after visible area
	if (mScrollOffset <= cursorX - textAreaWidth())
	{
		mScrollOffset = cursorX - textAreaWidth();
	}

	// Check if cursor is before visible area
	if (mScrollOffset >= cursorX)
	{
		mScrollOffset = cursorX - textAreaWidth() / 2;
	}

	if (mScrollOffset < 0)
	{
		mScrollOffset = 0;
	}

	mCursorX = mRect.x + fieldPadding + cursorX - mScrollOffset;
}


void TextField::update()
{
	if (!visible()) { return; }

	auto& renderer = Utility<Renderer>::get();

	const auto showFocused = hasFocus() && editable();
	const auto& skin = showFocused ? mSkinFocus : mSkinNormal;
	skin.draw(renderer, mRect);

	if (highlight()) { renderer.drawBox(mRect, NAS2D::Color::Yellow); }

	drawCursor();

	renderer.drawText(mFont, text(), position() + NAS2D::Vector{fieldPadding, fieldPadding}, NAS2D::Color::White);
}
