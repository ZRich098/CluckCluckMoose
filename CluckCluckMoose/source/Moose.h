/** The header file for Moose*/
#ifndef __MOOSE_H__
#define __MOOSE_H__
#include <cugl/cugl.h>

#include "Coop.h"

/** The Moose Player */
class Moose {
private:
	//Pecking Order >:D This is actually a queue tho lol
	//0 is bottom of stack
	vector <Chicken> stack;	
	vector <Chicken> hand;
	Coop deck;
	vector <Chicken> discard;
	int handSize;
	int health;
	void refillDeck();
public:
	//Constructor
	/** Create a new Moose*/
	Moose(int health = 5, int handsize = 5, int deckNumber = 4); //see Decks.h for deck numbers, will eventually json (hopefully)
	/** Destroys the Moose*/
	~Moose();

	//Access
	/** Get the current pecking order of the Moose*/
	vector <Chicken> getStack() { return stack; };
	/** Get the Chicken at position pos in the stack of the Moose*/
	Chicken getStackAt(int pos) { return stack.at(pos); };
	/** Get the current hand of the Moose*/
	vector <Chicken> getHand() { return hand; };
	/** Get the Chicken at position pos in the hand of the Moose*/
	Chicken getHandAt(int pos) { return hand.at(pos); };
	/** Get the current draw pile of the Moose*/
	Coop getDeck() { return deck; };
	/** Get the Chicken at position pos in the Coop of the Moose*/
	Chicken getDeckAt(int pos) { return deck.at(pos); };
	/** Get the current discard pile of the Moose*/
	vector <Chicken> getDiscard() { return discard; };
	/** Get the Chicken at position pos in the discard pile of the Moose*/
	Chicken getDiscardAt(int pos) { return discard.at(pos); };
	/** Get the current health of the Moose*/
	int getHealth() { return health; };

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

	/**Sets the Chicken at position pos in stack to element e, special s, and damage d */
	void changeChickenInStack(element e, special s, int d = 1, int pos = 0);
	/**Sets the Chicken at position pos in stack to element e */
	void changeChickenInStackElement(element e, int pos = 0);
	/**Sets the Chicken at position pos in stack to special s */
	void changeChickenInStackSpecial(special s, int pos = 0);
	/**Sets the Chicken at position pos in stack to damage d */
	void changeChickenInStackDamage(int d, int pos = 0);

	//Hand
	/** Clears the hand of all Chickens */
	void clearHandToDiscard();
	/** Refills the hand with Chickens from the Coop */
	void refillHand();
	/** Draws num card from the Coop and adds it to the hand 
		Discards any cards drawn over the hand size (straight to discard pile)*/
	void draw(int num = 1);

	//Health
	/** Set the current health of the Moose*/
	void setHealth(int h) { health = h;};
	/** The moose loses $damage$ points of health*/
	void takeDamage(int damage) { health -= damage; };

	//Info
	/** returns a string of the relevant details about the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string mooseString() const;
	/** returns a string of the hand of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string handString() const;
	/** returns a string of the discard pile of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string discardString() const;
	/** returns a string of the stack of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string stackString() const;
	/** returns a string of the deck of the moose
		CAREFUL vectors are 0 indexed but info printed is 1 indexed*/
	string deckString() const { return deck.coopString(); };
};

#endif /* __MOOSE_H__ */