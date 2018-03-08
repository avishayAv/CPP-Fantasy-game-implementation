#include "Plain.h"
#include <iostream>
#include "Clan.h"
#include <assert.h>

using namespace mtm;

namespace plainHelper   {
    //Helper function for groupArrive
    //JOB : checking if a group name already exist.
    bool isStringExist(const string &current_name, map<string, Clan> &clan_map){
        for (std::map<string,Clan>::iterator it=clan_map.begin();
             it!=clan_map.end(); it++)  {
            if (it->second.doesContain(current_name))   {
                return true;
            }
        }
        return false;
    }
}

Plain::Plain(const std::string &name): Area(name) {

}

/*
 * Add group to Plain feature. Functionality :
 * - Add group to plain
 * - If group is less than 1/3 than clan -
 *      group should unite with the strongest team from the same clan in
 *      the plain, assuming that there isn't more than 1/3 from clan in both
 *      together. if there isn't - do nothing else.
 * - if Group has : more than 1/3 people of the clan AND at least 10 people,
 *      group should be divided.
 *      name of the new group should be _<OriginalGroupName>x,
 *      x - number bigger/equal 2 that promises there is no such group with
 *      this name.
 */
void Plain::groupArrive(const string &group_name, const string &clan,
                        map<string, Clan> &clan_map) {
    std::map<string, Clan>::iterator current = clan_map.find(clan);
    Area::groupArrive(group_name, clan, clan_map);
    GroupPointer new_group = (*current).second.getGroup(group_name);
    int group_size = new_group->getSize();
    Clan current_clan = (*current).second;
    int clan_size = current_clan.getSize();
    string current_clan_string = current->first;
	if ((group_size * 3) < clan_size) {
		MtmSet<string> all_groups = this->getGroupsNames();
		std::list<Group> sorted_by_power;
		std::vector<GroupPointer>::iterator ptr = groups.begin();
		for (; ptr != groups.end(); ptr++) {
			sorted_by_power.push_back(*(*ptr));
		}
		sorted_by_power.sort();
		sorted_by_power.reverse();
		for (std::list<Group>::iterator temp = sorted_by_power.begin();
             temp != sorted_by_power.end(); temp++) {
			std::map<string, Clan>::iterator strongest_clan =
                    clan_map.find(temp->getClan());
			if (current_clan_string == strongest_clan->first) {
				if ((((temp->getSize() + new_group->getSize()) * 3) <
                        current_clan.getSize()) &&
				(strongest_clan->second.getGroup(temp->getName())->unite(*new_group, clan_size))) {
					Area::groupLeave("");
					break;
				}
			}
		}
	}
    else    {
        if (group_size >= 10)   {
            int i = 2;
            string new_string = new_group->getName() + "_" + std::to_string(i);
            while (plainHelper::isStringExist(new_string, clan_map)) {
				i++;
				new_string = new_group->getName() + "_" + std::to_string(i); 
            }
            Group divided = new_group->divide(new_string);
            current->second.addGroup(divided);
            Area::groupArrive(new_string, divided.getClan(), clan_map);
        }
    }
}