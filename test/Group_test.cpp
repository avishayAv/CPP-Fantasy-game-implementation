#include "../testMacros.h"
#include "../Group.h"
#include "../exceptions.h"

using namespace mtm;

bool constructorTest() {
	ASSERT_EXCEPTION(Group("", 1, 1), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("the lions", 0, 0), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("the lions", -1, 1), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("the lions", 1, -1), GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("the lions", "tribe", 1, 1, -1, 1, 1),
                     GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("the lions", "tribe", 1, 1, 1, -1, 1),
                     GroupInvalidArgs);
	ASSERT_EXCEPTION(Group("the lions", "tribe", 1, 1, 1, 1, 101),
                     GroupInvalidArgs);
	Group tigers("tigers",  5, 5);
	ASSERT_TRUE(tigers.getName() == "tigers");
	ASSERT_TRUE(tigers.getSize() == 10);
	ASSERT_TRUE(tigers.getClan().empty());
	Group lions("lions", "greatest", 10, 10, 40, 50, 80);
	Group wolfs("wolfs", "winners", 5, 5, 20, 10, 90);
	ASSERT_TRUE(lions.getName() == "lions");
	ASSERT_TRUE(wolfs.getSize() == 10);
	ASSERT_TRUE(lions.getClan() == "greatest");
	ASSERT_NO_EXCEPTION(lions.changeClan("Lambda"));
	ASSERT_TRUE(lions.getClan() == "Lambda");
	ASSERT_TRUE(lions > wolfs);
	ASSERT_FALSE(lions == wolfs);

	return true;
}

bool getsTest() {
	Group lions("lions", "greatest", 10, 10, 40, 50, 80);
	Group wolfs("wolfs", "", 5, 5, 20, 10, 90);
	ASSERT_TRUE(lions.getName() == "lions");
	ASSERT_TRUE(lions.getSize() == 20);
	ASSERT_TRUE(lions.getClan() == "greatest");
	ASSERT_TRUE(wolfs.getName() == "wolfs");
	ASSERT_TRUE(wolfs.getSize() == 10);
	ASSERT_TRUE(wolfs.getClan().empty());
	return true;
}

bool checkOperators() {
	Group lions1("lions", "", 10, 10, 40, 50, 75);   //POWER 43,785
	Group lions2("lion", "", 10, 10, 40, 50, 75);   //POWER 43,785
	Group lions3("lion", "", 10, 10, 40, 50, 76);   //POWER 44,460
	Group lions4("lion", "", 10, 10, 40, 50, 76);   //POWER 44,460
	Group wolfs("wolfs", "", 5, 5, 20, 10, 90);     //POWER 12,285
	ASSERT_TRUE(lions2 < lions1);
	ASSERT_TRUE(lions3 > lions1);
	ASSERT_TRUE(lions3 > wolfs);
	ASSERT_TRUE(lions3 >= wolfs);
	ASSERT_TRUE(wolfs <= lions1);
	ASSERT_TRUE(lions2 <= lions1);
	ASSERT_TRUE(lions1 != lions2);
	ASSERT_TRUE(lions3 == lions4);
	ASSERT_TRUE(lions3 >= lions4);
	ASSERT_TRUE(lions3 <= lions4);
	return true;
}

bool changeClanTest() {
	Group lions("lions", "", 10, 10, 40, 50, 75);
	lions.changeClan("");
    std::ostringstream os;
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: \n"
            "Group's children: 10\n"
            "Group's adults: 10\n"
            "Group's tools: 40\n"
            "Group's food: 50\n"
            "Group's morale: 75\n"));
	lions.changeClan("winners");
	ASSERT_TRUE(lions.getClan() == "winners");
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: winners\n"
            "Group's children: 10\n"
            "Group's adults: 10\n"
            "Group's tools: 40\n"
            "Group's food: 50\n"
            "Group's morale: 82\n"));
	lions.changeClan("winners");
	ASSERT_TRUE(lions.getClan() == "winners");
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: winners\n"
            "Group's children: 10\n"
            "Group's adults: 10\n"
            "Group's tools: 40\n"
            "Group's food: 50\n"
            "Group's morale: 82\n"));
	lions.changeClan("losers");
	ASSERT_TRUE(lions.getClan() == "losers");
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: losers\n"
            "Group's children: 10\n"
            "Group's adults: 10\n"
            "Group's tools: 40\n"
            "Group's food: 50\n"
            "Group's morale: 74\n"));
	lions.changeClan("");
	ASSERT_TRUE(lions.getClan().empty());
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: \n"
            "Group's children: 10\n"
            "Group's adults: 10\n"
            "Group's tools: 40\n"
            "Group's food: 50\n"
            "Group's morale: 67\n"));
	Group wolfs("wolfs", "", 10, 10, 40, 50, 99);
	wolfs.changeClan("bla");
	ASSERT_TRUE(wolfs.getClan() == "bla");
    os << wolfs;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: wolfs\n"
            "Group's clan: bla\n"
            "Group's children: 10\n"
            "Group's adults: 10\n"
            "Group's tools: 40\n"
            "Group's food: 50\n"
            "Group's morale: 100\n"));
	return true;
}

bool uniteTest() {
	Group lions("lions", "greatest", 10, 10, 40, 50, 70);
	Group wolfs("wolfs", "", 5, 5, 20, 10, 90);
	Group tigers("tigers", "greatest", 5, 5, 20, 10, 30);
	Group mulan("mulan", "greatest", 5, 5, 20, 10, 75);
	bool result = lions.unite(wolfs, 90);
	ASSERT_TRUE(!result);
	result = lions.unite(lions, 90);
	ASSERT_TRUE(!result);
	wolfs.changeClan("blabla");
	result = lions.unite(wolfs, 90);
	ASSERT_TRUE(!result);
	wolfs.changeClan("greatest");
	result = lions.unite(wolfs, 3);
	ASSERT_TRUE(!result);
	result = lions.unite(tigers, 90);
	ASSERT_TRUE(!result);
    lions.unite(mulan, 90);
	ASSERT_TRUE(lions.getName() == "lions");
	ASSERT_TRUE(lions.getSize() == 30);
    std::ostringstream os;
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: greatest\n"
            "Group's children: 15\n"
            "Group's adults: 15\n"
            "Group's tools: 60\n"
            "Group's food: 60\n"
            "Group's morale: 71\n"));
	return true;
}

bool divideTest() {
	Group lions("lions", "greatest", 1, 1, 40, 50, 70);
	Group tigers("tigers", "greatest", 5, 6, 15, 17, 30);
	ASSERT_EXCEPTION(tigers.divide(""), GroupInvalidArgs);
	ASSERT_EXCEPTION(lions.divide("wolfs"), GroupCantDivide);
	Group wolfs = tigers.divide("wolfs");
    std::ostringstream os;
    os << wolfs;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: wolfs\n"
                        "Group's clan: greatest\n"
                        "Group's children: 2\n"
                        "Group's adults: 3\n"
                        "Group's tools: 7\n"
                        "Group's food: 8\n"
                        "Group's morale: 30\n"));
    os << tigers;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: tigers\n"
            "Group's clan: greatest\n"
            "Group's children: 3\n"
            "Group's adults: 3\n"
            "Group's tools: 8\n"
            "Group's food: 9\n"
            "Group's morale: 30\n"));
	return true;
}

bool fightTest() {
	Group lions("lions", "greatest", 1, 1, 40, 50, 70);//power 4095
	Group lions2("lions", "greatest", 1, 1, 40, 50, 70);//power 4095
	Group tigers("tigers", "greatest", 5, 6, 15, 17, 31);//3757.5
	Group mulan("mulan", "greatest", 30, 61, 22, 10, 77);//118125
	ASSERT_EXCEPTION(lions.fight(lions), GroupCantFightWithItself);
	ASSERT_TRUE(lions.fight(lions2) == DRAW);
	ASSERT_TRUE(lions.fight(mulan) == LOST);
    std::ostringstream os;
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: \n"
            "Group's clan: \n"
            "Group's children: 0\n"
            "Group's adults: 0\n"
            "Group's tools: 0\n"
            "Group's food: 0\n"
            "Group's morale: 0\n"));
    os << mulan;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: mulan\n"
            "Group's clan: greatest\n"
            "Group's children: 30\n"
            "Group's adults: 46\n"
            "Group's tools: 17\n"
            "Group's food: 22\n"
            "Group's morale: 93\n"));
	ASSERT_EXCEPTION(lions.fight(mulan), GroupCantFightEmptyGroup);
	ASSERT_EXCEPTION(mulan.fight(lions), GroupCantFightEmptyGroup);
	ASSERT_TRUE(lions2.fight(tigers) == WON);
    os << lions2;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: greatest\n"
            "Group's children: 1\n"
            "Group's adults: 1\n"
            "Group's tools: 30\n"
            "Group's food: 54\n"
            "Group's morale: 84\n"));
    os << tigers;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: tigers\n"
            "Group's clan: greatest\n"
            "Group's children: 3\n"
            "Group's adults: 4\n"
            "Group's tools: 7\n"
            "Group's food: 8\n"
            "Group's morale: 24\n"));
	return true;
}

bool printTest() {
	Group lions("lions", "", 1, 1, 40, 50, 70);
	Group tigers("tigers", "greatest", 5, 6, 15, 17, 30);
	std::ostringstream os;
	os << lions << tigers;
	ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
		"Group's clan: \n"
		"Group's children: 1\n"
		"Group's adults: 1\n"
		"Group's tools: 40\n"
		"Group's food: 50\n"
		"Group's morale: 70\n"
		"Group's name: tigers\n"
		"Group's clan: greatest\n"
		"Group's children: 5\n"
		"Group's adults: 6\n"
		"Group's tools: 15\n"
		"Group's food: 17\n"
		"Group's morale: 30\n"));

	return true;
}

bool tradeTest() {
	Group lions("lions", "greatest", 1, 1, 5, 200, 70);
	Group wolfs("wolfs", "", 5, 5, 17, 26, 90);
	Group tigers("tigers", "greatest", 5, 6, 19, 17, 31);
	Group mulan("mulan", "greatest", 30, 61, 22, 10, 77);
	Group equal("equal", "", 1, 1, 1, 1, 1);
	ASSERT_EXCEPTION(lions.trade(lions), GroupCantTradeWithItself);
	ASSERT_TRUE(!lions.trade(equal));
	ASSERT_TRUE(!lions.trade(wolfs));
	ASSERT_TRUE(!mulan.trade(tigers));
	ASSERT_TRUE(mulan.trade(wolfs));
    std::ostringstream os;
    os << mulan;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: mulan\n"
            "Group's clan: greatest\n"
            "Group's children: 30\n"
            "Group's adults: 61\n"
            "Group's tools: 16\n"
            "Group's food: 16\n"
            "Group's morale: 77\n"));
    os << wolfs;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: wolfs\n"
            "Group's clan: \n"
            "Group's children: 5\n"
            "Group's adults: 5\n"
            "Group's tools: 23\n"
            "Group's food: 20\n"
            "Group's morale: 90\n"));
	ASSERT_TRUE(lions.trade(tigers));
    os << lions;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: lions\n"
            "Group's clan: greatest\n"
            "Group's children: 1\n"
            "Group's adults: 1\n"
            "Group's tools: 24\n"
            "Group's food: 181\n"
            "Group's morale: 70\n"));
    os << tigers
            ;
    ASSERT_TRUE(VerifyOutput(os, "Group's name: tigers\n"
            "Group's clan: greatest\n"
            "Group's children: 5\n"
            "Group's adults: 6\n"
            "Group's tools: 0\n"
            "Group's food: 36\n"
            "Group's morale: 31\n"));
	return true;
}
int main() {
	RUN_TEST(constructorTest);
	RUN_TEST(getsTest);
	RUN_TEST(changeClanTest);
	RUN_TEST(uniteTest);
	RUN_TEST(divideTest);
	RUN_TEST(checkOperators);
	RUN_TEST(fightTest);
	RUN_TEST(tradeTest)
	RUN_TEST(printTest);
    return 0;
}