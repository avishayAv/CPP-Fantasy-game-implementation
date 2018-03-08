#include <string>
#include <ostream>
#include "Group.h"
#include "exceptions.h"

#define UP 1
#define TEN 10
#define TWO 2
#define THREE 3
#define FOUR 4
#define SEVENTY 70
#define HUNDRED 100
#define ZERO 0

//------------------------------------------------------------------------------

using namespace mtm;

namespace groupHelper {
    /*
     * Input : basic group parameters (name, adults, children)
     * Output : TRUE if it is valid, FALSE otherwise
     */
    bool isValidBasic(const std::string &name, const int children,
                      const int adults) {
        //Empty Group name
        if (name.length() == ZERO) {
            return false;
        }
        //Negative amount of children/adults
        if ((children < ZERO) || (adults < ZERO)) {
            return false;
        }
        //Empty group - both adults and children are 0
        if (adults == ZERO && (children == ZERO)) {
            return false;
        }
        return true;
    }

    //--------------------------------------------------------------------------

    /*
     * Input : advanced group parameters (tools, food, morale)
    * Output : TRUE if it is valid, FALSE otherwise
    */
    bool isValidAdvanced(const int tools, const int food, const int morale) {
        //Negative amount of tools/food
        if ((tools < ZERO) || (food < ZERO)) {
            return false;
        }
        //Morale is not between 0 and 100
        if ((morale < ZERO) || (morale > HUNDRED)) {
            return false;
        }
        return true;
    }
}

//------------------------------------------------------------------------------

using namespace groupHelper;

Group::Group(const std::string& name, const std::string& clan, int children,
      int adults, int tools, int food, int morale)  {
    if (!isValidBasic(name, children, adults)) {
        throw GroupInvalidArgs();
    }
    if (!isValidAdvanced(tools, food, morale)) {
        throw GroupInvalidArgs();
    }
    this->name = name;
    this->clan = clan;
    this->children = children;
    this->adults = adults;
    this->tools = tools;
    this->food = food;
    this->morale = morale;
}

//------------------------------------------------------------------------------

Group::Group(const std::string& name, int children, int adults) {
    if (!isValidBasic(name, children, adults)) {
        throw GroupInvalidArgs();
    }
    this->name = name;
    this->clan = "";
    this->children = children;
    this->adults = adults;
    this->tools = FOUR * adults;
    this->food = (TWO * children) + (THREE * adults);
    this->morale = SEVENTY;
}

//------------------------------------------------------------------------------

const std::string& Group::getName() const {
    return (this->name);
}

//------------------------------------------------------------------------------

int Group::getSize() const {
    return (this->adults + this->children);
}

//------------------------------------------------------------------------------

const std::string& Group::getClan() const {
    return (this->clan);
}

//------------------------------------------------------------------------------

void Group::changeClan(const std::string& clan) {
    if (clan == this->clan)    {
        return;
    }
    if (this->clan.empty()){
        this->clan.assign(clan);
        if (clan.empty()){
            return;
        }
		this->changeMoraleByPercentage(TEN, UP, false);
        return;
    }
    this->clan.assign(clan);
	this->changeMoraleByPercentage(TEN, DOWN, false);
}

//------------------------------------------------------------------------------

bool Group::operator==(const Group& rhs) const {
    return this->whichGrater(rhs) == EQUAL;
}

//------------------------------------------------------------------------------

bool Group::operator<(const Group& rhs) const {
    return this->whichGrater(rhs) == SECOND_BIGGER;
}

//------------------------------------------------------------------------------

bool Group::operator>(const Group& rhs) const {
    return this->whichGrater(rhs) == FIRST_BIGGER;
}

//------------------------------------------------------------------------------

bool Group::operator<=(const Group& rhs) const {
	int result = whichGrater(rhs);
    return (result == SECOND_BIGGER) || (result == EQUAL);
}

//------------------------------------------------------------------------------

bool Group::operator>=(const Group& rhs) const {
	int result = whichGrater(rhs);
    return (result == FIRST_BIGGER) || (result == EQUAL);
}

//------------------------------------------------------------------------------

bool Group::operator!=(const Group& rhs) const {
    return whichGrater(rhs) != EQUAL;
}

//------------------------------------------------------------------------------

bool Group::unite(Group& other, int max_amount)    {
    if (!isUniteValid(other, max_amount))  {
        return false;
    }
    int total_size = this->getSize() + other.getSize();
    int new_morale = (this->getSize() * this->morale)
                     + (other.getSize() * other.morale);
    new_morale = new_morale / total_size;
    this->morale = new_morale;
    if (*this < other)  {
        this->name = other.name;
    }
    uniteChanges(other);
    other.makeGroupEmpty();
    return true;
}

//------------------------------------------------------------------------------

Group Group::divide(const std::string& name)   {
    //check empty group
    if (name.length() == 0) {
        throw GroupInvalidArgs();
    }
    //Check that group could be divided
    if ((this->children <= 1)&&(this->adults <= 1))   {
        throw GroupCantDivide();
    }
    //divide group and return divided group
    int new_children = this->children / 2;
    this->children -= new_children;
    int new_adults = this->adults / 2;
    this->adults -= new_adults;
    int new_tools = this->tools / 2;
    this->tools -= new_tools;
    int new_food = this->food / 2;
    this->food -= new_food;
    Group new_group(name, this->clan, new_children, new_adults,
                    new_tools, new_food, this->morale);
    return new_group;
}

//------------------------------------------------------------------------------

FIGHT_RESULT Group::fight(Group& opponent) {
    if (this == &opponent)  {
        throw GroupCantFightWithItself();
    }
    if ((isGroupEmpty())||(opponent.isGroupEmpty())) {
        throw GroupCantFightEmptyGroup();
    }
    if (opponent == *this)   {
        return DRAW;
    }
    int lost_food;
    if (opponent > *this)   {
        lost_food = lostFightChanges();
        //check if losing group is empty
        if (getSize()==0)   {
            makeGroupEmpty();
        }
        opponent.winFightChanges(lost_food);
        return LOST;
    }
    lost_food = opponent.lostFightChanges();
    //check if losing group is empty
    if (opponent.getSize() == 0)    {
        opponent.makeGroupEmpty();
    }
    winFightChanges(lost_food);
    return WON;
}

//------------------------------------------------------------------------------

bool Group::trade(Group& other)    {
    if (&other == this) {
        throw GroupCantTradeWithItself();
    }
    if (!isTradeValid(other))   {
        return false;
    }
    //Assumption:trade is valid, one group has more food, and second- more tools
    //Case #1 : Group 1 has more food.
    if (this->food > this->tools)   {
        int food_offer = setOfferForTrade();
        int tool_offer = other.setOfferForTrade();
        double to_be_trade = ceil(((double)(food_offer+tool_offer))/2);
        int trade = ((int)to_be_trade);
        //Check if both teams has enough tools and food to do the trade
        if (trade > this->food)   {
            trade = this->food;
        }
        if (trade > other.tools)    {
            trade = other.tools;
        }
        this->tools += trade;
        other.tools -= trade;
        this->food -= trade;
        other.food += trade;
    }
    //Case #2 : Group 1 has more tools.
    else    {
        int food_offer = other.setOfferForTrade();
        int tool_offer = setOfferForTrade();
        double to_be_trade = ceil(((double)(food_offer+tool_offer))/2);
        int trade = ((int)to_be_trade);
        //Check if both teams has enough tools and food to do the trade
        if (trade > this->tools)    {
            trade = this->tools;
        }
        if (trade > other.food) {
            trade = other.food;
        }
        this->tools -= trade;
        other.tools += trade;
        this->food += trade;
        other.food -= trade;
    }
    return true;
}

//------------------------------------------------------------------------------

std::ostream& mtm::operator<<(std::ostream &os, const Group &group) {
	using std::endl;
	os << "Group's name: " << group.name << endl;
	os << "Group's clan: " << group.clan << endl;
	os << "Group's children: " << group.children << endl;
	os << "Group's adults: " << group.adults << endl;
	os << "Group's tools: " << group.tools << endl;
	os << "Group's food: " << group.food << endl;
	os << "Group's morale: " << group.morale << endl;
	return os;
}

//------------------------------------------------------------------------------