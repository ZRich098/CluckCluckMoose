/** The header file for Coops*/
#include <cugl/cugl.h>

#include "Coop.h"

/** The Deck of Chickens */
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
		Moose();

		//Access
		vector <Chicken> getStack();
		vector <Chicken> getHand();
		Coop getDeck();
		vector <Chicken> getDiscard();
		int getHealth();

		//Stack
		void addToStackFromHand(int pos);
		void removeTopFromStackToHand();
		void removeAllFromStackToHand();
		void removeBottomFromStackToDiscard();
		void clearStackToDiscard();

		//Hand
		void clearHand();
		void refillHand();

		//Health
		void setHealth(int health);
		void takeDamage(int damage);
};
