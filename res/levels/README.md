Enemies of each levels are placed here.

levelX.txt specifies what enemies to load in each levels. .data and .conf files specify each enemies.

levelX.txt are stored as

    name1 offsetX1
    name2 offsetX2

Each lines mean there will be a enemy *name* in the position of *offsetX*.

.data files are in the same format as game savings

.conf files specify how the enemies act, stored as

    key1 sec1 flag1
    key2 sec2 flag2
    ...

Each lines mean it will "press" (when *flag*=0), or "release" (when *flag*=1) *key* after *sec* seconds since starting.
