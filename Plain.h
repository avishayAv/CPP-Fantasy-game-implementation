#ifndef EX4_PLAIN_H
#define EX4_PLAIN_H

#include "Area.h"

using namespace mtm;

class Plain : public Area
{
public:
    //Constructor
    Plain(const std::string& name);

    //Destructor
    ~Plain() = default;

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
    void groupArrive(const string& group_name, const string& clan,
                     map<string, Clan>& clan_map) override ;

};

#endif //EX4_PLAIN_H
