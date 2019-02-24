#ifndef __DECKS_H__
#define __DECKS_H__
#include "Chicken.h"

ChickenType deckV1[30] = {
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,

	ChickenType::Ninja,
	ChickenType::Ninja,
	ChickenType::Thicken,
	ChickenType::Thicken,
	ChickenType::Scientist,
	ChickenType::Scientist,
	ChickenType::Reaper,
	ChickenType::Reaper,
	ChickenType::Mirror,
	ChickenType::Mirror,
	ChickenType::PartyFowl,
	ChickenType::PartyFowl,
	ChickenType::Spy,
	ChickenType::Spy,
	ChickenType::Angry,
	ChickenType::Angry,
	ChickenType::Lichen,
	ChickenType::Lichen
};

ChickenType deckV2[28] = { //actually 30 but 1 deprecated chicken type
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,

	ChickenType::Consigliere,
	ChickenType::Consigliere,
	//2 fusion chicken
	ChickenType::Ninja,
	ChickenType::Ninja,
	ChickenType::Bomb,
	ChickenType::Bomb,
	ChickenType::Alchemist,
	ChickenType::Alchemist,
	ChickenType::Reaper,
	ChickenType::Reaper,
	ChickenType::BirdBrain,
	ChickenType::BirdBrain,
	ChickenType::PartyFowl,
	ChickenType::PartyFowl,
	ChickenType::Mirror,
	ChickenType::Mirror,
	ChickenType::Spy,
	ChickenType::Spy,
	ChickenType::Scientist,
	ChickenType::Scientist,
	ChickenType::Thicken,
	ChickenType::Thicken
};

ChickenType deckV3[18] = {
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,

	ChickenType::Thicken,
	ChickenType::Ninja,
	ChickenType::PartyFowl,
	ChickenType::Mirror,
	ChickenType::Reaper,
	ChickenType::BirdBrain,
	ChickenType::Bomb,
	ChickenType::Consigliere,
	ChickenType::Spy
};

ChickenType deckV4[30] = {
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicFire,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicWater,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,
	ChickenType::BasicGrass,

	ChickenType::Consigliere,
	ChickenType::WingMan,
	ChickenType::Ninja,
	ChickenType::Bomb,
	ChickenType::PoultryPals,
	ChickenType::Reaper,
	ChickenType::BirdBrain,
	ChickenType::Clicken,
	ChickenType::PartyFowl,
	ChickenType::Mirror,
	ChickenType::Spy,
	ChickenType::Smoked,
	ChickenType::Thicken,
	ChickenType::PartridgePilferer,
	ChickenType::Lichen
};

#endif /* __DECKS_H__ */