#include "World.h"
#include "Clan.h"
#include "Area.h"
#include "Mountain.h"
#include "River.h"
#include "Plain.h"
#include <string>
#include <iostream>
#include "Group.h"
#include "exceptions.h"
#include <map>

using namespace mtm;

void World::addClan(const string& new_clan) {
	if (new_clan.empty()) {
		throw WorldInvalidArgument();
	}
	if (clan_names.contains(new_clan))  {
        throw WorldClanNameIsTaken();
    }
    /*if (clan_map.find(new_clan)!=clan_map.end())	{
		throw WorldClanNameIsTaken();
	}*/         //TODO : erase
	Clan temp_clan(new_clan);
	clan_map.insert(std::pair<string, Clan>(new_clan, temp_clan));
    clan_names.insert(new_clan);
}

/**
* Add a new area to the world.
* @param area_name The name of the area
* @param type The type of the area (PLAIN, MOUNTAIN, RIVER)
* @throws WorldInvalidArgument If area_name is empty
* @throws WorldAreaNameIsTaken If there is already an area with the
*  given name.
*/
void World::addArea(const string& area_name, AreaType type) {
	if (area_name.empty())	{
		throw WorldInvalidArgument();
	}
	if (areas.find(area_name) != areas.end()) {
		throw WorldAreaNameIsTaken();
	}
	if (type == MOUNTAIN){
		AreaPtr temp (new Mountain(area_name));
		areas.insert(std::pair<string, AreaPtr>(area_name, temp));
	}
	else if (type == RIVER){
		AreaPtr temp(new River(area_name));
		areas.insert(std::pair<string, AreaPtr>(area_name, temp));
	}
	else {
		AreaPtr temp(new Plain(area_name));
		areas.insert(std::pair<string, AreaPtr>(area_name, temp));
	}

}


/**
* Add a new group to the world, with given size, clan, and starting
*  area (the group "arrives" to the area).
*  The group should have 77 morale when entering the area.
* @param group_name The name of the new group
* @param clan_name The name of the clan of the group.
* @param num_children Number of children in the group.
* @param num_adults Number of the adults in the group.
* @param area_name The name of the area the group starts in (arrives
*  to it first)
* @throws WorldInvalidArgument If group_name is empty, or at least
* one of num_children and num_adults is negative, or both are 0.
* @throws WorldGroupNameIsTaken If there is already a group with the
*  given name in the world.
* @throws WorldClanNotFound If there is no clan with the given name
*  in the world.
* @throws WorldAreaNotFound If there is no area with the given name
*  in the world.
*/
void World::addGroup(const string& group_name, const string& clan_name, int
	num_children, int num_adults, const string& area_name) {
    if ((group_name.empty())||(num_children<0)||(num_adults<0)) {
        throw WorldInvalidArgument();
    }
    if ((num_children==0)&&(num_adults==0))   {
        throw WorldInvalidArgument();
    }
    for ( map<string,Clan>::iterator ptr=  clan_map.begin();
          ptr!= clan_map.end()  ;  ptr++){
		if (ptr->second.doesContain(group_name)) {
			throw WorldGroupNameIsTaken();
		}
	}
	if (clan_map.find(clan_name)==clan_map.end()){
		throw WorldClanNotFound();
	}
	if (areas.find(area_name)==areas.end()){
		throw WorldAreaNotFound();
	}
	try{
		map<string, Clan>::iterator clan_ptr = clan_map.find(clan_name);
		clan_ptr->second.addGroup(*(std::make_shared<Group>
                (group_name, num_children, num_adults)));
		map<string, AreaPtr>::iterator area_ptr = areas.find(area_name);
		area_ptr->second->groupArrive(group_name,clan_name,clan_map);
	}
	catch (const GroupInvalidArgs&)
	{
		throw WorldInvalidArgument();
	}
}


/**
* Make that an area reachable from another area.
* (make 'to' reachable from 'from')
* @param from The name of the area that the other area will be
* reachable from.
* @param to The name of the area that should be reachable from the
* other area.
* @throws WorldAreaNotFound If at least one of the areas isn't in
*  the world.
*/
void World::makeReachable(const string& from, const string& to) {
	std::map<string, AreaPtr>::iterator from_ptr = areas.find(from);
	if ((from_ptr == areas.end()) || (areas.find(to) == areas.end())) {
		throw WorldAreaNotFound();
	}
	from_ptr->second->addReachableArea(to);
}



/**
* Move a group to destination area.
* @param group_name The name of the group that should move
* @param destination The name of the area the group should move to.
* @throws WorldGroupNotFound If there is no group with the given
*  name in the world.
* @throws WorldAreaNotFound If there is no area with the given name
*  in the world.
* @throws WorldGroupAlreadyInArea If the group is already in the
*  destination area.
* @throws WorldAreaNotReachable If the destination area isn't
*  reachable from the area the group is currently in.
*/
void World::moveGroup(const string& group_name, const string& destination) {
	map<string,Clan>::iterator clan_ptr = clan_map.begin();
	std::map<string, AreaPtr>::iterator dest_ptr= areas.find(destination);
	std::map<string, AreaPtr>::iterator from_ptr = areas.begin();
	bool is_exist = false;
	for (;  clan_ptr != clan_map.end(); clan_ptr++)	{
		if (clan_ptr->second.doesContain(group_name)) {
			is_exist = true;
			break;
		}
	}
	if (is_exist == false) {
		throw WorldGroupNotFound();
	}
	if (dest_ptr == areas.end()){
		throw WorldAreaNotFound();
	}
	for (; from_ptr!= areas.end(); from_ptr++){
		MtmSet<string> group_names_set = from_ptr->second->getGroupsNames();
		if (group_names_set.contains(group_name))	{
			break;
		}
	}
	if (from_ptr->first == dest_ptr->first)	{
		throw WorldGroupAlreadyInArea();
	}
	if (!(from_ptr->second->isReachable(dest_ptr->first)))	{
		throw WorldAreaNotReachable();
	}
	from_ptr->second->groupLeave(group_name);
	dest_ptr->second->groupArrive(group_name, clan_ptr->first, clan_map);
}



/**
* Make to clans friends.
* @param clan1 The name of one of the clans to become friends.
* @param clan2 The name of the other clan to become friends with.
* @throws WorldClanNotFound If at least one of the clans isn't in
* the world.
*/
void World::makeFriends(const string& clan1, const string& clan2) {
	std::map<string, Clan>::iterator clan1_ptr = clan_map.find(clan1);
	std::map<string, Clan>::iterator clan2_ptr = clan_map.find(clan2);
	if ((clan1_ptr == clan_map.end()) || (clan2_ptr == clan_map.end())) {
		throw WorldClanNotFound();
	}
	clan1_ptr->second.makeFriend(clan2_ptr->second);
}

/**
 * Unite to clans to a new clan with a new name.
 * @param clan1 The name of one of the clan that need to unite.
 * @param clan2 The name of the second clan that need to unite.
 * @param new_name The name of the new clan.
 * @throws WorldInvalidArgument If new_name is empty.
 * @throws WorldClanNameIsTaken If new_name was already used for a
 * clan that is not clan1 or clan2.
 * @throws WorldClanNotFound If clan1 or clan2 are not in the world.
 */
void World::uniteClans(const string& clan1, const string& clan2,
                       const string& new_name)   {
    if (new_name.empty())   {
        throw WorldInvalidArgument();
    }
    std::map<string, Clan>::iterator clan1_ptr = clan_map.find(clan1);
    std::map<string, Clan>::iterator clan2_ptr = clan_map.find(clan2);
    if (clan_names.contains(new_name)&&(*(clan_names.find(new_name)) != clan1)&&
            (*(clan_names.find(new_name)) != clan2) )   {
        throw WorldClanNameIsTaken();
    }
    if ((clan1_ptr == clan_map.end()) || (clan2_ptr == clan_map.end())) {
        throw WorldClanNotFound();
    }
    try {
        clan1_ptr->second.unite(clan2_ptr->second, new_name);
        clan_names.insert(new_name);
		Clan temp = clan1_ptr->second;
		clan_map.erase(clan1);
		clan_map.erase(clan2);
		clan_map.insert(std::pair<string, Clan>(new_name, temp));
    }   catch (const ClanEmptyName&)    {
        throw WorldInvalidArgument();
    } catch (const ClanCantUnite&)  {
        throw WorldInvalidArgument();
    } catch (...) {
        throw;
    }
}

/**
 * Print a group to the ostream, using the group output function (<<).
 * Add to it another line (after the last one of a regular print) of
 * the form:
 *      Group's current area: [area name]
 * That print the area which the group is in.
 * @param os The ostream to print into.
 * @param group_name The name of the group to print.
 * @throws WorldGroupNotFound If there is no group in the world with
 *  the given name.
 */
void World::printGroup(std::ostream& os, const string& group_name) const   {
	map<string, Clan>::const_iterator clan_ptr = clan_map.begin();
	for (; clan_ptr != clan_map.end(); clan_ptr++) {
		if (clan_ptr->second.doesContain(group_name)) {
			break;
		}
	}
    if (clan_ptr==clan_map.end()) {
        throw WorldGroupNotFound();
    }
    mtm::operator<<(os, *(clan_ptr->second.getGroup(group_name)));
    std::map<string, AreaPtr>::const_iterator current = this->areas.begin();
    std::map<string, AreaPtr>::const_iterator end = this->areas.end();
    for (; (current!=end); current++)   {
        if ((*current->second).getGroupsNames().contains(group_name))   {
            break;
        }
    }
    if (current == end) {
        os << "Group's current area: " << std::endl;
    }
    else    {
        os << "Group's current area: " << current->first << std::endl;
    }
}
/**
 * Print a clan to the ostream, using the clan output function.
 * @param os The ostream to print into.
 * @param clan_name The name of the clan to print.
 * @throws WorldClanNotFound If there is no clan with the given name
 *  in the world.
 */
void World::printClan(std::ostream& os, const string& clan_name) const  {
    std::map<string, Clan>::const_iterator end = this->clan_map.end();
    std::map<string, Clan>::const_iterator current =
            this->clan_map.find(clan_name);
    if (current == end)    {
        throw WorldClanNotFound();
    }
    os << current->second;
}