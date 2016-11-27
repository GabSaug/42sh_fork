#include <stdio.h>

#include "execute.h"
#include "builtins.h"
#include "hash_table.h"

static char* courtois = "\n\
:::::----------------------------:::::--.........-----------------::::----::::\
::::::::::::::::::::::\n\
:--------------------------------------------..........-----------------------\
----------::::::::::::\n\
-------------------------------------://///::::::::::::://:::-----------------\
---------------:::::::\n\
---------------------------------://++o++++++oo+oooosssysssoo+/:-----------:::\
::::::::::::::::::::::\n\
:::------------:::::::-:::::::///+oooo+/::-:-://////+ossyyyyssoo+:--:::::::///\
//////////////////////\n\
:::----------------------:::/++oo+/::---------::::::::/+osyyhhyso+/:-::::::::/\
//////////////////////\n\
::::-----------------:-:::::/++:--...............-------:/+oshhyooo+/:-:::::::\
:::://::://///////////\n\
:::::::::::::::::::::::://///:.....``````````........-----::/+yhysooo+::::::::\
::::::::::////////////\n\
----------------------:/+++/-.....`````````````........-----:/+yyyssooo/::::::\
::::::::::////////////\n\
........````..........:+++:....`````````````````.......-----::/sssssssso/:--..\
...------------:::::::\n\
`````````````````````./++:.....`````````````````........----::/ososysoo++/:-``\
``````````````````````\n\
```````            ``:++/.....`````````````````.........-----:/+osoyyyyso++/-`\
                     `\n\
```````            `-+oo-......````````````````.........-----:/+oosyyyyyssss+:\
.`                    \n\
``````            ``/ss/-......```````````````..........----::/+osyhhhhhhyyyyo\
/.                    \n\
                  `.sss/-......```````````````........------::/+syyhhhhhhhhyyy\
s:`                   \n\
```               `:yyy/-.......``````````..............--::::/+oyhhdhhhddhhhy\
so.`                  \n\
..`````````````````/yyh+....................---::::-------::::::+yhhddhhyhddhh\
ys+-`                 \n\
:::----------------/syh/-:::::::---....--:::/+ooossooo+///::::::/ohdddddhyyyyy\
ss+/`````````     ````\n\
:::::-::::::::::-:::ohysssssssoo+/::-..--:////+oosossso++/:::::::+ydmmdddhhyss\
sss/--------......-...\n\
------------------::ohysoossoysyso+/-.`.-:://+++:osssoso+/:::::::/ohddmdysoooo\
smy/:////////////////+\n\
-------------------:/sy+:+o+/+/////:-.`.-:::::::::::://///:::::::/+yhhdy+o+///\
/hy/:://////::///////+\n\
---------------------:s+:::::::::--...`.--.---...------------::::/+yhdhs++////\
:hy/:::::::::::///////\n\
---------.-----------:o+-----........`..--...............---:::///oyhhhyyo/:/:\
+dy/:::::::::::://////\n\
---------------------:++-...``.......``.----......````....--::///+ohhhyyso+::+\
dmy/:::::::::::::::://\n\
:::-------------------:+-...........```.----.---.--.......--:::/++sydhys+//:+d\
mds/:::::::::::::::::/\n\
:::::::::::::::::::::::o-.....--.......--:--::/:---::------::://++syddyo::/+hm\
ds/::::::::::::::::::/\n";
static char* courtois2 = ":::::::::::::::::::::::/:-----:-::/::::/+osyss+//::::/::::::::///+ohhdy/::+hdy\
+::::::::::::::::::///\n\
....--------------------::--::://oyhyysshdddyysoosssos+//:::/:://+oyhdh+:/hddh\
s+//////////////////++\n\
`````````````````........:::/osssshhhdyoshhhssosssyhhhhs//:/+///+osyydhhdmmmmd\
dhs+///////////////+++\n\
.....````````````````````-/:/hdyysooo+/:://////++oyss++ho//+o++ossyhhhhymmmmmm\
dhy+:::::::::::///////\n\
::::------.............``-o//yhsosys/::-:-.--://ooo/:::sy+syyssyyyyhdhysmmmNmd\
dhy+.....----------:::\n\
////::::::::::::::::::::-:yysyy+::/oo/:-:::://///::--::+ssyhhyyhhhhhdhoomdhhdh\
yys-``````````........\n\
::/::-::::::::::::::::::::ohddho:--::::::--:::::::---::/shhhdhhddddddy//ymdyss\
s+:..`````````````````\n\
:::::----:::--::::::::::--:sdmdho:--::/++o+o+++::----::+yyhhhhdddmdhy+/-:mmmdh\
sooo+/::::::::--------\n\
:::::------------:------.--:sddddo:--:/+so+++::-----:/osyyyhhdddmmds+/:..hmddm\
dhyhdhyoo+oo++++oo++++\n\
:::::-----------------....--:+ydmds:------...-----:/osyhhyhhdmmmmds+//-`.hmmmm\
mmddmmmdhsoo+++++++ooo\n\
:::::-------------::::://++oshddmmds//:--..---:/++oyyhdddddmmmdhyo+//:``.dmmmm\
mmmmmmmmmdyyssoo++++++\n\
::/::--------://+oosssyyyhhdddmmmmddhso+////+oyyyyhhddmmmmmmdyso+///-```-mmmmm\
mmmmmmmmmmmddhyssooooo\n\
:://:-:::/+ossyyyhhhhhddddddddmmmmmmmddhyyyyhhdddddddddmmdhyso++///.````+mmmmm\
mmmmmmmmmmmmmmdhhyyyss\n\
/////osyyyyhhhhhddddddddddddmmmmmmmmmddmmmdmmmmmdddhhhhhhso++////:`````.hmmmmm\
mmmmmmmmmmmmmmmddddhhh\n\
///+yhhhhhhdddddddddddddddmmmmmmmmmmdd/+hdmdmmddddddddhs++/////:.``````+mmmmmm\
mmmmmmmmmmmmmmmdmddddd\n\
///ydddddddddddddddddddddmmmmmmmmmmmmm+`-+syhhhhhhyyss+//////:.```````-dmmmmmm\
mmmmmmmmmmmmmmmddddddd\n\
:/odddddddddddddddddddddmmmmmmmmmmmmmmy` .////+++++///////::.````````-hmmmmmmm\
mmmmmmmmmmmmmmmmmdmddd\n\
-:yddddddddddddddddddddmmmmmmmmmmmmmmmd-  `::::://///::::-.`````````.ymmmmmmmm\
mmmmmmmmmmmmmmmmdddddd\n\
.-hdmdddddddddddddddddmmmmmmmmmmmmmmmmd+    `--::::::::-`` ````````.smmmmmmmmm\
mmmmmmmmmmmmmmmmdmdddd\n\
..hdmddddddddddddddddmmmmmmmmmmmmmmmmmmy`     `.-----.`    ````````/mmmmmmmmmm\
mmmmmmmmmmmmmmmmdmdddd\n\
.-hddmddddddddddddddmmmmmmmmmdmmmmdmmmmd.   `.+s//-`      ````````.hmmmmmmmmmm\
mmmmmmmmmNmmmmmmmmdmdd\n\
/odddmdddddddddddddmmmmmmmmmmddddddmmmmm/`.+yo++ooo/.`    ````````/mmmmmmmmmmm\
mmmmmmmNNmmmmmmmmddddm\n\
+hdddmddddddddddddmmmmmmmmmmddddddmmmmmmo-/:os+-::::/+:`  ```````.ymmmmmmmmmmm\
mmmmmNmmmmmmmmmmmddddm\n\
sddddmmdddddddddddmmmmmmmmmmddddddmmmmmmh+..-/+/--:/+oy+.````````:dmmmmmmmmmmm\
mmmmmmmmmmmmmmmmmmmmmm\n\
hddddmmdddddddddmmmmmmmmmmmddddddmdmmmmmmy.``-://:-/oso-....`````ommmmmmmmmmmm\
mmmmmmmmmmmmmmmmmmmdmm\n\
ymddmmmmdddddddmmmmmmmmmmmmddddddddmmmmmmy:`.-:-:/+++s:.```````.-hmmmmmmmmmmmm\
mmmmmmmmmmmmmmmmmmmmmm\n\
ydmmmmmmmdddddmmmmmmmmmmmmmmddddddmmmmmmmy+`.-+/-:+sys:````````:ommmmmmmmmmmmm\
mmmmmmmmmmmNmmmmmmmmmm\n\
dddmmmmmmmddmdmmmmmmmmmmmmmmdmdddmmmmmmmmho.`./o++osyy/.``````.:ymmmmmmmmmmmmm\
mmmmmmmmmmNNmmmmmmmmmm\n\
ddmdmmmmmmmmmmmmmmmmmmmmmmmmddddddmmmmmmmds-.-::++oshyo-.`.```-/dmmmmmmmmmmmmm\
mmmmmmmmmNNNmmmmmmmmmm\n\
dddmmmmmmmmmmmmmmmmmmmmmmmmmddddddmmmmmmmmy/.:::::+osss/.`.``.:ommmmmmmmmmmmmm\
mmmmmmmmNNNmmmmmmmmmmm\n\
mmddmmmmmmmmmmmmmmmmmmmmmmmmmdddddmmmmmmmmd+.//-:://+oo+-..``./hmmmmmmmmmmmmmm\
mmmmmmmNNNNmmmmmmmmmmm\n\
mmmmmmmmmmmmmmmmmmmmmmmmmmmmdddddddmmmmmmmmo-+::---/++//:-.`.:+mmmmmmmmmmmmmmm\
mmmmmmNNNNmmmmmmmmmmmm\n";


int builtin_courtois(char* argv[], struct hash_table *ht[])
{
  argv = argv;
  ht = ht;
  printf("%s%s", courtois, courtois2);
  return 0;
}
