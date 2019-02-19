#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Moose.h"

using namespace cugl;

/** Create a new Moose*/
Moose::Moose() {
	health = 5;
	handSize = 5;
}

/** Add the Chicken at position pos in the hand to the stack.  DOES remove the chicken from the hand. */
void Moose::addToStackFromHand(int pos) {
	stack.push_back(hand.at(pos));
	hand.erase(hand.begin() + pos);
}

/** Puts the Chicken at the top of the stack back into the hand*/
void Moose::removeTopFromStackToHand() {
	hand.push_back(stack.back());
	stack.pop_back();	
}

/** Puts all Chickens in the stack back into the hand*/
void Moose::removeAllFromStackToHand() {
	for (Chicken &c : stack) {
		hand.push_back(c);
	}
	stack.clear();
}

/** Puts the Chicken at the bottom of the stack into the discard pile */
void Moose::removeBottomFromStackToDiscard() {
	discard.push_back(stack.front());
	stack.erase(stack.begin());
}

/** Puts all Chickens in the stack into the discard pile */
void Moose::clearStackToDiscard() {
	for (Chicken &c : stack) {
		discard.push_back(c);
	}
	stack.clear();
}

/** Get the current pecking order of the Moose*/
vector <Chicken> Moose::getStack() {
	return stack;
}

/** Get the current hand of the Moose*/
vector <Chicken> Moose::getHand() {
	return hand;
}

/** Get the current draw pile of the Moose*/
Coop Moose::getDeck() {
	return deck;
}

/** Get the current discard pile of the Moose*/
vector <Chicken> Moose::getDiscard() {
	return discard;
}

/** Clears the hand of all Chickens */
void Moose::clearHand() {
	hand.clear();
}

/** Refills the hand with Chickens from the Coop */
void Moose::refillHand() {
	//Draw from deck while there are still chickens in the deck and hand is not full
	while (deck.getSize() > 0 && hand.size() < handSize) {
		hand.push_back(deck.draw());
	}

	if (hand.size() == handSize) return;

	//refill deck from discard pile if empty and hand is still not full
	deck.fill(discard);
	discard.clear();

	//Redraw
	while (deck.getSize() > 0 && hand.size() < handSize) {
		hand.push_back(deck.draw());
	}
}

/** Get the current health of the Moose*/
int Moose::getHealth() {
	return health;
}

/** Set the current health of the Moose*/
void Moose::setHealth(int h) {
	health = h;
}

/** The moose loses $damage$ points of health*/
void Moose::takeDamage(int damage) {
	health = health - damage;
}