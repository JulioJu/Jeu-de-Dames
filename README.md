# Draughts

## Overview

It's a small Draughts video game (call « jeu de Dames » in French), implemented for the class of [« Méthodes informatiques et techniques de programmation » (Université Grenoble 1)](https://dlst.ujf-grenoble.fr/?module=ue&idue=123).

***This video game it's not totally finished.***


## Features

This game wants respect [WCDF Rules of Draughts](http://www.usacheckers.com/downloads/WCDF_Revised_Rules.doc ). 

This program valid every move of man according to the rules :
* « 1.13 The first move in each game is made by the player with the Whight men (…) »
* « 1.15 An ordinary move of a man is its transfer diagonally forward left or right from one square to an immediately neighbouring vacant square.  »
* « 1.16 When a man reaches the farthest row forward (known as the “king-row” or “crown-head”) it becomes a king, »
* « 1.18 A capturing move of a man is its transfer from one square over a diagonally adjacent and forward square occupied by an opponent\`s piece (man or king) and on to a vacant square immediately beyond it. (A capturing move is called a "jump"). On completion of the jump the captured piece is removed from the board. »
* « 1.19 If a jump creates an immediate further capturing opportunity, then the capturing move of the piece (man or king) is continued until all the jumps are completed. (…) At the end of the capturing sequence, all captured pieces are removed from the board. »
* « 1.20 All capturing moves are compulsory, whether offered actively or passively. If there are two or more ways to jump, a player may select any one that they wish, not necessarily that which gains the most pieces. Once started, a multiple jump must be carried through to completion. A man can only be jumped once during a multiple jumping sequence. »
* « 1.23 If a player on their turn to move touches a piece they must play that piece (…) »


## @TODO

* Modular programming. 
* Import a clock. 
* Change libraries, use the cross-platform development library [SDL](https://www.libsdl.org/).
* King is not totally finished. He can't move and capture an opponent's piece. 
* Display result of the game.
* Make a mode single player (play against the computer).


## Installation

1. Compile graphsimple.c: `gcc -c graphsimple.c`
2. Compile graphlib_w2.c: `gcc -c graphlib_w2.c`
3. Links: `ar -r libgraph.a raphsimple.o graphlib_w2.o`
4. Make libary: `ranlib libgraph.a`
5. Step 5

* For root user :
        1. Put include files `*.h` in `/usr/include`, and put the libary in `/usr/lib`.
        2. Compile: `gcc Demineur.c -lgraphe -lX11`

* For not root user :
        1. Put include files `*.h` in custom direcotry, for example `/home/toto/include`. Put libary in a custom directory lib, for example `/home/toto/lib`.
        2. Compile: `gcc monprog.c -I/home/toto/include -L/home/toto/lib -lgraphe -lX11`

## Licenses

* The file Jeu_de_Dames.c, by juanes10 in under MIT license.  Libaries Graphsimple and Graphlib_w2 are under copyright, see this files for credits.  ## Version 

Development started in december 2014. Last modified : january 2015. First realase : november 2015.
