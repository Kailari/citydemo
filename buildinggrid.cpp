#include "buildinggrid.h"
#include "util.h"
#include "building.h"

bool BuildingGrid::isFree(int32 x, int32 z) {
    return x >= 0 && x < sizeX
           && z >= 0 && z < sizeZ
           && grid[z][x] < 0;
}

void BuildingGrid::refill() {
    // Clear existing building instances
    for (Building* building : buildings) {
        building->clearInstances();
    }

    // Clear the grid
    for (int32 x = 0; x < sizeX; x++) {
        for (int32 z = 0; z < sizeZ; z++) {
            grid[z][x] = -1;
        }
    }

    // Refill the grid
    for (int32 x = 0; x < sizeX; x++) {
        for (int32 z = 0; z < sizeZ; z++) {
            int32 index = Util::randRange(buildings.size());
            add(x, z, index);
        }
    }
}

void BuildingGrid::add(int32 x, int32 z, int32 buildingIndex) {
    Building* building = buildings[buildingIndex];

    // Check that the building fits
    for (int32 xx = x; xx < x + building->getSizeX(); xx++) {
        for (int32 zz = z; zz < z + building->getSizeZ(); zz++) {
            if (!isFree(xx, zz)) {
                return;
            }
        }
    }

    // Fill the grid
    for (int32 xx = x; xx < x + building->getSizeX(); xx++) {
        for (int32 zz = z; zz < z + building->getSizeZ(); zz++) {
            grid[zz][xx] = buildingIndex;
        }
    }

    // Instantiate
    // TODO: Properly implement building margins
    building->createInstance({x * 2, z * 2}, false);
}

int32 BuildingGrid::registerBuilding(Building* building) {
    int32 index = buildings.size();
    buildings.push_back(building);
    return index;
}
