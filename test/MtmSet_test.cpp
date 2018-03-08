#include "../testMacros.h"
#include "../MtmSet.h"
#include "../exceptions.h"

using namespace mtm;

/*
 * Scenario #1 :
 * testing the following :
 * - insert : success, already exists, success after clear
 * - empty : true, false
 * - size : different sizes including 0
 * - contains : true, false, before and after insertion/erase.
 * - clear : success, clear empty set
 * - erase : success, erase not existing - should do nothing.
 */
bool scneario1()    {
    MtmSet<int> set1;
    ASSERT_TRUE(set1.size() == 0);
    ASSERT_NO_EXCEPTION(set1.insert(1));
    ASSERT_NO_EXCEPTION(set1.insert(2));
    ASSERT_NO_EXCEPTION(set1.insert(3));
    ASSERT_NO_EXCEPTION(set1.insert(4));
    ASSERT_NO_EXCEPTION(set1.insert(5));
    ASSERT_NO_EXCEPTION(set1.insert(5));
    ASSERT_TRUE(set1.size() == 5);
    ASSERT_TRUE(set1.contains(5));
    ASSERT_NO_EXCEPTION(set1.erase(5));
    ASSERT_TRUE(set1.size() == 4);
    ASSERT_NO_EXCEPTION(set1.erase(5));
    ASSERT_NO_EXCEPTION(set1.erase(6));
    ASSERT_TRUE(set1.size() == 4);
    ASSERT_TRUE(set1.contains(1));
    ASSERT_FALSE(set1.contains(5));
    ASSERT_FALSE(set1.empty());
    ASSERT_NO_EXCEPTION(set1.clear());
    ASSERT_TRUE(set1.empty());
    ASSERT_TRUE(set1.size() == 0);
    ASSERT_NO_EXCEPTION(set1.insert(1));
    ASSERT_TRUE(set1.contains(1));
    ASSERT_NO_EXCEPTION(set1.clear());
    ASSERT_NO_EXCEPTION(set1.clear());
    return true;
}

/*
 * Scenario #2 :
 * testing the following for both const+not const iterators
 * - begin : check success, check empty set.
 * - end : check success, check empty set.
 * - find : check exist, check not exist.
 * - contains : element is in/not in the set.
 */
bool scneario2() {
    //not const
    MtmSet<int> set1;
    MtmSet<int>::iterator iter1 = set1.begin();
    MtmSet<int>::iterator iter2 = set1.end();
    ASSERT_TRUE(iter1 == iter2);
    ASSERT_NO_EXCEPTION(set1.insert(1));
    iter1 = set1.begin();
    iter2 = set1.end();
    ASSERT_FALSE(iter1 == iter2);
    ASSERT_NO_EXCEPTION(set1.insert(2));
    iter1 = set1.begin();
    iter2 = set1.end();
    ASSERT_FALSE(iter1 == iter2);
    ASSERT_NO_EXCEPTION(set1.insert(3));
    ASSERT_NO_EXCEPTION(set1.insert(4));
    ASSERT_NO_EXCEPTION(set1.insert(5));
    ASSERT_NO_EXCEPTION(set1.insert(5));
    ASSERT_TRUE(*set1.find(1) == 1);
    ASSERT_TRUE(set1.find(6) == set1.end());
    ASSERT_TRUE(set1.contains(3));
    ASSERT_FALSE(set1.contains(0));

    set1.clear();

    //const
    MtmSet<int>::const_iterator const1 = set1.begin();
    MtmSet<int>::const_iterator const2 = set1.end();
    ASSERT_TRUE(const1 == const2);
    ASSERT_NO_EXCEPTION(set1.insert(1));
    const1 = set1.begin();
    const2 = set1.end();
    ASSERT_FALSE(const1 == const2);
    ASSERT_NO_EXCEPTION(set1.insert(2));
    const1 = set1.begin();
    const2 = set1.end();
    ASSERT_FALSE(const1 == const2);
    return true;
}

/*
 * Scenario #3 :
 * testing the following :
 * - isSuperSetOf : check empty set, check superset of the same set,
 *                  check same elements, check regular success.
 * - == : check success, check failure, check empty.
 * - != : check success, check failure, check empty.
 */
bool scneario3() {
    MtmSet<int> set1;
    ASSERT_TRUE(set1.isSuperSetOf(set1));
    ASSERT_NO_EXCEPTION(set1.insert(1));
    ASSERT_NO_EXCEPTION(set1.insert(2));
    ASSERT_NO_EXCEPTION(set1.insert(3));
    ASSERT_TRUE(set1.isSuperSetOf(set1));
    MtmSet<int> set2;
    ASSERT_FALSE(set1 == set2);
    ASSERT_TRUE(set1 != set2);
    ASSERT_TRUE(set1.isSuperSetOf(set2));
    ASSERT_FALSE(set2.isSuperSetOf(set1));
    ASSERT_NO_EXCEPTION(set2.insert(3));
    ASSERT_TRUE(set1.isSuperSetOf(set2));
    ASSERT_FALSE(set2.isSuperSetOf(set1));
    ASSERT_NO_EXCEPTION(set2.insert(2));
    ASSERT_NO_EXCEPTION(set2.insert(1));
    ASSERT_TRUE(set1.isSuperSetOf(set2));
    ASSERT_TRUE(set2.isSuperSetOf(set1));
    ASSERT_TRUE(set1 == set2);
    ASSERT_FALSE(set1 != set2);
    ASSERT_NO_EXCEPTION(set2.insert(4));
    ASSERT_FALSE(set1.isSuperSetOf(set2));
    ASSERT_TRUE(set2.isSuperSetOf(set1));
    ASSERT_FALSE(set1 == set2);
    ASSERT_TRUE(set1 != set2);
    set1.clear();
    set2.clear();
    ASSERT_TRUE(set1 == set2);
    ASSERT_FALSE(set1 != set2);
    return true;
}

bool checkUnion()   {
    MtmSet<int> set1;
    ASSERT_NO_EXCEPTION(set1.insert(1));
    ASSERT_NO_EXCEPTION(set1.insert(2));
    ASSERT_NO_EXCEPTION(set1.insert(3));
    MtmSet<int> set2;
    set1.unite(set2);
    ASSERT_TRUE(set1.size() == 3);
    ASSERT_NO_EXCEPTION(set2.insert(1));
    ASSERT_NO_EXCEPTION(set2.insert(2));
    set1.unite(set2);
    ASSERT_TRUE(set1.size() == 3);
    ASSERT_NO_EXCEPTION(set2.insert(3));
    set1.unite(set2);
    ASSERT_TRUE(set1.size() == 3);
    ASSERT_NO_EXCEPTION(set2.insert(4));
    set1.unite(set2);
    set2.unite(set1);
    ASSERT_TRUE(set1.size() == 4);
    ASSERT_TRUE(set2.size() == 4);
    ASSERT_TRUE(set1.contains(4));
    const MtmSet<int> set3;
    MtmSet<int> set4(set3.unite(set3));
    ASSERT_TRUE(set4.size() == 0);
    MtmSet<int> set5(set3.unite(set1));
    ASSERT_TRUE(set5.size() == 4);
    return true;
}

bool checkIntersect()   {
    MtmSet<int> set1;
    ASSERT_NO_EXCEPTION(set1.insert(1));
    ASSERT_NO_EXCEPTION(set1.insert(2));
    ASSERT_NO_EXCEPTION(set1.insert(3));
    MtmSet<int> set2;
    set2.intersect(set1);
    ASSERT_TRUE(set2.size() == 0);
    ASSERT_NO_EXCEPTION(set2.insert(1));
    set2.intersect(set1);
    ASSERT_TRUE(set2.size() == 1);
    ASSERT_NO_EXCEPTION(set2.insert(2));
    set2.intersect(set1);
    ASSERT_TRUE(set2.size() == 2);
    ASSERT_NO_EXCEPTION(set2.insert(3));
    set2.intersect(set1);
    ASSERT_TRUE(set2.size() == 3);
    ASSERT_NO_EXCEPTION(set2.insert(4));
    set2.intersect(set1);
    ASSERT_TRUE(set2.size() == 3);
    const MtmSet<int> set3;
    MtmSet<int> set4(set3.intersect(set3));
    ASSERT_TRUE(set4.size() == 0);
    MtmSet<int> set5(set3.unite(set1));
    MtmSet<int> set6(set3.intersect(set1));
    ASSERT_TRUE(set5.size() == 3);
    return true;
}

//Helper function for getSubSet
bool isBiggerThanFive(int n){
    return (n>5);
}

bool checkGetSubSet()   {
    MtmSet<int> set1;
    ASSERT_NO_EXCEPTION(set1.insert(1));
    ASSERT_NO_EXCEPTION(set1.insert(2));
    ASSERT_NO_EXCEPTION(set1.insert(3));
    ASSERT_TRUE(set1.getSubSet(isBiggerThanFive).size() == 0);
    ASSERT_NO_EXCEPTION(set1.insert(5));
    ASSERT_NO_EXCEPTION(set1.insert(6));
    ASSERT_TRUE(set1.getSubSet(isBiggerThanFive).size() == 1);
    ASSERT_NO_EXCEPTION(set1.insert(7));
    ASSERT_TRUE(set1.getSubSet(isBiggerThanFive).size() == 2);
    set1.clear();
    ASSERT_TRUE(set1.getSubSet(isBiggerThanFive).size() == 0);
    const MtmSet<int> set3;
    MtmSet<int> set4(set3.unite(set1));
    ASSERT_TRUE(set4.getSubSet(isBiggerThanFive).size() == 0);
    return true;
}

int main(){
    RUN_TEST(scneario1);
    RUN_TEST(scneario2);
    RUN_TEST(scneario3);
    RUN_TEST(checkUnion);
    RUN_TEST(checkIntersect);
    RUN_TEST(checkGetSubSet);
    return 0;
}