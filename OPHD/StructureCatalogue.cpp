// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "StructureCatalogue.h"


//vector<ResourcePool> StructureCatalogue::mStructureCostTable;
std::array<ResourcePool, StructureID::SID_COUNT> StructureCatalogue::mStructureCostTable;
std::array<ResourcePool, StructureID::SID_COUNT> StructureCatalogue::mStructureRecycleValueTable;
std::array<PopulationRequirements, StructureID::SID_COUNT> StructureCatalogue::mPopulationRequirementsTable = {};

/**	Default recycle value. Currently set at 90% but this should probably be
 *	lowered for actual gameplay with modifiers to improve efficiency. */
const float DEFAULT_RECYCLE_VALUE = 0.9f;


/**
 * Gets a new Structure object given a StructureID.
 * 
 * \param	type	A valid StructureID value.
 * 
 * \return	Pointer to a newly constructed Structure or
 *			\c nullptr if structure type unsupported/invalid.
 */
Structure* StructureCatalogue::get(StructureID type)
{
	Structure* structure = nullptr;

	// This seems like a naive approach... I usually see these implemented as the base
	// object type has a static function that is used as an interface to instantiate
	// derived types.
	switch (type)
	{
		case StructureID::SID_NONE:
			break;

		case StructureID::SID_AGRIDOME:
			structure = new Agridome();
			break;

		case StructureID::SID_AIR_SHAFT:
			structure = new AirShaft();
			break;

		case StructureID::SID_CARGO_LANDER: // only here for loading games
			structure = new CargoLander(nullptr);
			break;

		case StructureID::SID_CHAP:
			structure = new CHAP();
			break;

		case StructureID::SID_COLONIST_LANDER: // only here for loading games
			structure = new ColonistLander(nullptr);
			break;

		case StructureID::SID_COMMAND_CENTER:
			structure = new CommandCenter();
			break;

		case StructureID::SID_COMMERCIAL:
			structure = new Commercial();
			break;

		case StructureID::SID_COMM_TOWER:
			structure = new CommTower();
			break;

		case StructureID::SID_FUSION_REACTOR:
			structure = new FusionReactor();
			break;

		case StructureID::SID_HOT_LABORATORY:
			structure = new HotLaboratory();
			break;

		case StructureID::SID_LABORATORY:
			structure = new Laboratory();
			break;

		case StructureID::SID_MEDICAL_CENTER:
			structure = new MedicalCenter();
			break;

		case StructureID::SID_MINE_FACILITY: // only here for loading games
			structure = new MineFacility(nullptr);
			break;

		case StructureID::SID_MINE_SHAFT: // only here for loading games
			structure = new MineShaft();
			break;

		case StructureID::SID_NURSERY:
			structure = new Nursery();
			break;

		case StructureID::SID_PARK:
			structure = new Park();
			break;

		case StructureID::SID_SURFACE_POLICE:
			structure = new SurfacePolice();
			break;

		case StructureID::SID_UNDERGROUND_POLICE:
			structure = new UndergroundPolice();
			break;

		case StructureID::SID_RECREATION_CENTER:
			structure = new RecreationCenter();
			break;

		case StructureID::SID_RED_LIGHT_DISTRICT:
			structure = new RedLightDistrict();
			break;

		case StructureID::SID_RESIDENCE:
			structure = new Residence();
			break;

		case StructureID::SID_ROBOT_COMMAND:
			structure = new RobotCommand();
			break;

		case StructureID::SID_SEED_FACTORY:
			structure = new SeedFactory();
			break;

		case StructureID::SID_SEED_LANDER: // only here for loading games
			structure = new SeedLander(0, 0);
			break;

		case StructureID::SID_SEED_POWER:
			structure = new SeedPower();
			break;

		case StructureID::SID_SEED_SMELTER:
			structure = new SeedSmelter();
			break;

		case StructureID::SID_SMELTER:
			structure = new Smelter();
			break;

		case StructureID::SID_SOLAR_PANEL1:
			structure = new SolarPanelArray();
			break;

		case StructureID::SID_SOLAR_PLANT:
			structure = new SolarPlant();
			break;

		case StructureID::SID_STORAGE_TANKS:
			structure = new StorageTanks();
			break;

		case StructureID::SID_SURFACE_FACTORY:
			structure = new SurfaceFactory();
			break;

		case StructureID::SID_UNDERGROUND_FACTORY:
			structure = new UndergroundFactory();
			break;

		case StructureID::SID_UNIVERSITY:
			structure = new University();
			break;

		case StructureID::SID_WAREHOUSE:
			structure = new Warehouse();
			break;

		default:
			std::cout << "StructureCatalogue::get(): Unsupported structure type called." << std::endl;
			break;
	}

	if (structure)
	{
		structure->setPopulationRequirements(StructureCatalogue::populationRequirements(type));
	}

	return structure;
}


/**
 * Gets the population required to operate a given Structure.
 * 
 * \param	type	A valid StructureID value.
 */
const PopulationRequirements& StructureCatalogue::populationRequirements(StructureID type)
{
	return mPopulationRequirementsTable[type];
}


/**
 * Gets the cost in resources required to build a given Structure.
 * 
 * \param	type	A valid StructureID value.
 */
const ResourcePool& StructureCatalogue::costToBuild(StructureID type)
{
	return mStructureCostTable[type];
}


/**
 * Gets the recycling value of a specified structure type.
 * 
 * \param	type	A valid StructureID value.
 */
const ResourcePool& StructureCatalogue::recyclingValue(StructureID type)
{
	return mStructureRecycleValueTable[type];
}


/**
 * Initializes StructureCatalogue and builds the requirements tables.
 */
void StructureCatalogue::init()
{
	buildCostTable();
	buildRecycleValueTable();
	buildPopulationRequirementsTable();
}


/**
 * Indicates that the source ResourcePool has enough resources to accommodate
 * the resource requirements of the specificed structure.
 */
bool StructureCatalogue::canBuild(const ResourcePool& source, StructureID type)
{
	ResourcePool resourcePool = StructureCatalogue::costToBuild(type);

	if (source.commonMetals() < resourcePool.commonMetals() || source.commonMinerals() < resourcePool.commonMinerals() ||
		source.rareMetals() < resourcePool.rareMetals() || source.rareMinerals() < resourcePool.rareMinerals())
	{
		return false;
	}

	return true;
}


/**
 * Fills out the build costs for all structures.
 */
void StructureCatalogue::buildCostTable()
{
	// RESOURCES: COMM_MET_ORE, COMM_MIN_ORE, RARE_MET_ORE, RARE_MIN_ORE, COMM_MET, COMM_MIN, RARE_MET, RARE_MIN
	mStructureCostTable[StructureID::SID_AGRIDOME] = ResourcePool(0, 0, 0, 0, 20, 10, 5, 0, 0, 0);
	mStructureCostTable[StructureID::SID_CHAP] = ResourcePool(0, 0, 0, 0, 50, 10, 20, 5, 0, 0);
	mStructureCostTable[StructureID::SID_COMMAND_CENTER] = ResourcePool(0, 0, 0, 0, 100, 75, 65, 35, 0, 0);
	mStructureCostTable[StructureID::SID_COMMERCIAL] = ResourcePool(0, 0, 0, 0, 25, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_COMM_TOWER] = ResourcePool(0, 0, 0, 0, 30, 10, 5, 5, 0, 0);
	mStructureCostTable[StructureID::SID_FUSION_REACTOR] = ResourcePool(0, 0, 0, 0, 75, 25, 50, 30, 0, 0);
	mStructureCostTable[StructureID::SID_HOT_LABORATORY] = ResourcePool(0, 0, 0, 0, 45, 10, 15, 5, 0, 0);
	mStructureCostTable[StructureID::SID_LABORATORY] = ResourcePool(0, 0, 0, 0, 20, 10, 10, 5, 0, 0);
	mStructureCostTable[StructureID::SID_MEDICAL_CENTER] = ResourcePool(0, 0, 0, 0, 25, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_NURSERY] = ResourcePool(0, 0, 0, 0, 20, 10, 5, 0, 0, 0);
	mStructureCostTable[StructureID::SID_PARK] = ResourcePool(0, 0, 0, 0, 25, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_SURFACE_POLICE] = ResourcePool(0, 0, 0, 0, 25, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_UNDERGROUND_POLICE] = ResourcePool(0, 0, 0, 0, 25, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_RECREATION_CENTER] = ResourcePool(0, 0, 0, 0, 25, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_RED_LIGHT_DISTRICT] = ResourcePool(0, 0, 0, 0, 20, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_RESIDENCE] = ResourcePool(0, 0, 0, 0, 25, 5, 2, 0, 0, 0);
	mStructureCostTable[StructureID::SID_ROBOT_COMMAND] = ResourcePool(0, 0, 0, 0, 75, 50, 45, 25, 0, 0);
	mStructureCostTable[StructureID::SID_SMELTER] = ResourcePool(0, 0, 0, 0, 30, 20, 10, 5, 0, 0);
	mStructureCostTable[StructureID::SID_SOLAR_PANEL1] = ResourcePool(0, 0, 0, 0, 10, 20, 5, 5, 0, 0);
	mStructureCostTable[StructureID::SID_SOLAR_PLANT] = ResourcePool(0, 0, 0, 0, 50, 25, 50, 20, 0, 0);
	mStructureCostTable[StructureID::SID_STORAGE_TANKS] = ResourcePool(0, 0, 0, 0, 15, 5, 6, 1, 0, 0);
	mStructureCostTable[StructureID::SID_SURFACE_FACTORY] = ResourcePool(0, 0, 0, 0, 20, 10, 10, 5, 0, 0);
	mStructureCostTable[StructureID::SID_UNDERGROUND_FACTORY] = ResourcePool(0, 0, 0, 0, 20, 10, 10, 5, 0, 0);
	mStructureCostTable[StructureID::SID_UNIVERSITY] = ResourcePool(0, 0, 0, 0, 20, 10, 10, 5, 0, 0);
	mStructureCostTable[StructureID::SID_WAREHOUSE] = ResourcePool(0, 0, 0, 0, 15, 5, 6, 1, 0, 0);
}


/**
 * Fills out the recycle value for all structures.
 */
void StructureCatalogue::buildRecycleValueTable()
{
	for (std::size_t i = 0; i < StructureID::SID_COUNT; ++i)
	{
		mStructureRecycleValueTable[static_cast<StructureID>(i)] = recycleValue(static_cast<StructureID>(i), DEFAULT_RECYCLE_VALUE);
	}

	// Set recycling values for landers and automatically built structures.
	// RESOURCES: COMM_MET_ORE, COMM_MIN_ORE, RARE_MET_ORE, RARE_MIN_ORE, COMM_MET, COMM_MIN, RARE_MET, RARE_MIN
	mStructureRecycleValueTable[StructureID::SID_MINE_FACILITY] = ResourcePool(0, 0, 0, 0, 15, 10, 5, 5, 0, 0);
	mStructureRecycleValueTable[StructureID::SID_CARGO_LANDER] = ResourcePool(0, 0, 0, 0, 15, 10, 5, 5, 0, 0);
	mStructureRecycleValueTable[StructureID::SID_COLONIST_LANDER] = ResourcePool(0, 0, 0, 0, 15, 10, 5, 5, 0, 0);
	mStructureRecycleValueTable[StructureID::SID_SEED_LANDER] = ResourcePool(0, 0, 0, 0, 10, 5, 5, 5, 0, 0);
	mStructureRecycleValueTable[StructureID::SID_SEED_FACTORY] = ResourcePool(0, 0, 0, 0, 15, 10, 5, 5, 0, 0);
	mStructureRecycleValueTable[StructureID::SID_SEED_POWER] = ResourcePool(0, 0, 0, 0, 15, 10, 5, 5, 0, 0);
	mStructureRecycleValueTable[StructureID::SID_SEED_SMELTER] = ResourcePool(0, 0, 0, 0, 15, 10, 5, 5, 0, 0);

}


/**
 * Fills out the population requirements for all structures.
 */
void StructureCatalogue::buildPopulationRequirementsTable()
{
	// WORKERS, SCIENTISTS
	mPopulationRequirementsTable[StructureID::SID_AGRIDOME] = { 1, 0 };
	mPopulationRequirementsTable[StructureID::SID_CHAP] = { 2, 0 };
	mPopulationRequirementsTable[StructureID::SID_COMMAND_CENTER] = { 3, 0 };
	mPopulationRequirementsTable[StructureID::SID_COMMERCIAL] = { 1, 0 };
	mPopulationRequirementsTable[StructureID::SID_FUSION_REACTOR] = { 1, 2 };
	mPopulationRequirementsTable[StructureID::SID_HOT_LABORATORY] = { 1, 5 };
	mPopulationRequirementsTable[StructureID::SID_LABORATORY] = { 1, 5 };
	mPopulationRequirementsTable[StructureID::SID_MEDICAL_CENTER] = { 1, 2 };
	mPopulationRequirementsTable[StructureID::SID_NURSERY] = { 1, 1 };
	mPopulationRequirementsTable[StructureID::SID_PARK] = { 1, 0 };
	mPopulationRequirementsTable[StructureID::SID_SURFACE_POLICE] = { 5, 0 };
	mPopulationRequirementsTable[StructureID::SID_UNDERGROUND_POLICE] = { 5, 0 };
	mPopulationRequirementsTable[StructureID::SID_RECREATION_CENTER] = { 2, 0 };
	mPopulationRequirementsTable[StructureID::SID_RED_LIGHT_DISTRICT] = { 2, 0 };
	mPopulationRequirementsTable[StructureID::SID_ROBOT_COMMAND] = { 4, 0 };
	mPopulationRequirementsTable[StructureID::SID_SEED_FACTORY] = { 2, 0 };
	mPopulationRequirementsTable[StructureID::SID_SEED_SMELTER] = { 2, 0 };
	mPopulationRequirementsTable[StructureID::SID_SMELTER] = { 4, 0 };
	mPopulationRequirementsTable[StructureID::SID_SOLAR_PANEL1] = { 1, 0 };
	mPopulationRequirementsTable[StructureID::SID_SURFACE_FACTORY] = { 4, 0 };
	mPopulationRequirementsTable[StructureID::SID_UNDERGROUND_FACTORY] = { 2, 0 };
	mPopulationRequirementsTable[StructureID::SID_UNIVERSITY] = { 1, 3 };
	mPopulationRequirementsTable[StructureID::SID_WAREHOUSE] = { 1, 0 };
}


/**
 * Calculates the base recycling value of a given structure.
 * 
 * \param	type	A valid StructureID value.
 */
ResourcePool StructureCatalogue::recycleValue(StructureID type, float percent)
{
	if (mStructureCostTable.empty())
	{
		throw std::runtime_error("StructureCatalogue::recycleValue() called before StructureCatalogue::buildCostTable().");
	}

	ResourcePool resourcePool = mStructureCostTable[type];

	/** Truncation of value from float to int cast is intended and desired behavior. */
	return ResourcePool(static_cast<int>(resourcePool.commonMetalsOre() * percent),
						static_cast<int>(resourcePool.commonMineralsOre() * percent),
						static_cast<int>(resourcePool.rareMetalsOre() * percent),
						static_cast<int>(resourcePool.rareMineralsOre() * percent),
						static_cast<int>(resourcePool.commonMetals() * percent),
						static_cast<int>(resourcePool.commonMinerals() * percent),
						static_cast<int>(resourcePool.rareMetals() * percent),
						static_cast<int>(resourcePool.rareMinerals() * percent),
						static_cast<int>(resourcePool.food() * percent),
						static_cast<int>(resourcePool.energy() * percent));
}