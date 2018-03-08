#include <string>
#include <list>
#include <iterator>
#include "Clan.h"
#include <ostream>
#include <memory>

using namespace mtm;

Clan::Clan(const std::string& name)  {
    if (name.empty())   {
        throw ClanEmptyName();
    }
    this->name = name;
    this->members = MtmSet<GroupPointer>();
    this->friends = std::list<Clan>();
}

Clan::Clan(const Clan& other) : members(MtmSet<GroupPointer>(other.members)),
                                friends(std::list<Clan>(other.friends))  {
    this->name = other.name;
}

void Clan::addGroup(const Group& group)   {
	GroupPointer temp =  std::make_shared<Group>(group);
	
    if (group.getSize() == 0)   {
        throw ClanGroupIsEmpty();
    }
	MtmSet<GroupPointer>::const_iterator ptr;
	MtmSet<GroupPointer>::const_iterator end_of_set;
	ptr = this->members.begin();
	while (ptr != end_of_set) {
		if ((*ptr)->getName() == group.getName()) {
			throw ClanGroupNameAlreadyTaken();
		}
		ptr++;
	}
	//GroupPointer copy_group
	(*temp).changeClan(this->name);
    this->members.insert(temp); 

}


const GroupPointer& Clan::getGroup(const std::string& group_name) const   {
    MtmSet<GroupPointer>::const_iterator temp;
    MtmSet<GroupPointer>::const_iterator end_of_set;
    temp = this->members.begin();
    while (temp != end_of_set)  {
        if ((*temp)->getName() == group_name)  {
            return *temp; 
        }
        temp++;
    }
    throw ClanGroupNotFound();
}

bool Clan::doesContain(const std::string& group_name) const {
    MtmSet<GroupPointer>::const_iterator temp;
    MtmSet<GroupPointer>::const_iterator end_of_set;
    temp = this->members.begin();
    while (temp != end_of_set)  {
        if ((*temp)->getName() == group_name)  {
            return true;
        }
        temp++;
    }
    return false;
}

int Clan::getSize() const {
    int size = 0;
    MtmSet<GroupPointer>::const_iterator ptr = this->members.begin();
    MtmSet<GroupPointer>::const_iterator end_of_set;
    while (ptr != end_of_set) {
        size += (*(ptr))->getSize();
        ptr++;
    }
    return size;
}


Clan& Clan::unite(Clan& other, const std::string& new_name) {
	if (new_name.empty()) {
		throw ClanEmptyName();
	}
    if (this == &other) {
        throw ClanCantUnite();
    }
	MtmSet<GroupPointer>::iterator ptr_other = other.members.begin();
	MtmSet<GroupPointer>::const_iterator ptr_this;
	MtmSet<GroupPointer>::iterator end_of_set;
	while (ptr_other != end_of_set) {
		ptr_this = this->members.begin();
		while (ptr_this != end_of_set) {
			if ((*ptr_this)->getName() == (*ptr_other)->getName()) {
				throw ClanCantUnite();
			}
			ptr_this++;
		}
		ptr_other++;
	}
	this->members.unite(other.members);
	std::list<Clan>::iterator list_ptr = other.friends.begin();
	for (; list_ptr != other.friends.end(); list_ptr++) {
		this->makeFriend(*list_ptr);
	}
	other.friends.clear();
	this->name.assign(new_name);
	ptr_this = this->members.begin();
	while (ptr_this!=end_of_set){
		(*ptr_this)->changeClan(this->name);
		ptr_this++;
	}
	other.members.clear();
	other.name.clear();
	return *this;
}

void Clan::makeFriend(Clan& other)  {
    if (this->isFriend(other))  {
        return;
    }
    this->friends.push_front(other);
    other.friends.push_front(*this);
}

bool Clan::isFriend(const Clan& other) const  {
    if (this->name == other.name)   {
        return true;
    }
    std::list<Clan>::const_iterator list_ptr = this->friends.begin();
    while (list_ptr != this->friends.end())    {
        if (list_ptr->name == other.name)   {
            return true;
        }
        list_ptr++;
    }
    return false;
}

std::ostream& mtm::operator<<(std::ostream& os, const Clan& clan){
	using std::endl;
	MtmSet<GroupPointer>::const_iterator ptr = clan.members.begin();
	MtmSet<GroupPointer>::const_iterator end_of_set;
	std::list<Group> list_from_set;
	while (ptr != end_of_set) {
		list_from_set.push_front(*(*ptr));
		ptr++;
	}
	list_from_set.sort();
	os << "Clan's name: " << clan.name << endl;
	os << "Clan's groups:" << endl;
	while (list_from_set.size()>0) {
		if (!list_from_set.back().getName().empty())    {
            os << list_from_set.back().getName() << endl;
        }
		list_from_set.pop_back();
	}
	return os;
}
