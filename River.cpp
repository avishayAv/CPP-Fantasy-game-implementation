#include "River.h"
#include "MtmSet.h"
#include <assert.h>
#include "Group.h"

using namespace mtm;

River::River(const std::string &name): Area(name) {
}

/*
 * Add group to River feature. Functionality :
 * - add group to river
 * - trade with the strongest team in the river (from the same clan or
 *      a friend clan), that trade is valid with her.
 *      if there is no such - do nothing.
 */
void River::groupArrive(const string &group_name, const string &clan,
                        map<string, Clan> &clan_map) {
	std::list<Group> sorted_by_power;
	std::vector<GroupPointer>::iterator ptr = groups.begin();
	for (; ptr != groups.end(); ptr++){
		sorted_by_power.push_back(*(*ptr));
	}
	sorted_by_power.sort();
	sorted_by_power.reverse();
	Area::groupArrive(group_name, clan, clan_map);
	std::map<string, Clan>::iterator current = clan_map.find(clan);
	Clan current_clan = (*current).second;
	for (std::list<Group>::iterator temp = sorted_by_power.begin();
		 temp != sorted_by_power.end(); temp++){
		std::map<string, Clan>::iterator strongest_clan;
		strongest_clan = clan_map.find(temp->getClan());
		if (current_clan.isFriend(strongest_clan->second)){
			if (current->second.getGroup(group_name)->trade(*(strongest_clan->second.getGroup(temp->getName())))) {
				break;
			}
		}
	}
}

