#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Chicken.h"

using namespace cugl;
using namespace std;

Chicken::Chicken(special sp) {
	s = sp;
	damage = 1;
	cycled = false;
	switch (s) {
	case special::BasicFire:
		e = element::Fire;
		return;
	case special::BasicWater:
		e = element::Water;
		return;
	case special::BasicGrass:
		e = element::Grass;
		return;
	case special::Reaper:
		e = element::SpecialWin;
		//damage = 0;
		return;
	case special::BirdBrain:
		e = element::Grass;
		return;
	case special::Ninja:
		e = element::Fire;
		return;
	case special::Clicken:
		e = element::Water;
		return;
	case special::PartyFowl:
		e = element::Water;
		return;
	case special::Thicken:
		e = element::Grass;
		return;
	case special::Mirror:
		e = element::Unset;
		return;
	case special::Smoked:
		e = element::Fire;
		return;
	case special::Spy:
		e = element::Fire;
		return;
	case special::PartridgePilferer:
		e = element::LoseAll;
		return;
	case special::Witchen:
		e = element::Water;
		return;
	case special::WingMan:
		e = element::Water;
		return;
	case special::Bomb:
		e = element::LoseAll;
		damage = 2;
		return;
	case special::PoultryPals:
		e = element::Fire;
		return;
	case special::Angry:
		e = element::Fire;
		return;
	case special::Lichen:
		e = element::Grass;
		return;
	case special::Scientist:
		e = element::Grass;
		return;
	case special::Alchemist:
		e = element::Fire;
		return;
	}
}

Chicken::~Chicken() {
	//CULog("Destroyed %s.", this->toString().c_str());
}

void Chicken::cycle() {
	//CULog("%s", toString().c_str());
	if (e == element::Fire) {
		e = element::Water;
		if (s == special::BasicFire) {
			s = special::BasicWater;
		}
	} else if (e == element::Grass) {
		e = element::Fire;
		if (s == special::BasicGrass) {
			s = special::BasicFire;
		}
	} else if (e == element::Water) {
		e = element::Grass;
		if (s == special::BasicWater) {
			s = special::BasicGrass;
		}
	}
	cycled = true;
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
        default:
            return "";
	}	
}

int elementToInt(element e) {
	switch (e) {
	case element::Fire:
		return 1;
	case element::Water:
		return 2;
	case element::Grass:
		return 3;
	case element::TieAll:
		return 4;
	case element::WinAll:
		return 5;
	case element::LoseAll:
		return 6;
	case element::Unset:
		return 7;
	case element::SpecialWin:
		return 8;
	}
}


/** Get the int representation of a special (for json)*/
int specialToInt(special s) {
	switch (s) {
	case special::BasicFire:
		return 0;
	case special::BasicGrass:
		return 1;
	case special::BasicWater:
		return 2;
	case special::Reaper:
		return 3;
	case special::BirdBrain:
		return 4;
	case special::Ninja:
		return 5;
	case special::Clicken:
		return 6;
	case special::PartyFowl:
		return 7;
	case special::Thicken:
		return 8;
	case special::Mirror:
		return 9;
	case special::Smoked:
		return 10;
	case special::Spy:
		return 11;
	case special::PartridgePilferer:
		return 12;
	case special::Witchen:
		return 13;
	case special::WingMan:
		return 14;
	case special::Bomb:
		return 15;
	case special::PoultryPals:
		return 16;
	case special::Angry:
		return 17;
	case special::Lichen:
		return 18;
	case special::Scientist:
		return 19;
	case special::Alchemist:
		return 20;
	}
}

/** Get the special representation of a int (for json)*/
special intToSpecial(int i)
{
	switch (i) {
		case 0:
			return special::BasicFire;
		case 1:
			return special::BasicGrass;
		case 2:
			return special::BasicWater;
		case 3:
			return special::Reaper;
		case 4:
			return special::BirdBrain;
		case 5:
			return special::Ninja;
		case 6:
			return special::Clicken;
		case 7:
			return special::PartyFowl;
		case 8:
			return special::Thicken;
		case 9:
			return special::Mirror;
		case 10:
			return special::Smoked;
		case 11:
			return special::Spy;
		case 12:
			return special::PartridgePilferer;
		case 13:
			return special::Witchen;
		case 14:
			return special::WingMan;
		case 15:
			return special::Bomb;
		case 16:
			return special::PoultryPals;
		case 17:
			return special::Angry;
		case 18:
			return special::Lichen;
		case 19:
			return special::Scientist;
		case 20:
			return special::Alchemist;
        default:
            return special::BasicFire;
	}
}


string sString(special s) {
	switch (s) {
		case special::BasicFire:
		case special::BasicGrass:
		case special::BasicWater:
			return "None";
		case special::Reaper:
			return "Reaper";
		case special::BirdBrain:
			return "Bird Brain";
		case special::Ninja:
			return "Ninja";
		case special::Clicken:
			return "Clicken";
		case special::PartyFowl:
			return "Party Fowl";
		case special::Thicken:
			return "Thicken";
		case special::Mirror:
			return "Mirror";
		case special::Smoked:
			return "Smoked";
		case special::Spy:
			return "Spy";
		case special::PartridgePilferer:
			return "Partridge Pilferer";
		case special::Witchen:
			return "Witchen";
		case special::WingMan:
			return "Wing Man";
		case special::Bomb:
			return "Bomb";
		case special::PoultryPals:
			return "Poultry Pals";
		case special::Angry:
			return "Angry";
		case special::Lichen:
			return "Lich-en";
		case special::Scientist:
			return "Scientist";
		case special::Alchemist:
			return "Alchemist";
	}
}

string sStringLong(special s) {
	switch (s) {
		case special::BasicFire:
		case special::BasicGrass:
		case special::BasicWater:
			return "Basic Chicken";
		case special::Reaper:
			return "Wins against any chicken, remove from the pecking order after combat. Does not deal damage. If negated, become a Basic Water Chicken";
		case special::BirdBrain:
			return "Draw a Chicken"; 
		case special::Ninja:
			return "Swap your opponent�fs bottommost and topmost Chicken";
		case special::Clicken:
			return "Discard your hand and draw that many Chickens";
		case special::PartyFowl:
			return "Negate the effects of the Chicken played against it";
		case special::Thicken:
			return "Move this Chicken to the bottom of the pecking order and move all Chickens you own up one space";
		case special::Mirror:
			return "Copy the ability and type of the Chicken played against this Chicken.  If the opposing Chicken is a Mirror Chicken or Party Fowl, become a Basic Grass Chicken";
		case special::Smoked:
			return "Do not reveal your next played Chicken until clash time";
		case special::Spy:
			return "Draw a card";
		case special::PartridgePilferer: //Is this worth playing?
			return "Draw two cards. Loses to all other chickens.  If negated, become a Basic Grass Chicken";
		case special::Witchen:
			return "Change the type of the Chicken below this one (Fire -> Water -> Grass -> Fire)";
		case special::WingMan: //This seems too good to be true
			return "Play the top chicken in your deck on top of this one";
		case special::Bomb:
			return "Loses to everything.  Deal 2 damage to the enemy Moose.  If negated, become a Basic Fire Chicken";
		case special::PoultryPals:
			return "Reveal cards from your deck until you reveal a Fire Chicken.  Add that Chicken to your Coop and reshuffle the rest.  If no chicken is revealed, end this effect and reshuffle";
		case special::Angry:
			return "Trigger a Clash";
		case special::Lichen:
			return "Your opponent discards a Chicken";
		case special::Scientist:
			return "Swap this Chicken with the Chicken below it";
		case special::Alchemist:
			return "Cycle all your Chickens except this one (Fire -> Water -> Grass -> Fire)";
	}
}

string Chicken::toString() const {
	stringstream ss;
    if(s == special::BasicFire || s == special::BasicWater || s == special::BasicGrass) { //If Chicken special is none
		ss << "Basic " << eString(e) << " Chicken";
	} else {
		ss << sString(s) << " " << eString(e) << " Chicken";
	}
	return ss.str();
}

int Chicken::compare(const Chicken& other) const {
	//CULog("%s clashing with %s", toString().c_str(), other.toString().c_str());
	if (e == element::Unset || other.e == element::Unset) {
		//CULogError("Unset element exception: %s and %s cannot be compared", toString().c_str(), other.toString().c_str());
		return 0;
	}

	//All ties
	if (e == other.e || e == element::TieAll || other.e == element::TieAll) return 0;

	switch (other.e) {
		case element::WinAll:
			return -1;
		case element::LoseAll:
			return 1;
		case element::SpecialWin:
			if (s == special::BasicFire || s == special::BasicGrass || s == special::BasicWater)
				return 1;
			else
				return -1;
		default:
			switch (e) {
			case element::SpecialWin:
				if (other.s == special::BasicFire || other.s == special::BasicGrass || other.s == special::BasicWater)
					return -1;
				else
					return 1;
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
			default:
				//Never reached
				return -2;
			}
	}
}
