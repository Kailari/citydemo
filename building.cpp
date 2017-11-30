#include "building.h"

Building::Building(const BuildingTemplate &properties, const AGenerator &generator)
        : properties(properties), generator(generator) {

}

void Building::createInstance(GridPos pos, bool useRandomTexture) {
    null
}
