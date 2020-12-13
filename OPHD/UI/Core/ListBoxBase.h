#pragma once

#include "Control.h"
#include "Slider.h"
#include "../../Constants/UiConstants.h"

#include <NAS2D/Signal.h>
#include <NAS2D/EventHandler.h>
#include <NAS2D/Renderer/Color.h>

#include <string>
#include <vector>
#include <cstddef>


/**
 * Implements a base ListBox control that handles the low-level drawing,
 * input handling and management code while leaving the specific implementation
 * to more derived types.
 * 
 * \note	This is an abstract class -- it will need to be inherited from
 *			in order to be used.
 */
class ListBoxBase : public Control
{
public:
	/**
	 * Callback signal fired whenever the list selection changes.
	 */
	using SelectionChangedCallback = NAS2D::Signals::Signal<>;

	/**
	 * Derived SpecialListBox types can inherit from this struct
	 * for specialized information needed for derived types.
	 */
	struct ListBoxItem
	{
		ListBoxItem() = default;
		ListBoxItem(std::string initialText) : text(initialText) {}
		virtual ~ListBoxItem() = default;

		std::string text;
	};


	ListBoxBase();
	~ListBoxBase() override;

	bool isEmpty() const;
	std::size_t count() const;

	void clear();

	bool isItemSelected() const;
	const ListBoxItem& selected() const;
	std::size_t selectedIndex() const;
	void setSelection(std::size_t selection);
	void clearSelected();

	std::size_t currentHighlight() const;

	SelectionChangedCallback& selectionChanged() { return mSelectionChanged; }

	void update() override = 0;

protected:
	template <typename ItemType, typename... Args>
	void add(Args&&... args) {
		mItems.emplace_back(new ItemType{std::forward<Args>(args)...});
		updateScrollLayout();
	}

	void updateScrollLayout();

	unsigned int item_width() const { return static_cast<unsigned int>(mItemWidth); }
	unsigned int item_height() const { return static_cast<unsigned int>(mItemHeight); }
	void item_height(int);

	unsigned int draw_offset() const { return mScrollOffsetInPixels; }

	void visibilityChanged(bool) override;


	std::vector<ListBoxItem*> mItems; /**< List of Items. Pointers used for polymorphism. */

private:
	void slideChanged(float newPosition);

	virtual void onMouseDown(NAS2D::EventHandler::MouseButton button, int x, int y);
	void onMouseMove(int x, int y, int relX, int relY);
	void onMouseWheel(int x, int y);

	void onSizeChanged() override;


	std::size_t mHighlightIndex = constants::NO_SELECTION;
	std::size_t mSelectedIndex = constants::NO_SELECTION;
	unsigned int mScrollOffsetInPixels = 0;

	int mItemHeight = 1; /**< Height of a ListBoxItem. */
	int mItemWidth = 0; /**< Width of a ListBoxItem. */

	bool mHasFocus = false;

	NAS2D::Color mText = NAS2D::Color::White; /**< Text Color */
	NAS2D::Color mHighlightBg = NAS2D::Color::DarkGreen; /**< Highlight Background color. */
	NAS2D::Color mHighlightText = NAS2D::Color::White; /**< Text Color for an item that is currently highlighted. */

	SelectionChangedCallback mSelectionChanged; /**< Callback for selection changed callback. */
	Slider mSlider; /**< Slider control. */
};
