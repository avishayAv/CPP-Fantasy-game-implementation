#include "Area.h"
using namespace mtm;

class Mountain : public Area
{
	GroupPointer area_king;

    public:
		//C'tor
        Mountain(const std::string& name);
		~Mountain() = default;

    /*
     * group arrive to mountain feature. JOB :
     * - if there's no king - the new group will turn into the king
     * - if the king is from the same clan - the strongest group from both will
     * be the king
     * - if the king is not from the same clan - both group will fight
     * and the winner will turn into king.
     */
        void groupArrive(const string& group_name, const string& clan,
			map<string, Clan>& clan_map);

    /*
     * if the leaving group is...
     * - not the king - no special functionality
     * - the king - replaced by the strongest team from the same clan
     * - if there's no other team from the same clan - the strongest team in the
     *      area will be a king.
     */
        void groupLeave(const std::string& group_name);
		GroupPointer getAreaKing() const;
};
