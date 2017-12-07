#ifndef DEMO001_BUILDINGGRID_H
#define DEMO001_BUILDINGGRID_H

#include <vector>
#include "typedef.h"

class Building;

class BuildingGrid {
public:
    static const int32 sizeX = 10;
    static const int32 sizeZ = 10;

    /**
     * Checks if given position on grid is free
     *
     * @param x x-coordinate to check
     * @param z y-coordinate to check
     * @return true if position is free, false otherwise.
     */
    bool isFree(int32 x, int32 z);

    /**
     * Registers a new building to be used with the grid
     * @param building
     * @return The index of the building just registered.
     */
    int32 registerBuilding(Building* building);

    /**
     * Adds a building at given coordinate.
     *
     * @param x x-coordinate to add the building to
     * @param z z-coordinate to add the building to
     * @param buildingIndex Index of the building to add.
     */
    void add(int32 x, int32 z, int32 buildingIndex);

    /**
     * Re-fills the grid with buildings.
     */
    void refill();
private:
    std::vector<Building*> buildings;
    int32 grid[sizeX][sizeZ];
};

#endif //DEMO001_BUILDINGGRID_H
