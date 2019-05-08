#include <sstream>

#include "Stack.h"

using namespace cugl;

/** default special feedback length */
#define SPECIALLENGTH 6.0

/** values representing the states in special chicken effect, except the entry state which is handled in gamescene */
#define NONE 0
#define PLAYER 1
#define OPP 2
#define EXIT 3

Stack::Stack() {

}

Stack::~Stack() {
	//CULog("Stack Destroyed");
}

Stack Stack::substack(int size) {
	Stack s = Stack();
	s.chickens = vector<Chicken>(chickens.cbegin(), chickens.cbegin() + size);
	return s;
}

void Stack::add(const Chicken &c) {
	play_order.push_back(c.getSpecial());
	chickens.push_back(c);
}

Chicken &Stack::getBottom() {
	return chickens.front();
}

Chicken &Stack::getTop() {
	//CULog("Pointer of chicken: %p", &chickens.back());
	return chickens.back();
}

int const Stack::getDamage() {
	int total = 0;
	for (Chicken c : chickens) {
		total += c.getDamage();
	}
	return total;
}

Chicken Stack::removeBottom() {
	special s = chickens.front().getSpecial();
	chickens.erase(chickens.begin());
	return Chicken(s);
}


Chicken Stack::removeTop() {
	special s = chickens.back().getSpecial();
	chickens.erase(chickens.end()-1);
	return Chicken(s);
}

void Stack::swap(int pos1, int pos2) {
	iter_swap(&chickens.at(pos1), &chickens.at(pos2));
}

void Stack::insert(int pos, const Chicken &c) {
	chickens.insert(chickens.begin(), c);
}

void Stack::changeChickenInStack(element e, special s, int d, int pos) {
	chickens.at(pos).setChicken(e, s, d);
}

void Stack::changeChickenInStackElement(element e, int pos) {
	Chicken &c = chickens.at(pos);
	c.setChicken(e, c.getSpecial());
}

void Stack::changeChickenInStackSpecial(special s, int pos) {
	Chicken &c = chickens.at(pos);
	c.setChicken(c.getElement(), s);
}

void Stack::changeChickenInStackDamage(int d, int pos) {
	Chicken &c = chickens.at(pos);
	c.setChicken(c.getElement(), c.getSpecial(), d);
}

/* Note to Dylan : look at the spec for specialChickenEffects and make sure the 'addToStackFromHand' method does states similarly to how gamescene does. 
	Also, Hand and coop arent included in stack.h so Im not sure how that will work */

tuple<int,int> Stack::specialChickenEffect(Stack &opp, int skipState) {
	// lambda function, make cooldown an argument if/when it becomes necessary
	// tuple is same as return value of method
	auto setSkip = [](int state, int v) { return make_tuple((state == NONE ? v : EXIT), (state == NONE ? SPECIALLENGTH : 0)); };

	// Note that this method of skipping WILL cause issues in the event
	// that there is a special chicken that occurs after a ninja chicken
	special s1 = special::BasicFire;
	special s2 = special::BasicFire;
	if (skipState == 0) {
		s1 = getTop().getSpecial();
		s2 = opp.getTop().getSpecial();
	}
	else if (skipState == 1) {
		s2 = opp.getTop().getSpecial();
	}
	else if (skipState == 2) {
		s1 = getTop().getSpecial();
	}

	//CULog("\n%s", stackString().c_str());
	// Reaper, Bomb, and Basics are all represented by element and damage and do not need special effects

	if (s1 == special::PartyFowl || s2 == special::PartyFowl) {
		//CULog("resolving Party");
		s1 == special::PartyFowl ? partyHelper(opp.getTop()) : partyHelper(getTop());

		return setSkip(skipState, EXIT);
	}

	if (s1 == special::Mirror && s2 == special::Mirror) {
		getTop().setChicken(element::Grass, special::BasicGrass);
		opp.getTop().setChicken(element::Grass, special::BasicGrass);
		return setSkip(skipState, EXIT);
	}
	else if (s1 == special::Mirror) {
		//CULog("player mirror");
		s1 = s2;
		getTop().setChicken(opp.getTop().getElement(), opp.getTop().getSpecial(), opp.getTop().getDamage());
		return setSkip(skipState, NONE);
	}
	else if (s2 == special::Mirror) {
		//CULog("opp mirror");
		s2 = s1;
		opp.getTop().setChicken(getTop().getElement(), getTop().getSpecial(), getTop().getDamage());
		return setSkip(skipState, NONE);
	}

	//potentially TODO special::Peek

	if (s1 == special::Witchen && getSize() >= 2) {
		witchenPlayed = true;
		at(getSize() - 2).cycle();
		return setSkip(skipState, PLAYER);
	}
	if (s2 == special::Witchen && opp.getSize() >= 2) {
		witchenPlayed = true;
		opp.at(opp.getSize() - 2).cycle();
		return setSkip(skipState, OPP);
	}

	if (s1 == special::Thicken && getSize() >= 2) {
		swap(getSize() - 2, getSize() - 1);
		return setSkip(skipState, PLAYER);
	}
	if (s2 == special::Thicken && opp.getSize() >= 2) {
		opp.swap(opp.getSize() - 2, opp.getSize() - 1);
		return setSkip(skipState, OPP);
	}

	/*if (s1 == special::Thicken) {
		insert(0, getTop());
		removeTop();
		return setSkip(skipState, PLAYER);
	}
	if (s2 == special::Thicken) {
		opp.insert(0, opp.getTop());
		opp.removeTop();
		return setSkip(skipState, OPP);
	}*/

	//TODO special::Hide

	//potentially TODO special::Extra

	if (s1 == special::Spy) {
		//Can be made from here
	}
	if (s2 == special::Spy) {
		//see above
	}

	if (s1 == special::Ninja && s2 == special::Ninja) {
		swap(0, getSize() - 1);
		opp.swap(0, opp.getSize() - 1);
		return setSkip(skipState, EXIT);
	}
	else if (s1 == special::Ninja) {
		opp.swap(0, opp.getSize() - 1);
		return setSkip(skipState, EXIT);
	}
	else if (s2 == special::Ninja) {
		swap(0, getSize() - 1);
		return setSkip(skipState, EXIT);
	}

	//CULog("\n%s", stackString().c_str());

	// Exits state machine if no special chicken is found
	return make_tuple(EXIT, 0);
}


void Stack::specialChickenEffect() {
	special s1 = getTop().getSpecial();

	// Reaper, Bomb, and Basics are all represented by element and damage and do not need special effects
	
	if (s1 == special::Mirror) { //Mirror is considered TieAll for single stack
		getTop().setElement(element::TieAll);
	}

	if (s1 == special::Witchen && getSize() >= 2) {
		at(getSize() - 2).cycle();
	}

	if (s1 == special::Thicken && getSize() >= 2) {
		swap(getSize() - 2, getSize() - 1);
	}

	/*if (s1 == special::Thicken) {
		insert(0, getTop());
		removeTop();
	}*/

}

void partyHelper(Chicken& target) {
	switch (target.getSpecial()) {
	case special::Reaper:
		target.setChicken(element::Water, special::BasicWater);
		break;
	case special::Mirror:
		target.setChicken(element::Grass, special::BasicGrass);
		break;
	case special::Bomb:
		target.setChicken(element::Fire, special::BasicFire);
		break;
	case special::Ninja:
		target.setChicken(element::Fire, special::BasicFire);
		break;
	case special::PartyFowl:
		break;
	case special::Spy:
		target.setChicken(element::Fire, special::BasicFire);
		break;
	case special::Thicken:
		target.setChicken(element::Grass, special::BasicGrass);
		break;
	case special::Witchen:
		target.setChicken(element::Water, special::BasicWater);
		break;
	default:
		//CULog("reached default");
		break;
	}
}

string Stack::stackString() const {
	stringstream ss;
	for (int i = 0; i < chickens.size(); i++) {
		ss << "Stack " << i + 1 << ": " << chickens.at(i).toString().c_str() << "\n";
	}

	return ss.str();
}

int Stack::compareWithoutRemove(Stack &opp) {
	if (!empty() && !opp.empty()) {
		int result = getBottom().compare(opp.getBottom());
		if (result == -1)
		{
			//CULog("opp win");
			getBottom();
		}
		else if (result == 1)
		{
			//CULog("player win");
			opp.getBottom();
		}
		else
		{
			//CULog("tie");
			getBottom();
			opp.getBottom();
		}
		return result;
	}
	else {
		//CULog("compare called on empty stacks");
		return -2;
	}
}

int Stack::compare(Stack &opp) {
	if (!empty() && !opp.empty()) {
		int result = getBottom().compare(opp.getBottom());
		if (result == -1)
		{
			//CULog("opp win");
			removeBottom();
		}
		else if (result == 1)
		{
			//CULog("player win");
			opp.removeBottom();
		}
		else
		{
			//CULog("tie");
			removeBottom();
			opp.removeBottom();
		}
		return result;
	}
	else {
		//CULog("compare called on empty stacks");
		return -2;
	}
}