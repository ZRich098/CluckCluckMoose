//
//  Moose.cpp
//

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Moose.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Constructors

void Moose::refillDeck() {
	deck.fill(discard);
	discard.clear();
}

void Moose::refillHandPool() {
	vector<Chicken> hand;
	vector<int> options;
	int opts[] = { 3,5,7,8,9,11,13,15 }; //implemented specials: 3,5,7,8,9,11,13,15
	for (int i : opts) {
		options.push_back(i);
	}
	for (int i = 0; i < 3; i++) {
		int rand = std::rand() % options.size();
		hand.push_back(intToSpecial(options.at(rand)));
		handPool.push_back(hand);
	}
}

/**
 * Initializes a new moose with the given health and hand size.
 *
 * @return  true if the Moose is initialized properly, false otherwise.
 */
bool Moose::init(int h, int hSize, bool tutor, bool player) {
	health = h;
	handSize = hSize;
	numChickensWillDiePreview = 0;
	isTutor = tutor;
	for (int i = 0; i < 5; i++) vecChickensClashPreview.push_back(-1);
	if (tutor) {
		isPlayer = player;
		costume = "basic_moose";
	}
    return true;
}


void Moose::jsonInit(int h, vector<int> handArray, vector<int> playOrderArray, vector<int> coopArray, string cost) {
	health = h;
	
	for (int i : handArray) {
		hand.push_back(Chicken(intToSpecial(i)));
	}

	for (int i : playOrderArray) {
		playOrder.push_back(Chicken(intToSpecial(i)));
	}

	deck.clear();
	deck.fill(coopArray);

	if (handArray.size() == 0) {
		refillHand();
	}

	/*for (int i : discardArray) {
		discard.push_back(Chicken(intToSpecial(i)));
	}*/

	//not yet supported
	costume = cost;
}

void Moose::jsonInit(int h, vector<int> handArray, vector<int> playOrderArray, vector<vector<int>> handPoolArray, string cost) {
	health = h;
	for (int i : handArray) {
		hand.push_back(Chicken(intToSpecial(i)));
	}

	for (int i : playOrderArray) {
		playOrder.push_back(Chicken(intToSpecial(i)));
	}

	handPool.clear();
	if (handPoolArray.size() == 0) {
		refillHandPool();
	}
	for (vector<int> h : handPoolArray) {
		vector<Chicken> hand;
		for (int i : h) {
			hand.push_back(Chicken(intToSpecial(i)));
		}
		handPool.push_back(hand);
	}

	if (handArray.size() == 0) {
		refillHand();
	}

	/*for (int i : discardArray) {
		discard.push_back(Chicken(intToSpecial(i)));
	}*/

	//not yet supported
	costume = cost;
}


/**
 * Disposes all resources and assets of this moose
 *
 * Does not currently actually dispose all assets!
 */
void Moose::dispose() {
    health = 0;
    handSize = 0;
    // deck = Coop(4);
}


vector<int> Moose::getChickenElementDistribution() {
	int fire = 0;
	int water = 0;
	int grass = 0;
	int other = 0;

	for (Chicken c : hand) {
		switch (c.getElement()) {
		case element::Fire:
			fire += 1;
			break;
		case element::Water:
			water += 1;
			break;
		case element::Grass:
			grass += 1;
			break;
		default:
			if (nonEleDist.size() <= other)
				nonEleDist.push_back(rand() % 3);
			if (nonEleDist.at(other) == 0)
				fire += 1;
			if (nonEleDist.at(other) == 1)
				water += 1;
			if (nonEleDist.at(other) == 2)
				grass += 1;
			other += 1;
			break;
		}
	}

	vector<int> res;
	res.push_back(fire);
	res.push_back(water);
	res.push_back(grass);
	res.push_back(other);

	return res;
}


#pragma mark -
#pragma mark Stack

void Moose::addToStackFromHand(int pos) {
	//CULog("playing at pos: %d", pos);
	//CULog("playing %s", hand.at(pos).toString().c_str());
	stack.add(hand.at(pos));
	hand.erase(hand.begin() + pos);
	//CULog("new hand size: %d", hand.size());
	playOrder.push_back(getStack().getTop());
}

void Moose::removeTopFromStackToHand() {
	hand.push_back(stack.removeTop());
}

void Moose::discardStack() {
	while (playOrder.size() > 0) {
		switch (playOrder.front().getSpecial()) {
			case special::BasicFire:
			case special::BasicGrass:
			case special::BasicWater:
				break;
			default:
				discard.push_back(playOrder.front());
		}
		playOrder.erase(playOrder.begin());
	}
	playOrder.clear();
}

/*void Moose::clearStackToDiscard() {
	while(stack.getSize() > 0){
		discard.push_back(stack.removeBottom());
	}
	stack.clear();
}*/

void Moose::setStack(Stack s) {
	stack.clear();
	while ((s.getSize() > 0)) {
		stack.add(s.getBottom());
		s.removeBottom();
	}
}

void Moose::discardChickens() {
	// booleans to check if one of each basic is erased
	bool fireRemoved, waterRemoved, grassRemoved = false;
	//for debugging purposes
	int basicCounter;
	// concatenates playOrder and hand for iterating, this is fine since they are both cleared at the end
	playOrder.insert(playOrder.end(), hand.begin(), hand.end());

	for (Chicken &c : playOrder) {
		// basics are added to the deck directly so they aren't blacklisted at the end of the method
		switch (c.getSpecial()) {
		case special::BasicFire:
			if (!fireRemoved)
				fireRemoved = true;
			else {
				basicCounter++;
				deck.add(c);
			}
			break;
		case special::BasicGrass:
			if (!grassRemoved) 
				grassRemoved = true;
			else {
				basicCounter++;
				deck.add(c);
			}
			break;
		case special::BasicWater:
			if (!waterRemoved)
				waterRemoved = true;
			else {
				basicCounter++;
				deck.add(c);
			}
			break;
		default:
			discard.push_back(c);
			break;
		}
	}

	if (discard.size() + basicCounter != playOrder.size() - 3)
		CULog("Warning: Basic chicken discard inconsistency.");
	blacklist = discard;
	refillDeck();
	deck.shuffle();

	playOrder.clear();
	nonEleDist.clear();
	hand.clear();
	CULog("blacklist size: %d", blacklist.size());
}

void Moose::clearHandToDiscard() {
	for (Chicken &c : hand) {
		switch (c.getSpecial()) {
		case special::BasicFire:
		case special::BasicGrass:
		case special::BasicWater:
			break;
		default:
			discard.push_back(c);
		}
	}
	nonEleDist.clear();
	hand.clear();
}

void Moose::refillHand() {
	if (isTutor) {
		if (hand.size() > 0) {
			hand.clear();
		}
		if (isPlayer) {
			hand.push_back(Chicken(element::Fire, special::BasicFire));
			hand.push_back(Chicken(element::Grass, special::BasicGrass));
			hand.push_back(Chicken(element::Water, special::BasicWater));
			hand.push_back(Chicken(element::Grass, special::BasicGrass));
			hand.push_back(Chicken(special::Spy));
			hand.push_back(Chicken(special::Spy));
			return;
		}
		else {
			hand.push_back(Chicken(element::Fire, special::BasicFire));
			hand.push_back(Chicken(element::Grass, special::BasicGrass));
			hand.push_back(Chicken(element::Water, special::BasicWater));
			hand.push_back(Chicken(element::Fire, special::BasicFire));
			hand.push_back(Chicken(special::Spy));
			hand.push_back(Chicken(special::Spy));
			return;
		}
	}
	//Draw from deck while there are still chickens in the deck and hand is not full
	if (hand.size() == 0) {
		//CULog("refilling basics");
		hand.push_back(Chicken(element::Fire, special::BasicFire));
		hand.push_back(Chicken(element::Grass, special::BasicGrass));
		hand.push_back(Chicken(element::Water, special::BasicWater));
	}
	random_shuffle(hand.begin(),hand.end());
	//Uses hands that are given if one is available
	if (handPool.size() != 0) {
		//CULog("refilling with handPool");
		random_shuffle(handPool.begin(), handPool.end());
		for (Chicken i : handPool.front()) {
			hand.push_back(i);
		}
	}
	//Uses the pool system otherwise
	else {
		//CULog("refilling with pool");
		int specialNum = 0;
		while (hand.size() < handSize) {
			//If deck is empty for whatever reason, draw random basic
			if (deck.getSize() == 0) {
				int random = rand() % 3;
				if (random == 0)
					hand.push_back(Chicken(element::Fire, special::BasicFire));
				if (random == 1)
					hand.push_back(Chicken(element::Grass, special::BasicGrass));
				if (random == 2)
					hand.push_back(Chicken(element::Water, special::BasicWater));
			}
			else if (deck.getSizeB() != 0 && (deck.getSizeS() == 0 || specialNum == 2))
				hand.push_back(deck.getBasic());
			else {
				hand.push_back(deck.getSpecial());
				specialNum++;
			}
		}
		// Deprecated Pool system portion
		//refillDeck();
		//deck.shuffle();
	}
	//CULog("%s", handString().c_str());
}

void Moose::draw(int num) {
	if (isTutor) {
		//CULog("tutorial drawing %i", num);
		for (int i = 0; i < num; i++) {
			if (hand.size() < handSize) {
				int random = rand() % 3;
				if (random == 0)
					hand.push_back(Chicken(element::Fire, special::BasicFire));
				if (random == 1)
					hand.push_back(Chicken(element::Grass, special::BasicGrass));
				if (random == 2)
					hand.push_back(Chicken(element::Water, special::BasicWater));
			}
		}
		return;
	}
	for (int i = 0; i < num; i++) {
		if (hand.size() < handSize) {
			int random = rand() % 3;
			if (random == 0)
				hand.push_back(Chicken(element::Fire, special::BasicFire));
			if (random == 1)
				hand.push_back(Chicken(element::Grass, special::BasicGrass));
			if (random == 2)
				hand.push_back(Chicken(element::Water, special::BasicWater));
		}
	}
}

string Moose::handString() const {
	stringstream ss;
	for (int i = 0; i < hand.size(); i++) {
		ss << "Hand " << i + 1 << ": " << hand.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	return ss.str();
}

string Moose::playOrderString() const {
	stringstream ss;
	for (int i = 0; i < playOrder.size(); i++) {
		ss << "PlayOrder " << i + 1 << ": " << playOrder.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	return ss.str();
}

string Moose::discardString() const {
	stringstream ss;
	for (int i = 0; i < discard.size(); i++) {
		ss << "Discard " << i + 1 << ": " << discard.at(i).toString().c_str() << "\n";
	}
	ss << "\n";
	return ss.str();
}

string Moose::mooseString() const {
	stringstream ss;
	ss << "Moose Info:\n Health " << health << "\n" <<
		handString() << stackString() << discardString() <<
		deck.coopString();
	return ss.str();
}