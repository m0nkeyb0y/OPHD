#include "UIContainer.h"

#include "RadioButton.h"
#include "../../Common.h"

#include <NAS2D/Utility.h>

#include <algorithm>
#include <iostream>
#include <stdexcept>


using namespace NAS2D;


UIContainer::UIContainer()
{
	Utility<EventHandler>::get().mouseButtonDown().connect(this, &UIContainer::onMouseDown);
}


UIContainer::~UIContainer()
{
	Utility<EventHandler>::get().mouseButtonDown().disconnect(this, &UIContainer::onMouseDown);
}


void UIContainer::add(Control& control, NAS2D::Vector<int> offset)
{
	if (std::find(mControls.begin(), mControls.end(), &control) != mControls.end())
	{
		throw std::runtime_error("UIContainer::add(): Duplicate control");
	}

	if (mControls.size() > 0) { mControls.back()->hasFocus(false); }
	mControls.push_back(&control);

	control.position(mRect.startPoint() + offset);
	control.visible(visible());
	control.hasFocus(true);
	if (auto* asRadioButton = dynamic_cast<RadioButton*>(&control))
	{
		asRadioButton->parentContainer(this);
	}
}


/**
 * Drops all controls.
 */
void UIContainer::clear()
{
	mControls.clear();
}


void UIContainer::bringToFront(Control* control)
{
	auto control_iterator = std::find(mControls.begin(), mControls.end(), control);
	if (control_iterator == mControls.end())
	{
		std::cout << "UIContainer::bringToFront(): Control is not managed by this container." << std::endl; // debug aid, can be pulled in release modes.
		return;
	}

	mControls.back()->hasFocus(false);

	mControls.erase(control_iterator);
	mControls.push_back(control);
	control->hasFocus(true);
}


void UIContainer::visibilityChanged(bool visible)
{
	for (auto control : mControls) { control->visible(visible); }
}


void UIContainer::positionChanged(int dX, int dY)
{
	Control::positionChanged(dX, dY);

	for (auto control : mControls)
	{
		control->position(control->position() + NAS2D::Vector{dX, dY});
	}
}


void UIContainer::onMouseDown(EventHandler::MouseButton /*button*/, int x, int y)
{
	if (!visible()) { return; }

	Control* control = nullptr;
	for (auto it = mControls.rbegin(); it != mControls.rend(); ++it)
	{
		control = (*it);
		if (control->visible() && control->rect().contains(NAS2D::Point{x, y}))
		{
			if (control == mControls.back()) { return; }
			bringToFront(control);
			return;
		}
	}
}


/**
 * Updates all Controls in the UIContainer.
 * 
 * \note	This function can be overridden in derived types
 *			but if done, don't forget to update all contained
 *			Controls.
 */
void UIContainer::update()
{
	if (!visible()) { return; }
	for (auto control : mControls) { control->update(); /*if (control->hasFocus()) { Utility<Renderer>::get().drawBox(control->rect(), 255, 0, 255); }*/ }
}


std::vector<Control*> UIContainer::controls() const {
	return mControls;
}
