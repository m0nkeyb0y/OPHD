#pragma once

#include "Core/Control.h"

#include <NAS2D/Resources/Font.h>
#include <NAS2D/Renderer/RectangleSkin.h>


class Population;


class PopulationPanel: public Control
{
public:
	PopulationPanel();

	void population(Population* pop) { mPopulation = pop; }
	void morale(int* m) { mMorale = m; }
	void old_morale(int* m) { mPreviousMorale = m; }

	void residential_capacity(int m) { mResidentialCapacity = m; }

	/**
	 * \fixme	This class/function is use to store residence capacity
	 *			by the MapViewState. Probably not an appropriate place.
	 */
	int capacity() const { return mCapacity; }

	void update() override;

private:
	const NAS2D::Font& mFont;
	const NAS2D::Image& mIcons;
	NAS2D::RectangleSkin mSkin;

	Population* mPopulation = nullptr;

	int mResidentialCapacity = 0;

	int* mMorale = nullptr;
	int* mPreviousMorale = nullptr;

	int mCapacity = 0;
};
