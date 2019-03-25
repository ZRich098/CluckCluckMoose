//
//  AI.cpp
//
#include "AI.h"
#include <iterator>

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Constructors

/**
 * Initializes a new AI
 *
 * @return  true if the AI is initialized properly, false otherwise.
 */
bool AI::init(std::shared_ptr<Moose> p, std::shared_ptr<Moose> e, AIType at) {
	player = p;
	enemy = e;
	type = at;

	return true;
}

int processStackDamage(Stack &you, Stack &opp) {
	while (!you.empty() && !opp.empty()) {
		you.compare(opp);
	}
	return you.getDamage() - opp.getDamage();
}

void AI::setup() {
	switch (type) {
		case AIType::Expert:
			distribution = enemy->getChickenElementDistribution();
		case AIType::Smart:
		case AIType::Basic:
			stack = Stack(player->getStack());
			hand = player->getHand();
			oppStack = Stack(enemy->getStack());
			oppHandSize = enemy->getHand().size();

			stackProcessed = Stack(stack);
			oppStackProcessed = Stack(oppStack);
			while (!stackProcessed.empty() && !oppStackProcessed.empty()) {
				stackProcessed.compare(oppStackProcessed);
			}
		case AIType::Dumb:
			break;
		default:
			CULogError("Unknown AI Type");
	}
}


/**
 * Disposes all resources and assets of this AI
 */
void AI::dispose() {
	//Stack based objects automatically deleted

	player = nullptr;
	enemy = nullptr;
	//typ = AIType::Dumb; // default to Dumb because it won't take a nullptr
}

int AI::defeatOpponentBottom(Chicken &c) {
	if (oppStackProcessed.empty()) return 0;

	CULog("OppStackProBottom:, %s", oppStackProcessed.getBottom().toString().c_str());

	int result = c.compare(oppStackProcessed.getBottom());
	if (result == 1) //if beats opponent chicken
		return 3;
	else if (result == 0) //if ties
		return 1;
	else //if loses to opponent bottom
		return -1;
}

int AI::typeBonus(Chicken &c) {
	if (stack.empty()) return 0;

	int result = stack.getTop().compare(c);
	if (result == 1) //if beaten by top chicken of our stack
		return 1;
	else if (result == 0) //if ties
		return -1;
	else //if loses to top chicken of our stack
		return 2;
}

int stackOrderingBonus(Chicken &c1, Chicken &c2) {
	if (c2.getElement() == element::LoseAll) return -1000;
	int result = c1.compare(c2);
	if (result == 1) //if beats
		return 1;
	else if (result == 0) //if ties
		return -1;
	else //if loses
		return 2;
}

int AI::basicPlay() {
	int highestScore = 0;
	int bestPlay = 0;

	for (int i = 0; i < hand.size(); i++) {
		Chicken &c = hand.at(i);
		int score = defeatOpponentBottom(c) + typeBonus(c);

		if (score > highestScore) {
			highestScore = score;
			bestPlay = i;
		}
	}

	return bestPlay;
}

int stackHash(Stack s) {
	int value = 0;

	for (int i = 0; i < s.getSize(); i++) {
		value += elementToInt(s.at(i).getElement()) * 8 * (i + 1);
	}
	return value;
}


void AI::addPermutationsToMap(map<int, Stack> &stackHashMap, vector <int> pos, Stack currentStack) {
	if (pos.empty()) {
		stackHashMap.insert({stackHash(currentStack), currentStack });
	}
	for (int i = 0; i < pos.size(); i++) {
		Stack newStack = Stack(currentStack);
		vector <int> newPos = vector <int>(pos);

		newStack.add(hand.at(newPos.at(i)));
		newStack.specialChickenEffect();
		newPos.erase(newPos.begin() + i);

		addPermutationsToMap(stackHashMap, newPos, newStack);
	}
}

int AI::smartPlay() {
	vector<int> handPos;
	for (int i = 0; i < hand.size(); i++) handPos.push_back(i);

	map<int, Stack> stackHashMap;
	addPermutationsToMap(stackHashMap, handPos, stack);

	map<int, Stack>::iterator it = stackHashMap.begin();
	int highestScore = INT_MIN;
	Stack bestStack = it->second;
	while (it != stackHashMap.end()) {
		Stack curStack = it->second;

		int score = processStackDamage(curStack.substack(oppStack.getSize()),oppStack) * 5; //5 cards on stack

		for (int i = 1; i < curStack.getSize(); i++) {
			score += stackOrderingBonus(curStack.at(i), curStack.at(i - 1));
		}

		if (score > highestScore) {
			highestScore = score;
			bestStack = curStack;
		}

		it++;
	}

	//Search through hand for best play
	for (int i = 0; i < hand.size(); i++) {
		if (hand.at(i).getSpecial() == bestStack.getPlayOrder().at(stack.getSize())) return i;
	}

	CULogError("Best Play not found in hand- talk to Richard");
	return 0;
}

void addOppPermutationsToMap(map<int, Stack> &stackHashMap, vector <Chicken> hand, Stack currentStack) {
	if (hand.empty()) {
		stackHashMap.insert({ stackHash(currentStack), currentStack });
	}
	for (int i = 0; i < hand.size(); i++) {
		CULog("hand size %i", hand.size());
		Stack newStack = Stack(currentStack);
		vector <Chicken> newHand = vector <Chicken>(hand);

		newStack.add(hand.at(i));
		newHand.erase(newHand.begin() + i);

		addOppPermutationsToMap(stackHashMap, newHand, newStack);
	}
}

int AI::expertPlay() {
	vector<int> handPos;
	for (int i = 0; i < hand.size(); i++) handPos.push_back(i);

	map<int, Stack> stackHashMap;
	addPermutationsToMap(stackHashMap, handPos, stack);

	vector<Chicken> oppHand;
	for (int i = 0; i < distribution.at(0); i++)
		oppHand.push_back(Chicken(special::BasicFire));
	for (int i = 0; i < distribution.at(1); i++)
		oppHand.push_back(Chicken(special::BasicWater));
	for (int i = 0; i < distribution.at(2); i++)
		oppHand.push_back(Chicken(special::BasicGrass));
	for (int i = 0; i < distribution.at(3); i++)
		oppHand.push_back(Chicken(element::TieAll, special::Reaper));

	map<int, Stack> oppStackHashMap;
	addOppPermutationsToMap(oppStackHashMap, oppHand, oppStack);

	map<int, Stack>::iterator it = stackHashMap.begin();
	int highestScore = INT_MIN;
	Stack bestStack = it->second;

	while (it != stackHashMap.end()) {
		Stack curStack = it->second;

		int oppScore = 0;

		map<int, Stack>::iterator oit = oppStackHashMap.begin();
		while (oit != oppStackHashMap.end()) {
			oppScore += processStackDamage(curStack, oit->second);
			oit++;
		}
		oppScore *= 5; //5 cards on stack

		int orderScore = 0;
		for (int i = 1; i < curStack.getSize(); i++) {
			orderScore += stackOrderingBonus(curStack.at(i), curStack.at(i - 1));
		}
		orderScore *= oppStackHashMap.size();
		
		int score = oppScore + orderScore;

		if (score > highestScore) {
			highestScore = score;
			bestStack = curStack;
		}

		it++;
	}

	//Search through hand for best play
	for (int i = 0; i < hand.size(); i++) {
		if (hand.at(i).getSpecial() == bestStack.getPlayOrder().at(stack.getSize())) return i;
	}

	CULogError("Best Play not found in hand- talk to Richard");
	return 0;
}

int AI::getPlay() {
	setup();

	switch (type) {
		case AIType::Dumb:
			return 0;
		case AIType::Basic:
			return basicPlay();
		case AIType::Smart:
			return smartPlay();
		case AIType::Expert:
			return expertPlay();

		default:
			CULogError("Unknown AI Type");
			return 0;
	}
}
