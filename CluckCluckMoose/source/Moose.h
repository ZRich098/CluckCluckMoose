/** The header file for Coops*/
#include <cugl/cugl.h>

#include "Coop.h"

/** The Moose Player */
class Moose {
	private:
		vector <Chicken> stack;	//Pecking Order >:D This is actually a queue tho lol
		vector <Chicken> hand;	
		Coop deck;					
		vector <Chicken> discard;	
		int handSize;
		int health;
	public:
		//Constructor
		/** Create a new Moose*/
		Moose();

		//Access
		/** Get the current pecking order of the Moose*/
		vector <Chicken> getStack();
		/** Get the current hand of the Moose*/
		vector <Chicken> getHand();
		/** Get the current draw pile of the Moose*/
		Coop getDeck();
		/** Get the current discard pile of the Moose*/
		vector <Chicken> getDiscard();
		/** Get the current health of the Moose*/
		int getHealth();

		//Stack
		/** Add the Chicken at position pos in the hand to the stack.  DOES remove the chicken from the hand. */
		void addToStackFromHand(int pos);
		/** Puts the Chicken at the top of the stack back into the hand*/
		void removeTopFromStackToHand();
		/** Puts all Chickens in the stack back into the hand*/
		void removeAllFromStackToHand();
		/** Puts the Chicken at the bottom of the stack into the discard pile */
		void removeBottomFromStackToDiscard();
		/** Puts all Chickens in the stack into the discard pile */
		void clearStackToDiscard();

		//Hand
		/** Clears the hand of all Chickens */
		void clearHand();
		/** Refills the hand with Chickens from the Coop */
		void refillHand();

		//Health
		/** Set the current health of the Moose*/
		void setHealth(int health);
		/** The moose loses $damage$ points of health*/
		void takeDamage(int damage);
};
