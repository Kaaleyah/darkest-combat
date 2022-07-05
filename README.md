# Darkest Combat
## Terminal Based Darkest Dungeon Clone

This clone is written for the project of BAU/CMP1002 lecture. 

This project is inspired from Darkest Dungeon by Redhook Studios. It showcases combat part of the game. 

### Installation
Cloning the project and compiling it in Clion is enough. 

### How to play
You are 4 heroes against 4 monsters. Game is turn based and you need select a skill that are constrained by position. 

    
```bash
[1] Lovell - C HP: 33 protection: 0  will play

Hero Team
[1] Lovell - C HP: 33 protection: 0 [2] Pericard - V HP: 24 protection: 0 
[3] Valance - C HP: 33 protection: 0 [4] Pipin - V HP: 24 protection: 0 

Monster Team
[1] Buttons - S HP: 10 protection: 15 [2] Whitewash - D HP: 22 protection: 45 
[3] Fetidling - D HP: 22 protection: 45 [4] Brinecat - S HP: 10 protection: 15 

| (1) Smite (2) StunningBlow (3) HolyLance (4) BulwarkOfFaith (5) Move |
Choose skill index: 
```

For example, turn is on Lovell. He can choose to play a skill or move to change his position. Position is critical because skill are only available to certain positions and they can be applied to certain positions.
```bash
Choose skill index: 2

Choose an opponent to attack: { 1 2 }
1
Lovell - C used StunningBlow on Buttons - S
Hit, 6 damage
Buttons - S is stunned!
Turn finished. Press enter to continue...
```
Lovell chose *Stunning Blow* and he dealt damage to Buttons. 

```bash
| (1) MaceBash (2) DazzlingLight (3) DivineGrace (4) DivineComfort (5) Move |
Choose skill index: 3

DivineGrace
You cannot use this skill at this specific position
You can only use this skill at these specific stated positions: {3  4}
| (1) MaceBash (2) DazzlingLight (3) DivineGrace (4) DivineComfort (5) Move |
Choose skill index: 
```
If you cannot use that skill in character's position, you will prompted with playable positions.

```bash
| (1) MaceBash (2) DazzlingLight (3) DivineGrace (4) DivineComfort (5) Move |
Choose skill index: 4

Pericard - V has healed Lovell - C by 3
Pericard - V has healed Pericard - V by 1
Pericard - V has healed Valance - C by 3
Pericard - V has healed Pipin - V by 1
Turn finished. Press enter to continue...
```
It is also possible heal your allies by some skills such as *Divine Comfort*.

```bash
[4] Brinecat - S HP: 10 protection: 15  will play

Hero Team
[1] Lovell - C HP: 33 protection: 0 [2] Valance - C HP: 32 protection: 0 
[3] Pericard - V HP: 24 protection: 0 [4] Pipin - V HP: 24 protection: 0 

Monster Team
[1] Buttons - S HP: 4 protection: 15 [2] Whitewash - D HP: 22 protection: 45 
[3] Fetidling - D HP: 21 protection: 45 [4] Brinecat - S HP: 10 protection: 15 

Brinecat - S moved from 4 to 2
Turn finished. Press enter to continue...
```
Monster team will play by rolling a dice to choose the skill and opponent.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
