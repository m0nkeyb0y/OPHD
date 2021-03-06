#pragma once

#include "Robot.h"

#include "../../Constants/Strings.h"

class Robodozer: public Robot
{
public:
	Robodozer(): Robot(constants::ROBODOZER, "robots/robodozer.sprite")
	{
	}

	void tileIndex(std::size_t index) { mTileIndex = index; }
	std::size_t tileIndex() const { return mTileIndex; }

	void update() override { updateTask(); }

private:
	std::size_t mTileIndex = 0;
};
