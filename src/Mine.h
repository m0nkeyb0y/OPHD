#pragma once

#include "Common.h"
#include "Constants.h"

#include "NAS2D/NAS2D.h"

#include <bitset>

/**
 * 
 */
class Mine
{
public:
	enum OreType
	{
		ORE_COMMON_METALS,
		ORE_COMMON_MINERALS,
		ORE_RARE_METALS,
		ORE_RARE_MINERALS,
	};

	typedef std::array<int, 4> MineVein;
	typedef std::vector<MineVein> MineVeins;	/**<  */

public:
	Mine();
	Mine(MineProductionRate rate);
	~Mine() {};

	bool active() const;
	void active(bool _b);

	bool exhausted() const;

	MineProductionRate productionRate() { return mProductionRate; }

	int depth() const;
	void increaseDepth();

	int commonMetalsAvailable() const;
	int commonMineralsAvailable() const;
	int rareMetalsAvailable() const;
	int rareMineralsAvailable() const;

	void serialize(NAS2D::Xml::XmlElement* _ti);
	void deserialize(NAS2D::Xml::XmlElement* _ti);

private:
	Mine(const Mine&) = delete;
	Mine& operator=(const Mine&) = delete;

private:
	MineVeins			mVeins;									/**< Ore veins */
	MineProductionRate	mProductionRate = PRODUCTION_RATE_LOW;	/**< Mine's production rate. */
	
	/**
	 * Flags indicating several states for the mine:
	 * 
	 * [0] : Mine Common Metal Ore
	 * [1] : Mine Common Mineral Ore
	 * [2] : Mine Rare Metal Ore
	 * [3] : Mine Rare Mineral Ore
	 * [4] : Mine is active
	 */
	std::bitset<5>		mFlags;									/**< Set of flags. */
};
