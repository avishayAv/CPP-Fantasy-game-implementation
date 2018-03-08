#include <string>
#include <list>
#include <iterator>
#include "Clan.h"
#include <ostream>
#include <memory>
#include "Area.h"

using namespace mtm;

Area::Area(const std::string& name) {
	if (name.empty()) {
		throw AreaInvalidArguments();
	}
	this->name = name;
}


/**
* Add an area, that can be reachable from this area.
* Doesn't mean that this area is reachable from the area with the
* given name.
* @param area_name The name of the area that can be reachable from
*  this area.
*/
void Area::addReachableArea(const std::string& area_name) {
	this->reach_able.insert(area_name);
}

/**
* Check if an area is reachable from this area.
* An area is always reachable from itself.
* @param area_name The name of the area to check if it's reachable.
* @return True if the area with the given name is reachable from
* this area, and false otherwise.
*/
bool Area::isReachable(const std::string& area_name) const {
	if (area_name == name){
		return true;
	}
	if (this->reach_able.contains(area_name)){
		return true;
	}
	return false;
}



/**
* Get a group into the area.
* @param group_name The name of the group that get into the area.
* @param clan The name of the clan that the group belongs to.
* @param clan_map The map of clans that contains the clan of the group
* @throws AreaClanNotFoundInMap If there is no clan with the given
* name in the map.
* @throws AreaGroupNotInClan If there is no group with the given name
* in the clan with the given name in the map.
* @throws AreaGroupAlreadyIn If group with same name already in the
*  area.
*
* Assert that the map does contain the clan that the group belongs to.
*/
void Area::groupArrive(const string& group_name, const string& clan,
	map<string, Clan>& clan_map) {
	std::map<string, Clan>::iterator current = clan_map.find(clan);
	if (current == clan_map.end()) {
		throw AreaClanNotFoundInMap();
	}
	if (!(current->second.doesContain(group_name))) {
        throw AreaGroupNotInClan();
	}
	int size = groups.size();
	for (int i = 0; i < size; i++)	{
		if (groups[i]->getName() == group_name) {
			throw AreaGroupAlreadyIn();
		}
	}
	GroupPointer new_group = (*current).second.getGroup(group_name);
	this->groups.push_back(new_group);
}


/**
* Get a group out of the area.
* @param group_name The name of the leaving group.
* @throws AreaGroupNotFound If there is no group in the area with the
*  same name;
*/
void Area::groupLeave(const std::string& group_name) {
	std::vector<GroupPointer>::iterator ptr = groups.begin();
	bool is_exist = false;
	for (; ptr != groups.end(); ptr++) {
		if ((*ptr)->getName() == group_name) {
			groups.erase(ptr);
			is_exist = true;
			break;
		}
	}
	if (is_exist == false)	{
		throw AreaGroupNotFound();
	}
}

/**
* Get a set of the names of all the groups in the area.
* @return A set that contains the names of all the groups in the area.
*/
MtmSet<std::string> Area::getGroupsNames() const {
	MtmSet<string> new_set;
	int size = groups.size();
	for (int i = 0; i < size; i++) {
		new_set.insert(groups[i]->getName());
	}
	return new_set;
}