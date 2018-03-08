#ifndef EX4_RIVER_H
#define EX4_RIVER_H

#include "Clan.h"
#include "Area.h"

using namespace mtm;

class River : public Area
{

public:
    //Constructor
    River(const std::string& name);

    //Destructor
    ~River() = default;


    /*
     * Add group to River feature. Functionality :
     * - add group to river
     * - trade with the strongest team in the river (from the same clan or
     *      a friend clan), that trade is valid with her.
     *      if there is no such - do nothing.
     */
    void groupArrive(const string& group_name, const string& clan,
                     map<string, Clan>& clan_map) override ;

    /*
     * no special functionality (same as father)
     */
	void groupLeave(const std::string& group_name) {
		Area::groupLeave(group_name);
	}

};
#endif //EX4_RIVER_H
