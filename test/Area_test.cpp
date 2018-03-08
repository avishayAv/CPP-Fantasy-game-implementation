#include "../testMacros.h"
#include "../exceptions.h"
#include "../Area.h"
#include "../Plain.h"
#include "../Mountain.h"
#include "../River.h"

using namespace mtm;

bool testAreaConstructor()  {
    ASSERT_EXCEPTION(Area no_name(""), AreaInvalidArguments);
    ASSERT_EXCEPTION(Plain no_name(""), AreaInvalidArguments);
    ASSERT_EXCEPTION(Mountain no_name(""), AreaInvalidArguments);
    ASSERT_EXCEPTION(River no_name(""), AreaInvalidArguments);
    MtmSet<string> empty_set;
    Area new_area("new_area");
    ASSERT_TRUE(new_area.getGroupsNames() == empty_set);
    Mountain new_mountain("new_mountain");
    ASSERT_TRUE(new_mountain.getGroupsNames() == empty_set);
    Plain new_plain("new_plain");
    ASSERT_TRUE(new_plain.getGroupsNames() == empty_set);
    River new_river("new_river");
    ASSERT_TRUE(new_river.getGroupsNames() == empty_set);
    return true;
}

//Both functions are tested together since they are related
bool testReachableAreas() {
    Mountain carmel("carmel");
    River yarden("yarden");
    Plain beach("beach");
    ASSERT_FALSE(carmel.isReachable("yarden"));
    ASSERT_FALSE(beach.isReachable("yarden"));
    ASSERT_TRUE(yarden.isReachable("yarden"));
    yarden.addReachableArea("carmel");
    yarden.addReachableArea("beach");
    ASSERT_TRUE(yarden.isReachable("carmel"));
    ASSERT_TRUE(yarden.isReachable("beach"));
    ASSERT_FALSE(carmel.isReachable("yarden"));
    ASSERT_FALSE(beach.isReachable("yarden"));
    return true;
}

bool testGetGroupsNames() {
    Area new_area("new_area");
    Clan tribe("tribe");
    Clan coolest("coolest");
    std::map<std::string, Clan> clan_map;
    clan_map.insert(std::pair<std::string, Clan>("tribe", Clan("tribe")));
    clan_map.insert(std::pair<std::string, Clan>("coolest", Clan("coolest")));
    Group mulan("mulan", "", 30, 61, 22, 10, 77);
    clan_map.at("tribe").addGroup(mulan);
    clan_map.at("tribe").addGroup(Group("lions", "", 1, 1, 40, 50, 70));
    clan_map.at("coolest").addGroup(Group("cats", 10, 1));
    clan_map.at("coolest").addGroup(Group("tigers", "", 2, 2, 40, 50, 70));
    MtmSet<std::string> groupNames = new_area.getGroupsNames();
    ASSERT_TRUE(groupNames.size() == 0);
    new_area.groupArrive("tigers", "coolest", clan_map);
	MtmSet<std::string> groupNames2 = new_area.getGroupsNames();
    ASSERT_TRUE(groupNames2.size() == 1);
    new_area.groupArrive("cats", "coolest", clan_map);
    new_area.groupArrive("lions", "tribe", clan_map);
    new_area.groupArrive("mulan", "tribe", clan_map);
    MtmSet<std::string> groupNames3 = new_area.getGroupsNames();
    ASSERT_TRUE(groupNames3.size() == 4);
    new_area.groupLeave("cats");
    new_area.groupLeave("mulan");
    MtmSet<std::string> groupNames4 = new_area.getGroupsNames();
    ASSERT_TRUE(groupNames4.size() == 2);
    new_area.groupLeave("lions");
    new_area.groupLeave("tigers");
    MtmSet<std::string> groupNames5 = new_area.getGroupsNames();
    ASSERT_TRUE(groupNames5.size() == 0);
    return true;
}

bool testArriveAndLeaveMountain() {
    Mountain carmel("carmel");
    Clan tribe("tribe");
    Clan coolest("coolest");
    std::map<std::string, Clan> clan_map;
    clan_map.insert(std::pair<std::string, Clan>("tribe", Clan("tribe")));
    clan_map.insert(std::pair<std::string, Clan>("coolest", Clan("coolest")));
    Group mulan("mulan", "", 30, 61, 22, 10, 77);   //power 118125
    clan_map.at("tribe").addGroup(mulan);
    clan_map.at("tribe").addGroup(Group("lions", "", 1, 1, 40, 50, 70));
    clan_map.at("coolest").addGroup(Group("cats", "", 1, 1, 40, 50, 69));
    clan_map.at("coolest").addGroup(Group("tigers", "", 5, 6, 15, 17, 31));
    clan_map.at("coolest").addGroup(Group("tigers2", "", 5, 7, 15, 17, 31));
    carmel.groupArrive("lions", "tribe", clan_map);
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "lions");
    carmel.groupArrive("mulan", "tribe", clan_map);
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "mulan");
    carmel.groupArrive("cats", "coolest", clan_map);
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "mulan");
    carmel.groupLeave("mulan");
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "lions");
    carmel.groupLeave("lions");
    MtmSet<std::string> groupNames(carmel.getGroupsNames());
    ASSERT_TRUE(groupNames.size() == 0);
    ASSERT_EXCEPTION(carmel.groupLeave("cats"), AreaGroupNotFound);
    carmel.groupArrive("tigers", "coolest", clan_map);
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "tigers");
    carmel.groupArrive("mulan", "tribe", clan_map);
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "mulan");
    carmel.groupArrive("tigers2", "coolest", clan_map);
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "mulan");
    ASSERT_EXCEPTION(carmel.groupArrive("tigers2", "coolest", clan_map),
                     AreaGroupAlreadyIn);
    carmel.groupLeave("mulan");
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "tigers2");
    carmel.groupLeave("tigers");
    ASSERT_TRUE((*carmel.getAreaKing()).getName() == "tigers2");
    carmel.groupLeave("tigers2");
    ASSERT_EXCEPTION(carmel.groupArrive("dogs", "are cools", clan_map),
                     AreaClanNotFoundInMap);
    ASSERT_EXCEPTION(carmel.groupArrive("tigers", "tribe", clan_map),
                     AreaGroupNotInClan);
    return true;
}

bool testGroupArrivePlain() {
    Plain beach("beach");
    Clan tribe("tribe");
    Clan coolest("coolest");
    std::map<std::string, Clan> clan_map;
    clan_map.insert(std::pair<std::string, Clan>("tribe", Clan("tribe")));
    clan_map.insert(std::pair<std::string, Clan>("coolest", Clan("coolest")));
    Group mulan("mulan", "", 30, 30, 22, 10, 77);   //power 118125
    clan_map.at("tribe").addGroup(mulan);
    clan_map.at("tribe").addGroup(Group("lions", "", 1, 1, 40, 50, 70));
	clan_map.at("tribe").addGroup(Group("wolfs", "", 95, 95, 40, 50, 70));
    clan_map.at("coolest").addGroup(Group("cats", "", 9, 0, 40, 50, 69));
    clan_map.at("coolest").addGroup(Group("tigers_2", "", 5, 7, 15, 17, 31));
    beach.groupArrive("mulan", "tribe", clan_map);
	beach.groupArrive("lions", "tribe", clan_map);
    MtmSet<std::string> groupNames(beach.getGroupsNames());
    ASSERT_TRUE(groupNames.size() == 1);
	beach.groupArrive("wolfs", "tribe", clan_map);
	MtmSet<std::string> groupNames2(beach.getGroupsNames());
	ASSERT_TRUE(groupNames2.size() == 3);
	clan_map.at("tribe").addGroup(Group("mulan2", "", 30, 30, 40, 50, 70));
	beach.groupArrive("mulan2", "tribe", clan_map);
	MtmSet<std::string> groupNames3(beach.getGroupsNames());
	ASSERT_TRUE(groupNames3.size() == 4);
	beach.groupArrive("cats", "coolest", clan_map);
	MtmSet<std::string> groupNames4(beach.getGroupsNames());
	ASSERT_TRUE(groupNames4.size() == 5);
	clan_map.at("coolest").addGroup(Group("tigers", "", 30, 30, 15, 17, 31));
	beach.groupArrive("tigers", "coolest", clan_map);
	MtmSet<std::string> groupNames5(beach.getGroupsNames());
	ASSERT_TRUE(groupNames5.size() == 7);
    return true;
}

bool testGroupArriveRiver() {
    River jordan("jordan");
    Clan tribe("tribe");
    Clan coolest("coolest");
    Clan lonely("lonely");
	coolest.makeFriend(tribe);
    std::map<std::string, Clan> clan_map;
    clan_map.insert(std::pair<std::string, Clan>("tribe", Clan("tribe")));
    clan_map.insert(std::pair<std::string, Clan>("coolest", Clan("coolest")));
    clan_map.insert(std::pair<std::string, Clan>("lonely", Clan("lonely")));
    Group mulan("mulan", "", 30, 61, 22, 10, 77);
    clan_map.at("tribe").addGroup(mulan);
    clan_map.at("tribe").addGroup(Group("lions", "", 1, 1, 5, 200, 70));
    clan_map.at("tribe").addGroup(Group("equal", "", 1, 1, 1, 1, 1));
    clan_map.at("coolest").addGroup(Group("tigers", "", 5, 6, 19, 17, 31));
    clan_map.at("coolest").addGroup(Group("tigers2", "", 5, 6, 19, 17, 31));
    clan_map.at("coolest").addGroup(Group("dogs", "", 5, 5, 17, 26, 90));
    clan_map.at("lonely").addGroup(Group("wolfs", "", 5, 5, 17, 26, 90));
    jordan.groupArrive("lions", "tribe", clan_map);
    ASSERT_EXCEPTION(jordan.groupLeave("nothing_at_all"), AreaGroupNotFound);
    ASSERT_EXCEPTION(jordan.groupArrive("lions", "tribe", clan_map),
                     AreaGroupAlreadyIn);
    ASSERT_EXCEPTION(jordan.groupArrive("lions", "lonely", clan_map),
                     AreaGroupNotInClan);
    ASSERT_EXCEPTION(jordan.groupArrive("nothing", "no_such", clan_map),
                     AreaClanNotFoundInMap);
    jordan.groupArrive("equal", "tribe", clan_map);
    std::ostringstream os;
    os << (*clan_map.at("tribe").getGroup("lions"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: tribe\n"
            "Group's children: 1\n"
            "Group's adults: 1\n"
            "Group's tools: 5\n"
            "Group's food: 200\n"
            "Group's morale: 77\n"));
    jordan.groupArrive("wolfs", "lonely", clan_map);
    os << (*clan_map.at("lonely").getGroup("wolfs"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: wolfs\n"
            "Group's clan: lonely\n"
            "Group's children: 5\n"
            "Group's adults: 5\n"
            "Group's tools: 17\n"
            "Group's food: 26\n"
            "Group's morale: 99\n"));
    jordan.groupArrive("dogs", "coolest", clan_map);
    os << (*clan_map.at("coolest").getGroup("dogs"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: dogs\n"
            "Group's clan: coolest\n"
            "Group's children: 5\n"
            "Group's adults: 5\n"
            "Group's tools: 17\n"
            "Group's food: 26\n"
            "Group's morale: 99\n"));
    jordan.groupArrive("mulan", "tribe", clan_map);     //should trade with dogs
    os << (*clan_map.at("tribe").getGroup("mulan"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: mulan\n"
            "Group's clan: tribe\n"
            "Group's children: 30\n"
            "Group's adults: 61\n"
            "Group's tools: 0\n"
            "Group's food: 32\n"
            "Group's morale: 84\n"));
    MtmSet<std::string> groupNames4(jordan.getGroupsNames());
    ASSERT_TRUE(groupNames4.size() == 5);
    jordan.groupArrive("tigers2", "coolest", clan_map);//should trade with mulan
    os << (*clan_map.at("coolest").getGroup("tigers2"));
    ASSERT_TRUE(VerifyOutput(os, "Group's name: tigers2\n"
            "Group's clan: coolest\n"
            "Group's children: 5\n"
            "Group's adults: 6\n"
            "Group's tools: 16\n"
            "Group's food: 20\n"
            "Group's morale: 34\n"));
    jordan.groupLeave("lions");
    jordan.groupLeave("equal");
    return true;
}

int main() {
    RUN_TEST(testAreaConstructor);
    RUN_TEST(testReachableAreas);
    RUN_TEST(testGetGroupsNames);
    RUN_TEST(testArriveAndLeaveMountain);
    RUN_TEST(testGroupArrivePlain);
    RUN_TEST(testGroupArriveRiver);
    return 0;
}