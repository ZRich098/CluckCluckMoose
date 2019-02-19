#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chicken.h"

using namespace cugl;

Chicken::Chicken() {
	Chicken(element::TieAll, special::None, 0);
}

Chicken::Chicken(element el, special sp) {
	e = el;
	s = sp;
	damage = 1;
}

Chicken::Chicken(element el, special sp, int d) {
	e = el;
	s = sp;
	damage = d;
}

void Chicken::setChicken(element el, special sp) {
	e = el;
	s = sp;
	damage = 1;
}

void Chicken::setChicken(element el, special sp, int d) {
	e = el;
	s = sp;
	damage = d;
}

element Chicken::getElement() {
	return e;
}

special Chicken::getSpecial() {
	return s;
}

string eString(element e) {
	switch (e) {
		case element::Fire:
			return "Fire";
		case element::Water:
			return "Water";
		case element::Grass: 
			return "Grass"; 
		//The following cases are all special chickens
		case element::TieAll:
		case element::WinAll:
		case element::LoseAll:
			return "";
	}	
}

string sString(special s) {
	switch (s) {
		case special::None:
			return "None";
		case special::Reaper:
			return "Reaper";
		case special::Draw:
			return "Bird Brain";
		case special::Ninja:
			return "Ninja";
		case special::Refresh:
			return "Clicken";
		case special::Null:
			return "Party Fowl";
		case special::Thicken:
			return "Thicken";
		case special::Mirror:
			return "Mirror";
		case special::Hide:
			return "Smoked";
		case special::Peek:
			return "Spy";
		case special::Draw2:
			return "Partridge Pilferer";
		case special::Cycle:
			return "Consigliere";
		case special::Extra:
			return "Wing Man";
		case special::Bomb:
			return "Bomb";
		case special::Search:
			return "Poultry Pals";
	}
}

string sStringLong(special s) {
	switch (s) {
		case special::None:
			return "Basic Chicken";
		case special::Reaper:
			return "Wins against any chicken, remove from the pecking order after combat. Does not deal damage. If negated, become a Basic Water Chicken";
		case special::Draw:
			return "Draw a Chicken"; //Do we want to use "hatch?"
		case special::Ninja:
			return "Swap your opponentfs bottommost and topmost Chicken";
		case special::Refresh:
			return "Discard your hand and draw that many Chickens";
		case special::Null:
			return "Negate the effects of the Chicken played against it";
		case special::Thicken:
			return "Move this Chicken to the bottom of the pecking order and move all Chickens you own up one space";
		case special::Mirror:
			return "Copy the ability and type of the Chicken played against this Chicken.  If the opposing Chicken is a Mirror Chicken or Party Fowl, become a Basic Fire Chicken";
		case special::Hide:
			return "Do not reveal your next played Chicken until clash time";
		case special::Peek:
			return "Look at 1 card in your opponentfs hand";
		case special::Draw2: //Is this worth playing?
			return "Draw two cards. Loses to all other chickens.  If negated, become a Basic Grass Chicken";
		case special::Cycle:
			return "Change the type of the Chicken below this one (Fire -> Water -> Grass -> Fire)";
		case special::Extra: //This seems too good to be true
			return "Play the top chicken in your deck on top of this one";
		case special::Bomb:
			return "Loses to everything.  Deal 2 damage to the enemy Moose.  If negated, become a Basic Fire Chicken";
		case special::PoultryPals:
			return "Reveal cards from your deck until you reveal a Fire chicken.  Add that card to your hand and reshuffle the rest.  If no chicken is revealed, end this effect and reshuffle";
	}
}

string Chicken::toString() {
    if(s == special::None) { //If Chicken special is none
        return "Basic " + eString(e) + " Chicken";
	}
	return sString(s) + " " + eString(e) + " Chicken";
}