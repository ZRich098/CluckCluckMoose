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
	player->shuffleHand();
	hand = player->getHand();
	switch (type) {
		case AIType::Tutorial:
		case AIType::Loser:
			stack = Stack(player->getStack());
			oppStack = Stack(enemy->getStack());

			stackProcessed = Stack(stack);
			oppStackProcessed = Stack(oppStack);
			while (!stackProcessed.empty() && !oppStackProcessed.empty()) {
				stackProcessed.compare(oppStackProcessed);
			}
			break;
		case AIType::Dumb:
			break;
		case AIType::Intro:
			stack = Stack(player->getStack());
			break;
		case AIType::Beginner:
		case AIType::Basic:
		case AIType::Smart:
			stack = Stack(player->getStack());

			oppStack = enemy->getStack().substack(stack.getSize());
			oppHandSize = enemy->getHand().size();

			stackProcessed = Stack(stack);
			oppStackProcessed = Stack(oppStack);
			while (!stackProcessed.empty() && !oppStackProcessed.empty()) {
				stackProcessed.compare(oppStackProcessed);
			}
			break;
		case AIType::Expert:
			stack = Stack(player->getStack());

			oppStack = enemy->getStack().substack(stack.getSize());
			oppHandSize = enemy->getHand().size();

			stackProcessed = Stack(stack);
			oppStackProcessed = Stack(oppStack);
			while (!stackProcessed.empty() && !oppStackProcessed.empty()) {
				stackProcessed.compare(oppStackProcessed);
			}
			distribution = enemy->getChickenElementDistribution();
			break;
		default:
			CULogError("Unknown AI Type");
			return;
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

int AI::loserPlay() {
	hand = player->getHand();
	if (oppStackProcessed.empty()) return introPlay();
	if (stack.empty()) {
		Chicken tryToLoseToThis = oppStackProcessed.getBottom();

		for (int i = 0; i < hand.size(); i++) {
			if (hand.at(i).compare(tryToLoseToThis) == -1) return i;
		}
		return 0;
	}
	else {
		//look at all plays are different than our previous play
		special lastPlay = stack.getTop().getSpecial();
		vector <int> secondChoices;

		Chicken tryToLoseToThis = oppStackProcessed.getBottom();
		//CULog("Hand size: %d", hand.size());
		for (int i = 0; i < hand.size(); i++) {
			if (hand.at(i).getSpecial() != lastPlay) {
				int comparison = hand.at(i).compare(tryToLoseToThis);
				if (comparison == -1) return i;
				else if (comparison == 0) secondChoices.push_back(i);
			}
		}

		return secondChoices.empty() ? 0 : secondChoices.at(rand() % secondChoices.size());
	}
}

int AI::introPlay() {
	//if no previous play yet, play any random card from hand
	if (stack.empty()) return rand() % hand.size();
	
	//else look at all plays are different than our previous play and choose any random one
	special lastPlay = stack.getTop().getSpecial();
	vector <int> possPlays;

	for (int i = 0; i < hand.size(); i++)
		if (hand.at(i).getSpecial() != lastPlay) possPlays.push_back(i);

	return possPlays.empty() ? 0 : possPlays.at(rand() % possPlays.size());
}

int AI::defeatOpponentBottom(Chicken &c) {
	//If bomb chicken, try not to play unless it's the top
	if (c.getElement() == element::LoseAll && stack.getSize() != 4) return -2;

	if (oppStackProcessed.empty()) return 0;

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
		return 2;
	else if (result == 0) //if ties
		return -1;
	else //if loses to top chicken of our stack
		return 1;
}

int AI::typeBonusReverse(Chicken &c) {
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
	if (c2.getElement() == element::LoseAll) return -1000; //don't make plays so that bomb chicken isn't on top
	int result = c1.compare(c2);
	if (result == 1) //if beats
		return 1;
	else if (result == 0) //if ties
		return -2;
	else //if loses
		return 2;
}

int stackOrderingBonusReverse(Chicken &c1, Chicken &c2) {
	if (c2.getElement() == element::LoseAll) return -1000; //don't make plays so that bomb chicken isn't on top
	int result = c1.compare(c2);
	if (result == 1) //if beats
		return 2;
	else if (result == 0) //if ties
		return -2;
	else //if loses
		return 1;
}

int AI::beginnerPlay() {
	int highestScore = INT_MIN;
	int bestPlay = INT_MIN;

	for (int i = 0; i < hand.size(); i++) {
		Chicken &c = hand.at(i);
		int score = defeatOpponentBottom(c) + typeBonusReverse(c);

		if (score > highestScore) {
			highestScore = score;
			bestPlay = i;
		}
	}

	return bestPlay;
}

int AI::basicPlay() {
	int highestScore = INT_MIN;
	int bestPlay = INT_MIN;

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
	if (currentStack.getSize() == 5) {
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

	// Use hash map to store permutations by hash to ignore permutations that result in the same element after effects are resolved
	map<int, Stack> stackHashMap;
	addPermutationsToMap(stackHashMap, handPos, stack);

	map<int, Stack>::iterator it = stackHashMap.begin();
	int highestScore = INT_MIN;
	Stack bestStack = it->second;
	while (it != stackHashMap.end()) {
		Stack curStack = it->second;
		Stack oppCurStack = Stack(oppStack);

		Stack curSubstack = curStack.substack(oppStack.getSize() + 1);

		int score = processStackDamage(curSubstack, oppCurStack);

		score *= oppStack.getSize() * 2; //Weighted by amount of chickens already on stack

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

void addOppPermutationsToMap(map<int, Stack> &oppMap, vector <Chicken> hand, Stack currentStack) {
	if (currentStack.getSize() == 5) {
		int orderScore = 0;
		for (int i = 1; i < currentStack.getSize(); i++) {
			orderScore += stackOrderingBonus(currentStack.at(i), currentStack.at(i - 1));
		}
		if (orderScore > 4) oppMap.insert({ stackHash(currentStack), currentStack });
	}
	for (int i = 0; i < hand.size(); i++) {
		Stack newStack = Stack(currentStack);
		vector <Chicken> newHand = vector <Chicken>(hand);

		newStack.add(hand.at(i));
		newHand.erase(newHand.begin() + i);

		addOppPermutationsToMap(oppMap, newHand, newStack);
	}
}

int AI::expertPlay() {
	vector<int> handPos;
	for (int i = 0; i < hand.size(); i++) handPos.push_back(i);

	// Use hash map to store permutations by hash to ignore permutations that result in the same element after effects are resolved
	map<int, Stack> stackHashMap;
	addPermutationsToMap(stackHashMap, handPos, stack);

	//Guesstimate opponent hand
	vector<Chicken> oppHand;
	for (int i = 0; i < distribution.at(0); i++)
		oppHand.push_back(Chicken(special::BasicFire));
	for (int i = 0; i < distribution.at(1); i++)
		oppHand.push_back(Chicken(special::BasicWater));
	for (int i = 0; i < distribution.at(2); i++)
		oppHand.push_back(Chicken(special::BasicGrass));

	map<int, Stack> oppStackHashMap;
	addOppPermutationsToMap(oppStackHashMap, oppHand, oppStack);

	map<int, Stack>::iterator it = stackHashMap.begin();
	int highestScore = INT_MIN;
	Stack bestStack = it->second;

	while (it != stackHashMap.end()) {
		Stack curStack = it->second;

		//Score for how many permutations beat the opponent up
		int oppScore = 0;
		map<int, Stack>::iterator oit = oppStackHashMap.begin();
		while (oit != oppStackHashMap.end()) {
			oppScore += processStackDamage(curStack, oit->second);
			oit++;
		}

		//Score for diversity of chicken variation within own stack
		int orderScore = 0;
		for (int i = 1; i < curStack.getSize(); i++) {
			orderScore += stackOrderingBonus(curStack.at(i), curStack.at(i - 1));
		}
		
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

int AI::tutorialPlay() {
	int turn = stack.getSize();

	if (turn == 0) { //Play water chicken on first turn
		for (int i = 0; i < hand.size(); i++)
			if (hand.at(i).getSpecial() == special::BasicWater) return i;
		return 0;
	} else if (turn == 1) { //Play grass chicken on second turn
		for (int i = 0; i < hand.size(); i++)
			if (hand.at(i).getSpecial() == special::BasicGrass) return i;
		return 0;
	} else {
		return loserPlay();
	}
}

int AI::getPlay() {
	setup();

	switch (type) {
		case AIType::Loser:
			return loserPlay();
		case AIType::Dumb:
			return rand() % hand.size();
		case AIType::Intro:
			return introPlay();
		case AIType::Beginner:
			return beginnerPlay();
		case AIType::Basic:
			return basicPlay();
		case AIType::Smart:
			return smartPlay();
		case AIType::Expert:
			return expertPlay();

		case AIType::Tutorial:
			return tutorialPlay();

		default:
			CULogError("Unknown AI Type");
			return 0;
	}
}
