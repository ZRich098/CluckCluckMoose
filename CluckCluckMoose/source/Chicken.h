/** The header file for Chickens */
#ifndef __CHICKEN_H__
#define __CHICKEN_H__
#include <cugl/cugl.h>

enum class element { 
	Fire,	//Rock
	Water,	//Scissors
	Grass,	//Paper
	LoseAll, 
	WinAll, 
	TieAll,
	SpecialWin,
	Unset	//Not set until stack time
};

/** Depreciated

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
}; */

enum class special {
	//Basic Chickens
	BasicFire,
	BasicWater,
	BasicGrass,
	Reaper, //Ties with all chickens
	BirdBrain, // Draw a Card
	Ninja, // Swap opponent top and bottom
	Clicken, //Refresh your hand
	PartyFowl, //Nullify opposing chicken
	Thicken, //Drop to bottom of stack
	Mirror, //Become opposing chicken
	Smoked, //Hide your next chicken played
	Spy, //Draw a card
	PartridgePilferer, //Draw 2 but lose to all chickens
	Witchen, //Cycle element of chicken underneath
	WingMan, //Play top chicken on deck as well
	Bomb, //Loses to everything but deals 2 damage
	PoultryPals, //Search for a fire chicken and draw it
	Angry, //Immediately trigger combat
	Lichen, //Opponent Discards a card
	Scientist, //Swap with Chicken immediately below
	Alchemist //Cycle all your Chickens except this one
};


/** String representation of the element */
string eString(element e);
/** String representation of the special ability */
string sString(special s);
/** Description of the special ability */
string sStringLong(special s);

/** Get the int representation of a element*/
int elementToInt(element e);

/** Get the int representation of a special (for json)*/
int specialToInt(special s);
/** Get the special representation of a int (for json)*/
special intToSpecial(int i);

class Chicken{
	private:
		element e;
		special s;
		int damage;
		bool cycled;
	public:
		//Constructors
		/** Creates a standard chicken card of special s */
		Chicken(special s);
		/** Create a Chicken of element e and special ability s that deals damage d */
		Chicken(element el, special sp, int d = 1) {
			e = el;
			s = sp;
			damage = d;
			cycled = false;
		}
		/** Copy Constructor */
		Chicken(const Chicken& c) {
			e = c.e;
			s = c.s;
			damage = c.damage;
			cycled = c.cycled;
		}
		/** Destroys the Chicken */
		~Chicken();

		//Access
		/** Returns the element of the Chicken */
		element const getElement() const { return e; };
		/** Returns the special ability of the Chicken */
		special const getSpecial() const { return s; };
		/** Returns the damage of the Chicken */
		int const getDamage() const { return damage; };
		/** Returns a string of the Chicken's details */
		string toString() const;
		/** Returns true if the chicken is cycled */
		bool const isCycled() const { return cycled; };
		/** Returns true if the chicken is a basic type */
		bool const isBasic() const { return (s == special::BasicFire || s == special::BasicGrass || s == special::BasicWater); };

		//Modify
		/** Sets the element of the Chicken */
		void setElement(element el) { e = el; };
		/** Sets the special ability of the Chicken */
		void setSpecial(special sp) { s = sp; };
		/** Sets the damage of the Chicken */
		void setDamage(int d) { damage = d; };
		/** Set a Chicken's element to e, special to s, and damage to d */
		void setChicken(element el, special sp, int d = 1) {
			//CULog("setting chicken");
			e = el;
			s = sp;
			damage = d;
		}
		/** Cycles the element of the Chicken
			(fire -> grass -> water -> fire) */
		void cycle();

		/** Compares this chicken with another chicken
			returns -1 if defeated by other chicken
			returns 0  if ties with other chicken
			returns 1  if defeats the other chicken */
		int compare(const Chicken& other) const;
};

#endif /* __CHICKEN_H__ */