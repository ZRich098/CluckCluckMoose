#include "Stack.h"

using namespace cugl;

void Stack::specialChickenEffect(vector <Chicken> stack1, vector <Chicken> stack2) {
	Chicken opposing = stack2.back();
	switch (stack1.back().getSpecial()) {
	case special::None:
		break;
	case special::Reaper:
		break; // Effects represented elsewhere
	case special::Draw:
		break; // Depreciated?
	case special::Ninja:
		iter_swap(stack2.begin(), stack2.back());
		break;
	case special::Refresh:
		break; // Depreciated?
	case special::Null:
		// Might want to make this a helper once stack.cpp is made or when we have too many no element chickens
		switch (opposing.getSpecial()) {
		case special::Reaper:
			opposing.setElement(element::Water);
			break;
		case special::Mirror:
			opposing.setElement(element::Fire);
		case special::Bomb:
			opposing.setElement(element::Fire);
		}
		opposing.setSpecial(special::None);
		opposing.setDamage(1);
		break;
	case special::Thicken:
		stack1.insert(stack1.begin(), stack1.pop_back); // May cause issues if the resolution order is unlike how I expect
		break;
	case special::Mirror:
		if (opposing.getSpecial != special::Mirror)
			stack1.back().setChicken(opposing.getElement, opposing.getSpecial, opposing.getDamage);
		else
			stack1.back().setChicken(element::Fire, special::None);
		break;
	case special::Hide:
		break; //TODO later
	case special::Peek:
		break; // maybe later
	case special::Draw2:
		break; // Depreciated?
	case special::Cycle:
		if (stack1.size >= 2)
			stack1.at(stack1.begin() + stack1.size - 2).cycle();
		break;
	case special::Extra:
		break; // probably not
	case special::Bomb:
		break; // Effects manifest elsewhere
	case special::Search:
		break; // Nope
	case special::Clash:
		break; // Wow we lost a lot of chickens along the way
	case special::Discard:
		break; // And another one
	case special::SelfSwap:
		iter_swap(stack1.at(stack1.begin() + stack1.size - 2), stack1.back());
		break;
	case special::CycleAll:
		break; // Pretty sure this is gone
	}
}

special Stack::whichSpecial(special s1, special s2) {
	if (s1 == special::None || s1 == special::Reaper || s1 == special::Bomb)
		return s1;
	if (s2 == special::None || s2 == special::Reaper || s2 == special::Bomb)
		return s2;
	if (s1 == special::Null)
		return s1;
	if (s2 == special::Null)
		return s2;
	if (s1 == special::Mirror)
		return s1;
	if (s2 == special::Mirror)
		return s2;
	if (s1 == special::Cycle)
		return s1;
	if (s2 == special::Cycle)
		return s2;
	if (s1 == special::SelfSwap)
		return s1;
	if (s2 == special::SelfSwap)
		return s2;
	if (s1 == special::Thicken)
		return s1;
	if (s2 == special::Thicken)
		return s2;
	if (s1 == special::Ninja)
		return s1;
	if (s2 == special::Ninja)
		return s2;
}
