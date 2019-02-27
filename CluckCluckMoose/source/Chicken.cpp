#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chicken.h"

using namespace cugl;

Chicken::Chicken(ChickenType t) {
	switch (t) {
	case ChickenType::BasicFire:
		e = element::Fire;
		s = special::None;
		damage = 1;
		return;
	case ChickenType::BasicWater:
		e = element::Water;
		s = special::None;
		damage = 1;
		return;
	case ChickenType::BasicGrass:
		e = element::Grass;
		s = special::None;
		damage = 1;
		return;
	case ChickenType::Reaper:
		e = element::TieAll;
		s = special::Reaper;
		damage = 0;
		return;
	case ChickenType::BirdBrain:
		e = element::Grass;
		s = special::Draw;
		damage = 1;
		return;
	case ChickenType::Ninja:
		e = element::Grass;
		s = special::Ninja;
		damage = 1;
		return;
	case ChickenType::Clicken:
		e = element::Water;
		s = special::Refresh;
		damage = 1;
		return;
	case ChickenType::PartyFowl:
		e = element::Water;
		s = special::Null;
		damage = 1;
		return;
	case ChickenType::Thicken:
		e = element::Fire;
		s = special::Thicken;
		damage = 1;
		return;
	case ChickenType::Mirror:
		e = element::Unset;
		s = special::Mirror;
		damage = 1;
		return;
	case ChickenType::Smoked:
		e = element::Fire;
		s = special::Hide;
		damage = 1;
		return;
	case ChickenType::Spy:
		e = element::Grass;
		s = special::Peek;
		damage = 1;
		return;
	case ChickenType::PartridgePilferer:
		e = element::LoseAll;
		s = special::Draw2;
		damage = 1;
		return;
	case ChickenType::Consigliere:
		e = element::Water;
		s = special::Cycle;
		damage = 1;
		return;
	case ChickenType::WingMan:
		e = element::Water;
		s = special::Extra;
		damage = 1;
		return;
	case ChickenType::Bomb:
		e = element::LoseAll;
		s = special::Bomb;
		damage = 2;
		return;
	case ChickenType::PoultryPals:
		e = element::Fire;
		s = special::Search;
		damage = 1;
		return;
	case ChickenType::Angry:
		e = element::Fire;
		s = special::Clash;
		damage = 1;
		return;
	case ChickenType::Lichen:
		e = element::Grass;
		s = special::Discard;
		damage = 1;
		return;
	case ChickenType::Scientist:
		e = element::Grass;
		s = special::SelfSwap;
		damage = 1;
		return;
	case ChickenType::Alchemist:
		e = element::Fire;
		s = special::CycleAll;
		damage = 1;
		return;
	}
}

Chicken::~Chicken() {
	//CULog("Destroyed %s.", this->toString().c_str());
}

void Chicken::cycle() {
	if (e == element::Fire) {
		e = element::Grass;
	} else if (e == element::Grass) {
		e = element::Water;
	} else if (e == element::Water) {
		e = element::Fire;
	}
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
		case element::Unset:
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
		case special::Clash:
			return "Angry";
		case special::Discard:
			return "Lich-en";
		case special::SelfSwap:
			return "Scientist";
		case special::CycleAll:
			return "Alchemist";
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
			return "Swap your opponent�fs bottommost and topmost Chicken";
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
			return "Look at 1 card in your opponent�fs hand";
		case special::Draw2: //Is this worth playing?
			return "Draw two cards. Loses to all other chickens.  If negated, become a Basic Grass Chicken";
		case special::Cycle:
			return "Change the type of the Chicken below this one (Fire -> Water -> Grass -> Fire)";
		case special::Extra: //This seems too good to be true
			return "Play the top chicken in your deck on top of this one";
		case special::Bomb:
			return "Loses to everything.  Deal 2 damage to the enemy Moose.  If negated, become a Basic Fire Chicken";
		case special::Search:
			return "Reveal cards from your deck until you reveal a Fire Chicken.  Add that Chicken to your Coop and reshuffle the rest.  If no chicken is revealed, end this effect and reshuffle";
		case special::Clash:
			return "Trigger a Clash";
		case special::Discard:
			return "Your opponent discards a Chicken";
		case special::SelfSwap:
			return "Swap this Chicken with the Chicken below it";
		case special::CycleAll:
			return "Cycle all your Chickens except this one (Fire -> Water -> Grass -> Fire)";
	}
}

string Chicken::toString() const {
	stringstream ss;
    if(s == special::None) { //If Chicken special is none
		ss << "Basic " << eString(e) << " Chicken";
	} else {
		ss << sString(s) << " " << eString(e) << " Chicken";
	}
	return ss.str();
}

int Chicken::compare(const Chicken& other) const {
	if (e == element::Unset || other.e == element::Unset) {
		CULogError("Unset element exception: %s and %s cannot be compared", toString(), other.toString());
	}

	//All ties
	if (e == other.e || e == element::TieAll || other.e == element::TieAll) return 0;

	switch (other.e) {
		case element::WinAll:
			return 1;
		case element::LoseAll:
			return -1;
	}

	switch (e) {
		case element::WinAll:
			return 1;
		case element::LoseAll:
			return -1;
		case element::Fire:
			return other.e == element::Grass ? 1 : -1;
		case element::Water:
			return other.e == element::Fire ? 1 : -1;
		case element::Grass:
			return other.e == element::Water ? 1 : -1;
	}

	//Never reached
	return -2;
}