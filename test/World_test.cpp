#include "../testMacros.h"
#include "../Group.h"
#include "../Clan.h"
#include "../Area.h"
#include "../Plain.h"
#include "../Mountain.h"
#include "../River.h"
#include "../World.h"
#include "../exceptions.h"

using namespace mtm;

bool testAddClanAreaGroup(){
	World world;
	std::ostringstream os;
	ASSERT_EXCEPTION(world.addClan(""), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(world.addClan("Beta"));
	ASSERT_EXCEPTION(world.addClan("Beta"), WorldClanNameIsTaken);
	world.printClan(os, "Beta");
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: Beta\n"
            "Clan's groups:\n"));
	ASSERT_EXCEPTION(world.addArea("", MOUNTAIN), WorldInvalidArgument);
	ASSERT_NO_EXCEPTION(world.addArea("plain", PLAIN));
	ASSERT_NO_EXCEPTION(world.addArea("river", RIVER));
	ASSERT_NO_EXCEPTION(world.addArea("mountain", MOUNTAIN));
	ASSERT_EXCEPTION(world.addArea("plain", MOUNTAIN), WorldAreaNameIsTaken);
	ASSERT_EXCEPTION(world.addGroup("", "Beta", 5, 5, "plain"),
					 WorldInvalidArgument);
	ASSERT_EXCEPTION(world.addGroup("group", "Beta", 0, 0, "plain"),
					 WorldInvalidArgument);
	ASSERT_EXCEPTION(world.addGroup("group", "lamda", 5, 5, "plain"),
					 WorldClanNotFound);
	ASSERT_EXCEPTION(world.addGroup("group", "Beta", 5, 5, "no such area"),
					 WorldAreaNotFound);
	ASSERT_NO_EXCEPTION(world.addGroup("group", "Beta", 5, 5, "plain"));
	ASSERT_EXCEPTION(world.addGroup("group", "Beta", 5, 5, "plain"),
					 WorldGroupNameIsTaken);
	world.printGroup(os, "group");
	ASSERT_TRUE(VerifyOutput(os, "Group's name: group\n"
		"Group's clan: Beta\n"
		"Group's children: 3\n"
		"Group's adults: 3\n"
		"Group's tools: 10\n"
		"Group's food: 13\n"
		"Group's morale: 77\n"
		"Group's current area: plain\n"));
	return true;

}

bool makeReachableAndMoveGroup() {
	World world;
	std::ostringstream os;
	world.addArea("plain", PLAIN);
	world.addArea("river", RIVER);
	world.addClan("Beta");
	world.addGroup("group", "Beta", 5, 5, "river");
	ASSERT_NO_EXCEPTION(world.makeReachable("plain", "river"));
	ASSERT_EXCEPTION(world.makeReachable("", "plain"), WorldAreaNotFound);
	ASSERT_EXCEPTION(world.makeReachable("no such area", "plain"),
					 WorldAreaNotFound);
	ASSERT_EXCEPTION(world.makeReachable("plain", "no such area"),
					 WorldAreaNotFound);
	ASSERT_EXCEPTION(world.moveGroup("group","plain"), WorldAreaNotReachable);
	ASSERT_EXCEPTION(world.moveGroup("group", "river"),WorldGroupAlreadyInArea);
	ASSERT_EXCEPTION(world.moveGroup("group", ""), WorldAreaNotFound);
	ASSERT_EXCEPTION(world.moveGroup("group", "no sucharea"),WorldAreaNotFound);
	ASSERT_EXCEPTION(world.moveGroup("", "plain"), WorldGroupNotFound);
	ASSERT_EXCEPTION(world.moveGroup("no such group", "plain"),
					 WorldGroupNotFound);
	world.printGroup(os, "group");
	ASSERT_TRUE(VerifyOutput(os, "Group's name: group\n"
		"Group's clan: Beta\n"
		"Group's children: 5\n"
		"Group's adults: 5\n"
		"Group's tools: 20\n"
		"Group's food: 25\n"
		"Group's morale: 77\n"
		"Group's current area: river\n"));
	ASSERT_NO_EXCEPTION(world.makeReachable("river", "plain"));
	ASSERT_NO_EXCEPTION(world.moveGroup("group", "plain"));
	world.printGroup(os, "group");
	ASSERT_TRUE(VerifyOutput(os, "Group's name: group\n"
		"Group's clan: Beta\n"
		"Group's children: 3\n"
		"Group's adults: 3\n"
		"Group's tools: 10\n"
		"Group's food: 13\n"
		"Group's morale: 77\n"
		"Group's current area: plain\n"));
	ASSERT_EXCEPTION(world.moveGroup("group", "plain"),WorldGroupAlreadyInArea);
	return true;
}

bool TestMakeFriend() {
	World world;
	world.addClan("tribe");
	world.addClan("lions");
	world.addArea("plain", PLAIN);
	ASSERT_NO_EXCEPTION(world.makeFriends("tribe", "lions"));
	ASSERT_EXCEPTION(world.makeFriends("tribe", ""), WorldClanNotFound);
	ASSERT_EXCEPTION(world.makeFriends("tribe", "no such area"),
					 WorldClanNotFound);
	ASSERT_EXCEPTION(world.makeFriends("no such area", "lions"),
					 WorldClanNotFound);
	return true;
}

bool testUniteClans()	{
    World world;
    std::ostringstream os;
    world.addArea("beach", PLAIN);
    world.addArea("banias", RIVER);
    world.addClan("Beta");
    world.addClan("Gamma");
    world.addClan("Zeta");
    world.addClan("Alpha");
    world.addClan("Delta");
    world.addGroup("wolfs", "Beta", 5, 5, "beach");
    world.addGroup("bunnies", "Beta", 10, 9, "beach");
    world.addGroup("cats", "Gamma", 6, 4, "banias");
    world.addGroup("dogs", "Gamma", 2, 2, "banias");
    world.addGroup("lions", "Zeta", 2, 2, "banias");
    ASSERT_EXCEPTION(world.uniteClans("Beta", "Gamma", ""),
					 WorldInvalidArgument);
    ASSERT_EXCEPTION(world.uniteClans("Betas", "Gamma", "hey"),
					 WorldClanNotFound);
    ASSERT_EXCEPTION(world.uniteClans("Beta", "Beta", ""),WorldInvalidArgument);
    ASSERT_EXCEPTION(world.uniteClans("Beta", "Gamma", "Zeta"),
					 WorldClanNameIsTaken);
    ASSERT_NO_EXCEPTION(world.uniteClans("Beta", "Gamma", "Lamda"));
    ASSERT_EXCEPTION(world.uniteClans("Beta", "Alpha", "cant"),
					 WorldClanNotFound);
    ASSERT_NO_EXCEPTION(world.uniteClans("Alpha", "Lamda", "ByeBye"));
	return true;
}

bool testPrintGroup()	{
	World world;
	world.addClan("tribe");
	world.addArea("plain", PLAIN);
	world.addGroup("lions", "tribe", 1, 1, "plain");
	std::ostringstream os;
	world.printGroup(os, "lions");
	ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
			"Group's clan: tribe\n"
			"Group's children: 1\n"
			"Group's adults: 1\n"
			"Group's tools: 4\n"
			"Group's food: 5\n"
			"Group's morale: 77\n"
            "Group's current area: plain\n"));
	return true;
}

bool testPrintClan()	{
    World world;
    world.addClan("tribe");
    world.addArea("plain", PLAIN);
    std::ostringstream os;
    world.printClan(os, "tribe");
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: tribe\n"
            "Clan's groups:\n"));
	return true;
}

int main() {
	RUN_TEST(testAddClanAreaGroup);
	RUN_TEST(makeReachableAndMoveGroup);
	RUN_TEST(TestMakeFriend);
	RUN_TEST(testUniteClans);
	RUN_TEST(testPrintGroup);
	RUN_TEST(testPrintClan);
	return 0;
}