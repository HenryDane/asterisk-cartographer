# Game data formatting guide
All game data should be stored in `artifact/map_name` where `map_name` can be any space-less string.
In each directory inside `artifact/` there should be the following:
- `map.txt` : lists the number of maps as well as the starting prefix
- `objects.txt` : lists map data
- `quests.txt` : list quest data (only needed if `quests_on` is found in `objects.txt`
- various `.png` files all with the same starting space-less prefix and followed by a positive integer (starting from zero)

All IDs must be unique. 
**Deviation from the format WILL cause bugs, corruption of data, and or crashes!**

## Maps
All maps must be a `.png` file where each tile is one pixel. They should all start with the same prefix followed by a postitive integer starting from zero. The color of the pixels can be found in the `Colors` section.

## Colors
R - Randomized (the cartographer tool will randomize this tile) 
L - Lookup (the cartographer tool will look up the object in the objects.txt file)
A - This tile is animated internally
N - This tile will be turned into a floor unless the lookup completes
Black	 : Wall	
Gray 50% : Undefined [L] [N]
Dark Red : Enemy npc / turred [L]
Red	 : Fire [A][R]
Green 	 : Vegetation [R]
Lime	 : Impassable vegetation
Turquoise: Impassable machinery [R]
Indigo	 : Spawn point
Light Y. : Alternate floor
White 	 : Floor
Rose	 : NPC [L]
Blue Grey: Water [A][R]
Brown 	 : Chest [L]		
Light Tr.: Overgrown machinery [R] 

## Maps.txt
Must open with `! maps.txt`.
Second line contains number of maps.
Third line contains space-less string prefix for all maps.
Fourth line contains a boolean where true means that a `.c` file will be put out and false will result in a binary data being produced. 

## Objects.txt
Must open with `! objects.txt`. 
For the second line, put either `! quests_on` or `! quests_off`. If `quests_on` is used, the cartographer program will look for and try to parse `quests.txt`. 
The third line must contain the prefix all maps are named under.
Next five lines list the number of textures for [R] objects in order of the color list.
Then, the number of objects listed is given on a new line. If this is less than zero, [L] object data will be ignored and the maps will be parsed.
The remaining lines start with two integers corresponding with [L] objects on the first map. If multiple maps are used, the object defined by `-1 -1 -1 -1 -1` is to be used. If no [L] objects are defined, or if [L] objects are to be ignored, put `-2` here instead.  

For the map objects, this is the general format
`<x> <y> <type> <num data> <data>`

The types are:
0 - Chest / crate
1 - NPC
2 - Enemy
3 - Portals
4 - Undefined (reserved)

For chests and crates, they should have `3 * <number of items> + 1` as the `<num data>` parameter. Firstly, list `<num items>` and then for each item list `<id> <type> <useable (integer)>`.
For NPCs, they should have `3 * <number of items> + 10` as the `<num data>` parameter. The number of items must be less than or equal to 16. The general format for the first few data is `<id> <health> <type> <number of items in NPC inventory>` followed by (for each item) `<id> <type> <cost>`. After the inventory listing, `<is merchant (boolean)> <quest id (0 for none)> <x> <y>` should follow`. 
For enemies, the should have 5 as the `<num data>` parameter. Their format is `<id> <health> <type> <ammunition (-1 for infinite)> <immortal (boolean)>`. 
For portals, they should have 3 as the `<num data>` parameter. For their data, simply put `<map_id> <x> <y>`.
For all undefined tiles, `<num data>` should be 1 and `<data>` should be 0. This is to prevent parsing problems. For other uses, look at the source code. 

## Quests.txt
Must open with `! quests.txt`.
For second line, put `! all_is_null` as quests are not yet supported

## Example:
```
! maps.txt
3
example_map_
true
```
```
! objects.txt
! quests_on
example_map_
3
5
10
3
4
3
5 9 0 4 1 13 10 true
8 4 1 16 112 1000 0 2 17 10 140 18 2 70 true 0 8 4
5 0 2 5 4 1 0 -1 true
```
```
! quests.txt
! all_is_null
```
