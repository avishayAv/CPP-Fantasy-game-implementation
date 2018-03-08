#ifndef MTM4_GROUP_HPP
#define MTM4_GROUP_HPP

#include <string>
#include <ostream>
#include <cmath>

#define TWENTY 20
#define DOWN (-1)
#define UP 1

namespace mtm{
    
    enum FIGHT_RESULT{
        WON, LOST, DRAW
    };
	enum POWER_COMPARE_RESULT {
		FIRST_BIGGER, SECOND_BIGGER, EQUAL
	};
    /**
    * A Family group of hunter-gatherers.
    */
    class Group{
        std::string name;
        std::string clan;
        int children;
        int adults;
        int tools;
        int food;
        int morale;

		/*
		input: two grroups to compare
		comparison made by: power, if power is equal- by name
		power:(adults*10 + children*3) * (10*tools + food)*(morale/100)
		output: FIRST_BIGGER(0), SECOND_BIGGER(1) , EQUAL(2).
		*/
		int whichGrater(const Group& group2) const {
			unsigned int power_group1 = (10 * (this->adults) + 3 * (this->children))
				*(10 * (this->tools) + (this->food))*(this->morale);
			power_group1 = power_group1 / 100;
			unsigned int power_group2 = (10 * (group2.adults) + 3 * (group2.children))
				*(10 * (group2.tools) + (group2.food))*(group2.morale);
			power_group2 = power_group2 / 100;
			if (power_group1 > power_group2) {
				return FIRST_BIGGER;
			}
			if (power_group1<power_group2) {
				return SECOND_BIGGER;
			}
			int result = this->name.compare(group2.name);
			if (result>0) {
				return FIRST_BIGGER;
			}
			if (result<0) {
				return SECOND_BIGGER;
			}
			return EQUAL;
		}

        //----------------------------------------------------------------------

		/*
		 * Input : Group, percentage, change is up (1) or down (-1),
		 * rounded up (true) or down (false)
		 * JOB : this function will change group's morale upon request,
		 * and according to requested percentage, up or down, and rounded U/D.
		 */
		void changeMoraleByPercentage(const double percentage,
                                      const int up_or_down, bool is_ceil) {
			int delta_morale;
			if (is_ceil) {
				delta_morale = (int)(ceil((double)morale*percentage /100 ));
			}
			else {
				delta_morale = (morale*percentage /100 );
			}

			morale = morale + (delta_morale*up_or_down);
			if (morale > 100) {
				morale = 100;
			}
		}

        //----------------------------------------------------------------------

        /*
         * Input : two groups
         * Output : TRUE if groups can be united, FALSE otherwise.
         * Validation cases :
         * #1 : not the same group
         * #2 : belong to the same clan
         * #3 : both have at least 70 morale
         * #4 : don't surpass together a given amount of people.
         */
        bool isUniteValid (const Group& group2, const int max_amount) const {
            //#1 : not the same group
            if (this == (&group2)) {
                return false;
            }
            //#2 : belong to the same clan
            if (this->clan != group2.clan) {
                return false;
            }
            //#3 : both have at least 70 morale
            if ((this->morale < 70)||(group2.morale < 70)) {
                return false;
            }
            //#4 : don't surpass together a given amount of people.
            int sum_people = this->adults + group2.adults;
            sum_people += this->children + group2.children;
            return sum_people <= max_amount;
        }

        /*
         * Input : 2 groups
         * Process : add all Group2 parameters to Group1
         */
        void uniteChanges (const Group& group2) {
            this->children += group2.children;
            this->adults += group2.adults;
            this->tools += group2.tools;
            this->food += group2.food;
        }

        //----------------------------------------------------------------------

        /*
         * Input : Group
         * Process : make all parameters empty. (String -> empty, number -> 0)
         */
        void makeGroupEmpty ()  {
            this->name = "";
            this->clan = "";
            this->children = 0;
            this->adults = 0;
            this->tools = 0;
            this->food = 0;
            this->morale = 0;
        }

        //----------------------------------------------------------------------

        /*
         * Input : group
         * Process : change parameters according to losing fight situation :
         * - Losing third of each of the children and adult population (ceiling)
         * - Losing Half of the tools, and half of the food (ceiling)
         * - Morale decreases by 20%. (ceiling)
         * Output : how many food the losing group lost
         */
        int lostFightChanges() {
            int lost_childern = (int)(ceil(((double)this->children/3)));
            int lost_adults = (int)(ceil(((double)this->adults/3)));
            int lost_tools = (int)(ceil(((double)this->tools/2)));
            int lost_food = (int)(ceil(((double)this->food/2)));
            changeMoraleByPercentage(TWENTY, DOWN, true);
            this->children -= lost_childern;
            this->adults -= lost_adults;
            this->tools -= lost_tools;
            this->food -= lost_food;
            if (getSize() == 0) {
                makeGroupEmpty();
            }
            return lost_food;
        }

        //----------------------------------------------------------------------

        /*
         * Input : group, lost food of losing group
         * Process : change parameters according to winning fight situation :
         * - Losing a quarter of the adults (floor).
         * - Losing a quarter of the tools (floor).
         * - gaining half of the food the loser group lost in the fight. (floor)
         */
        void winFightChanges(const int lost_food)  {
            int lost_adults = this->adults/4;
            int lost_tools = this->tools/4;
			//int add_food = this
            changeMoraleByPercentage(TWENTY, UP, true);
            this->adults -= lost_adults;
            this->tools -= lost_tools;
            this->food += lost_food/2;
        }

        //----------------------------------------------------------------------

        /*
         * Input : two groups
         * Output : TRUE if trade is available, FALSE otherwise
         * Cases for validation :
         * #1 : number of tools is equal to amount of food in Group1
         * #2 : number of tools is equal to amount of food in Group2
         * #3 : both groups has more tools than food
         * #4 : both groups has more food than tools
         */
        bool isTradeValid (const Group& group2) const   {
            //#1 : number of tools is equal to amount of food in Group1
            if (this->food == this->tools)  {
                return false;
            }
            //#2 : number of tools is equal to amount of food in Group2
            if (group2.food == group2.tools)    {
                return false;
            }
            //#3 : both groups has more tools than food
            if ((this->food < this->tools)&&(group2.food < group2.tools)) {
                return false;
            }
            //#4 : both groups has more food than tools
            if ((this->food > this->tools)&&(group2.food > group2.tools)) {
                return false;
            }
            return true;
        }

        //----------------------------------------------------------------------

        /*
         * Input : group
         * Output : how many tools/food this group can offer.
         * Calc : abs((tools-food)/2)
         */
        int setOfferForTrade () const    {
            double offer = ((double)(this->food - this->tools));
            offer = abs(offer);
            offer = offer / 2;
            int make_offer = ((int)ceil(offer));
            return make_offer;
        }

        //----------------------------------------------------------------------

        /*
         * Input : group
         * Output : TRUE if it's empty, FALSE otherwise.
         */
        bool isGroupEmpty() const {
            return (this->adults == 0) && (this->children == 0);
        }

//------------------------------------------------------------------------------

    public:
        /**
         * Constructor
         * @param name The group's name. Can't be empty.
         * @param clan The clan which the group belongs to.
         *  empty string means no clan.
         * @param children The amount of children in the group. Must be
         *  non-negative.
         * @param adults Bigger number means more tools. Must be non-negative.
         * @param tools Bigger number means more tools. Must be non-negative.
         * @param food Bigger number means more food. Must be non-negative.
         * @param morale Bigger number means higher morale.
         *  Must be between 0 to 100.
         * @throws GroupInvalidArgs If at least one of the arguments is
         *  invalid, or both adults and children are 0.
         */
        Group(const std::string& name, const std::string& clan, int children,
              int adults, int tools, int food, int morale);
        
        /**
         * Constructor.
         * - The group will not belong to any clan.
         * - The group will have 4 tools for every adult.
         * - The group will have 3 food for every adult,
         *      and 2 food for every child
         * - The group will have 70 morale.
         * @param name The group's name. Can't be empty.
         * @param children The amount of children in the group. Must be
         *  non-negative.
         * @param adults Bigger number means more tools. Must be non-negative.
         * @throws GroupInvalidArgs If at least one of the arguments is
         *  invalid, or both adults and children are 0.
         */
        Group(const std::string& name, int children, int adults);
        
        /**
         * Copy constructor
         * @param other The group to copy
         */
        Group(const Group& other) = default;

        /** Destructor
         */
        ~Group() = default;

        /**
         * Get the name of the group
         * @return The name of the group
         */
        const std::string& getName() const;
        
        /**
         * Get the amount of people (children + adults) in the group.
         * @return  Amount of people in the group
         */
        int getSize() const;
        
        /**
         * @return The name of the clan that the group belongs to.
         */
        const std::string& getClan() const;
        
        /**
         * Change the clan of the group.
         * If the group had a different clan before, reduce morale by 10%.
         * If the group's clan was the same as the given one, do nothing.
         * Otherwise, increase morale by 10%. (floor in both cases)
         *
         * floor: 0.5 -> 0, 1.5 -> 1 etc...
         * @param clan The name of the new clan that the groups will belong to.
         */
        void changeClan(const std::string& clan);
        
        /**
         * Compare two groups. according to power (defined here), and name.
         *
         * Power: ((10*number_of_adults + 3 * number_of_children) *
         * (10 * number_of_tools + number_of_food)*morale)/100 (floor)
         *
         * @param rhs
         * @return true, if power(this) < power(rhs), or if they're equal and
         *  this group's name is smaller (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator<(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) > power(rhs), or if they're equal and
         *  this group's name is bigger (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator>(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) < power(rhs), or if they're equal and
         *  this group's name is not bigger (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator<=(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) > power(rhs), or if they're equal and
         *  this group's name is not smaller (lexicographic) than rhs's name.
         *  false otherwise.
         */
        bool operator>=(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) == power(rhs), and this group's name is
         * equal to rhs's name.
         * false otherwise.
         */
        bool operator==(const Group& rhs) const;
        
        /**
         * Compare two groups. according to power, and name.
         * @param rhs
         * @return true, if power(this) != power(rhs), and this group's name
         * isn't equal to rhs's name.
         * false otherwise.
         */
        bool operator!=(const Group& rhs) const;
        
        /**
         * Try to unite two groups, this group, and given group.
         * Will succeed only if the two groups are not the same group,
         * belong to the same clan, both have at least 70 morale, and they
         * don't surpass together a given amount of people.
         * Should the union succeed, this group will change according to:
         * - name = name of the more powerful group
         * - clan = old clan
         * - children = sum of the children in the two groups
         * - adults = sum of the adults in the two groups
         * - tools = sum of the tools in the two groups
         * - food = sum of the food in the two groups
         * - morale = average of the morales, counting the amount of people
         * (floor)
         *
         * The other group will become empty.
         * @param other The group to unite with.
         * @param max_amount If the two groups have more people combined then
         *  max_amount, then the union fails.
         * @return True if Unite was successful, false otherwise.
         */
        bool unite(Group& other, int max_amount);
        
        /**
         * Divide the group.
         *
         * After the division this group will have the same
         *  name, clan, morale. And ceiling of half of children , adults ,
         *  tools, food
         *
         * The other group will have a new name, same clan, same morale,
         *  and floor of half of children , adults, tools, food.
         *
         * Ceiling: 0.5 -> 1, 1.5 -> 2 etc...
         *
         * floor: 0.5 -> 0, 1.5 -> 1 etc...
         * @param name The name of the new group.
         * @return The new group, that divided from this group.
         * @throws GroupInvalidArgs If name is empty.
         * @throws GroupCantDivide If the divided group Would be empty (can
         * only happen of this group has no more than 1 child and 1 adult).
         */
        Group divide(const std::string& name);
        
        /**
         * This group fights an opponent group. The bigger group according to
         *  the relational operators is the winner. If both groups are equal,
         *  the groups will not fight.
         *
         * Fights effects on the loser group:
         * - Losing third of each of the children and adult population (ceiling)
         * - Losing Half of the tools, and half of the food (ceiling)
         * - Morale decreases by 20%. (ceiling)
         * For example: if a group that had 5 adults and 3 tools lost and 50
         * morale, it will have 3 adults, 1 tool, and 40 morale after the fight.
         *
         * Fights effects on the winner group:
         * - Losing a quarter of the adults (floor) and no children.
         * - Losing a quarter of the tools (floor).
         * - gaining half of the food the loser group lost in the fight. (floor)
         *      The gain is equal to ceiling(loser.food / 2) / 2, even if the
         *      loser group lost all of it's people.
         * - Morale increases by 20%. (ceiling)
         * @param opponent The group to fight with.
         * @return WON if this group won, LOST if this group lost, and DRAW
         * in case the 2 groups are equal.
         * @throws GroupCantFightWithItself if opponent is the same as this
         * @throws GroupCantFightEmptyGroup if this group, or the other group
         *  is empty (i.e no people).
         */
        FIGHT_RESULT fight(Group& opponent);
        
        /**
         * Try to do a trade between the two groups.
         * Each groups tries make its tools and food equal.
         * To do that, each group offers |group's tools - group's food| / 2
         * (ceiling) of the resource it has more of.
         * If the other group offers the other resource, a trade is made.
         * In the trade, each group gives the average of the offers [(offer1 +
         * offer2)/2 (ceiling)], from the resource it has more of. A group
         * can't give more tools or food than it has, and the two groups
         * always trade the same amount of resources.
         *
         * If one of the groups has the same amount of tools as amount of
         * food, or if both has more tools than food, or vice versa,
         * the trade would not be made.
         * @param other Group to trade with.
         * @return true if a trade was made, false otherwise.
         * @throws GroupCantTradeWithItself if other is the same as this
         */
        bool trade(Group& other);
        
        
        /**
         * Print the data of a given group. Output form:
         *      Group's name: [group's name]
         *      Group's clan: [group's clan]
         *      Group's children: [group's children]
         *      Group's adults: [group's adults]
         *      Group's tools: [group's tools]
         *      Group's food: [group's food]
         *      Group's morale: [group's morale]
         *
         * @param os the output stream
         * @param group The group to print
         * @return the output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const Group& group);
    };
	std::ostream& operator<<(std::ostream& os, const Group& group);
} // namespace mtm

#endif //MTM4_GROUP_HPP
