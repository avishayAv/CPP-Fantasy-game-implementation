#include "..\testMacros.h"
#include "..\Group.h"
#include "..\Clan.h"
#include "..\exceptions.h"

using namespace mtm;


bool testConstructors() {
	Clan beta("Beta");
	ASSERT_TRUE(beta.getSize() == 0);
	ASSERT_EXCEPTION(Clan no_name(""), ClanEmptyName);
	Clan beta2(beta);
	ASSERT_TRUE(beta2.getSize() == 0);
	return true;
}

bool testAddGroup() {
	std::ostringstream os;
	Clan tribe("tribe");
	Group wolfs("wolfs", 5, 5);
	Group lions("lions", "", 1, 1, 40, 50, 70);
	Group mulan("mulan", "", 30, 61, 22, 10, 77);
	tribe.addGroup(mulan);
	tribe.addGroup(lions);
	GroupPointer temp_mulan = tribe.getGroup("mulan");
	os << *temp_mulan;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: mulan\n"
	"Group's clan: tribe\n"
	"Group's children: 30\n"
	"Group's adults: 61\n"
	"Group's tools: 22\n"
	"Group's food: 10\n"
	"Group's morale: 84\n"));
	ASSERT_TRUE(tribe.doesContain("lions"));
	ASSERT_EXCEPTION(tribe.addGroup(mulan),ClanGroupNameAlreadyTaken);
	os << *temp_mulan;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: mulan\n"
	"Group's clan: tribe\n"
	"Group's children: 30\n"
	"Group's adults: 61\n"
	"Group's tools: 22\n"
	"Group's food: 10\n"
	"Group's morale: 84\n"));
	tribe.addGroup(wolfs);
	ASSERT_TRUE(tribe.getSize() == 103);
	lions.fight(mulan);
	ASSERT_EXCEPTION(tribe.addGroup(lions), ClanGroupIsEmpty);
	return true;
}

bool testIsAndMakeFriend() {
	Clan tribe1("tribe1");
	Clan tribe2("tribe2");
	Clan tribe3("tribe3");
	tribe1.makeFriend(tribe2);
	ASSERT_TRUE(tribe1.isFriend(tribe2));
	ASSERT_TRUE(tribe2.isFriend(tribe1));
	ASSERT_FALSE(tribe2.isFriend(tribe3));
	ASSERT_NO_EXCEPTION(tribe1.makeFriend(tribe2));
	ASSERT_NO_EXCEPTION(tribe2.makeFriend(tribe1));
	ASSERT_TRUE(tribe1.isFriend(tribe2));
	ASSERT_TRUE(tribe2.isFriend(tribe1));
	ASSERT_TRUE(tribe2.isFriend(tribe2));
	tribe2.makeFriend(tribe3);
	ASSERT_TRUE(tribe2.isFriend(tribe3));
	ASSERT_TRUE(tribe3.isFriend(tribe2));
	ASSERT_FALSE(tribe1.isFriend(tribe3));
	ASSERT_FALSE(tribe3.isFriend(tribe1));
	return true;
}

bool testGetSize()	{
	Clan tribe("tribe");
    Clan coolest("coolest");
	ASSERT_TRUE(tribe.getSize() == 0);
	Group wolfs("wolfs", 5, 5);
	Group lions("lions", "greatest", 1, 1, 40, 50, 70);
	Group mulan("mulan", "greatest", 30, 61, 22, 10, 77);
	tribe.addGroup(mulan);
	ASSERT_TRUE(tribe.getSize() == 91);
	tribe.addGroup(lions);
	ASSERT_TRUE(tribe.getSize() == 93);
	tribe.addGroup(wolfs);
	ASSERT_TRUE(tribe.getSize() == 103);
    coolest.addGroup(lions);
    ASSERT_TRUE(coolest.getSize() == 2);
	return true;
}

bool testUnite()    {
    Clan tribe("tribe");
    Clan coolest("coolest");
    Clan rocks("rocks");
    Clan greatClan("greatClan");
    Group wolfs("wolfs", 5, 5);
    Group lions("lions", "", 1, 1, 40, 50, 70);
    Group mulan("mulan", "", 30, 61, 22, 10, 77);
    Group wolfs2("wolfs2", 5, 5);
    Group lions2("lions2", "", 1, 1, 40, 50, 70);
    Group mulan2("mulan2", "", 30, 61, 22, 10, 77);
    Group gang("gang", "", 30, 61, 22, 10, 77);
    tribe.unite(coolest, "smartest");
    ASSERT_EXCEPTION(tribe.unite(coolest, ""), ClanEmptyName);
    ASSERT_EXCEPTION(tribe.unite(tribe, "gangsters"), ClanCantUnite);
    tribe.addGroup(wolfs);
    tribe.addGroup(lions);
    coolest.addGroup(wolfs);
    ASSERT_EXCEPTION(tribe.unite(coolest, "gangsters"), ClanCantUnite);
    ASSERT_TRUE(coolest.unite(rocks, "rockies").getSize() == 10);
    rocks.addGroup(lions);
    rocks.addGroup(mulan);
    ASSERT_TRUE(rocks.unite(coolest, "Hopa Hey").getSize() == 103);
    greatClan.addGroup(lions2);
    greatClan.addGroup(mulan2);
    greatClan.addGroup(wolfs2);
    ASSERT_TRUE(greatClan.getSize() == 103);
    ASSERT_TRUE(rocks.unite(greatClan, "Hopa Hey").getSize() == 206);
    return true;
}

bool testPrint()    {
    Group lions("lions", "", 1, 1, 40, 50, 70);//power 4095
    Group lions2("lions2", "", 1, 1, 40, 50, 69);//power a bit less than 4095
    Group tigers("tigers", "", 5, 6, 15, 17, 31);//3757.5
    Group mulan("mulan", "", 30, 61, 22, 10, 77);//118125
    Clan tribe("tribe");
    tribe.addGroup(mulan);
    tribe.addGroup(lions);
    tribe.addGroup(tigers);
    tribe.addGroup(lions2);
    std::ostringstream os;
    os << tribe;
    ASSERT_TRUE(VerifyOutput(os, "Clan's name: tribe\n"
            "Clan's groups:\n"
            "mulan\n"
            "lions\n"
            "lions2\n"
            "tigers\n"));
    return true;
}

bool testGetGroup() {
    Group lions("lions", "", 1, 1, 40, 50, 70);//power 4095
    Group lions2("lions2", "", 1, 1, 40, 50, 69);//power a bit less than 4095
    Group tigers("tigers", "", 5, 6, 15, 17, 31);//3757.5
    Group mulan("mulan", "", 30, 61, 22, 10, 77);//118125
    Clan tribe("tribe");
    tribe.addGroup(mulan);
    tribe.addGroup(lions);
    tribe.addGroup(tigers);
    tribe.addGroup(lions2);
    ASSERT_NO_EXCEPTION(tribe.getGroup("lions"));
    ASSERT_NO_EXCEPTION(tribe.getGroup("lions2"));
    ASSERT_NO_EXCEPTION(tribe.getGroup("tigers"));
    ASSERT_NO_EXCEPTION(tribe.getGroup("mulan"));
    ASSERT_EXCEPTION(tribe.getGroup("lions3"), ClanGroupNotFound);
    ASSERT_EXCEPTION(tribe.getGroup("tigers2"), ClanGroupNotFound);
    ASSERT_EXCEPTION(tribe.getGroup("cats"), ClanGroupNotFound);
    ASSERT_EXCEPTION(tribe.getGroup("themulan"), ClanGroupNotFound);
	return true;

}

int main() {
	RUN_TEST(testConstructors);
	RUN_TEST(testAddGroup);
	RUN_TEST(testIsAndMakeFriend);
    RUN_TEST(testGetSize);
    RUN_TEST(testUnite);
    RUN_TEST(testPrint);
    RUN_TEST(testGetGroup);
	return 0;
}