/** The header file for Chickens */
#ifndef __CHICKEN_H__
#define __CHICKEN_H__
#include <cugl/cugl.h>

// TODO:: Entity Component elements and specials.
enum class element { 
	Fire,	//Rock
	Water,	//Scissors
	Grass,	//Paper
	LoseAll, 
	WinAll, 
	TieAll,
	Unset	//Not set until stack time
};

enum class special { 
	None, //No Special Ability
	Reaper, //Ties with all chickens
	Draw, // Draw a Card
	Ninja, // Swap opponent top and bottom
	Refresh, //Refresh your hand
	Null, //Nullify opposing chicken
	Thicken, //Drop to bottom of stack
	Mirror, //Become opposing chicken
	Hide, //Hide your next chicken played
	Peek, //Peek at an opponent chicken
	Draw2, //Draw 2 but lose to all chickens
	Cycle, //Cycle element of chicken underneath
	Extra, //Play top chicken on deck as well
	Bomb, //Loses to everything but deals 2 damage
	Search, //Search for a fire chicken and draw it
	Clash, //Immediately trigger combat
	Discard, //Opponent Discards a card
	SelfSwap, //Swap with Chicken immediately below
	CycleAll //Cycle all your Chickens except this one
}; 


/** String representation of the element*/
string eString(element e);
/** String representation of the special ability*/
string sString(special s);
/** Description of the special ability*/
string sStringLong(special s);

class Chicken{
	private:
		element e;
		special s;
		int damage;
	public:
		//Constructor
		/** Create a new Chicken*/
		Chicken();
		/** Create a Chicken of element e and special ability s */
		Chicken(element e, special s);
		/** Create a Chicken of element e and special ability s that deals damage d */
		Chicken(element e, special s, int d);

		//Access
		/** Returns the element of the Chicken */
		element getElement();
		/** Returns the special ability of the Chicken */
		special getSpecial();
		/** Returns a string of the Chicken's details */
		string toString();

		//Modify
		/** Set a Chicken's element to e, special to s, and damage to 1 */
		void setChicken(element e, special s);
		/** Set a Chicken's element to e, special to s, and damage to d */
		void setChicken(element e, special s, int d);
};

#endif /* __CHICKEN_H__ */