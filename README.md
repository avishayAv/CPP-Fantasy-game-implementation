# CPP-Fantasy-game-implementation

OVERVIEW :
in our fantasy world we have clans, which contains groups, and some different areas - mountatins, rivers and plains.
The player can create different teams, split them into clans and areas, and use a different actions as described.

STRUCTURES :
Group :
- name
- name of clan
- number of children
- number of adults
- food (number)
- morale (0-100)
Clan :
- name
- List of the groups that belongs to the clan
- List of a friend-clans
Area :
- name
- List of the groups inside the area
- List of the reachable area from this area

Supported functionality and actions :
#1 : Clans creation.
#2 : Areas cretaion.
#3 : Groups creation.
#4 : Adding a group to an existing clan.
#5 : Uniting two clans.
#6 : Making two clans friends.
#7 : Printing details of a certain clan (Groups should be printed from strongest to weakest).
#8 : Adding a reachable-area from a certain area.
#9 : Move a certain group to a certain area. there are some side affects for each area, which described in area header.
      F.I, when arriving to a mountain, the group will fight other groups in order to be king of the mountatin.
      The rules are described in Mountain.h
#10 : Move a certain groun outside from it's current area.

NOTE :
Groups are able to trade, fight, and unite with each other according to the game rules.
For further details please see world.h and area.h documentation.

NOTE :
examples and formats supplied under "tests" folder.

thanks for reading.
