#include "Mountain.h"
#include <assert.h>
using namespace mtm;

Mountain::Mountain(const std::string& name) :Area(name){
	area_king.reset();
}

void Mountain::groupArrive(const string& group_name, const string& clan,
	map<string, Clan>& clan_map) {
	std::map<string, Clan>::iterator current = clan_map.find(clan);
	Area::groupArrive(group_name, clan, clan_map);
	GroupPointer new_group = (*current).second.getGroup(group_name);
	if (this->area_king == nullptr) {
		area_king = new_group;
		return;
	}
	if (area_king->getClan() == new_group->getClan()) {
		if (*area_king<*new_group){
			area_king = new_group;
		}
		return;
	}
	if (new_group->fight(*area_king)==WON) {
		if (area_king->getName().empty()) {
			Area::groupLeave("");
		}
		area_king = new_group;
	}
	if (new_group->getName().empty()) {
		Area::groupLeave("");
	}
	
}


void Mountain::groupLeave(const std::string& group_name) {
	
	GroupPointer leaving_group;
	for (std::vector<GroupPointer>::iterator ptr  =groups.begin();
         ptr != groups.end(); ptr++){
		if ((*ptr)->getName() == group_name) {
			leaving_group = *ptr;
		}
	}
	Area::groupLeave(group_name);
	if (groups.empty()){
		area_king = nullptr;
		return;}
	GroupPointer strongest = groups.front();
	GroupPointer strongest_from_clan = nullptr;
	string current_clan = leaving_group->getClan();
	for (int i = 0; i < (int)groups.size(); i++)	{
		if ((*groups[i])>(*strongest))	{
			strongest = groups[i];
		}
		if (groups[i]->getClan() == current_clan)	{
			if ((strongest_from_clan == nullptr) ||
                    ((*strongest_from_clan) < (*groups[i]))) {
				strongest_from_clan = groups[i];
			}
		}
	}
	if (area_king->getName() == leaving_group->getName()) {
		if (strongest_from_clan!= nullptr)	{
			area_king = strongest_from_clan;
		}
		else{
			area_king = strongest;
		}
	}
}

GroupPointer Mountain::getAreaKing() const {
	return this->area_king;
}





