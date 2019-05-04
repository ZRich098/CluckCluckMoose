//
//  UILayoutScene.cpp
//  UI Demo
//
//  This module shows off a (float) layout manager.  Clicking on different
//  buttons will show different alignments.
//
//  Author: Walker White
//  Version: 1/20/18
//
#include "CCMGameScene.h"
#include "SceneBuilder1.h"
#include "Moose.h"
#include "AI.h"

using namespace cugl;


/** The ID for the button listener */
#define LISTENER_ID 2
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 576
#define SCENE_HEIGHT 1024
/** length of time in animation frames for a clash between chickens */
#define CLASHLENGTH 10
/** maximum size of chicken stack */
#define MAXSTACKSIZE 5

/** BGM for the game*/
#define MUSIC_THEME			"theme"
#define MUSIC_TRAILER		"trailer"

/** Sfx for the game*/
#define SOUND_BELL			"bell"
#define SOUND_CHICKEN		"impact"
#define SOUND_WATER			"water"
#define SOUND_WITCHEN		"witchen"
#define SOUND_BUTTON_A		"button_a"
#define SOUND_BUTTON_B		"button_b"

/** Values representing the relevant states for special chicken effect */
#define ENTRY -1
#define NONE 0
#define EXIT 3

/** number of animation frames per timestep */
#define FRAMESPERTIME 10

//stack size
int stackSize;

//previous hand size for tracking placing a chicken
int prevHand;

//number of frames in between clashes
float cooldown;

//a state machine to decide whether or not to skip a player's special chicken call for animation/feedback purposes.
//-1 is the entry state, 0 is no skip, 1 is skip player, 2 is skip opponent, 3 is the exit state
int skipState;

//bool to signify a clash is in progress
bool isClashing;
bool firstClash;

//bool to signify a a winState
bool didWin;

//bool to signify a a loseState
bool didLose;

//bool to signify if sound should play
bool isSound;


////SceneBuilder
//std::shared_ptr<SceneBuilder1> sb;

//Root node for scene builder
std::shared_ptr<Node> root;

//Player moose
std::shared_ptr<Moose> player;
std::shared_ptr<Moose> opp;

std::shared_ptr<AI> oppAI;
//Preview Stacks
//Player Stack
Stack playerPreviewStack;
//Opponent Stack
Stack oppPreviewStack;


//AI
//AI oppAI = AI::alloc(opp, player, AIType::Dumb);



/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = computeActiveSize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }
	_assets = assets;

	auto game_music = _assets->get<Sound>(MUSIC_TRAILER);
	AudioChannels::get()->queueMusic(game_music, true, game_music->getVolume());

	//Root node the drawer can build off of
	root = Node::alloc();
	addChild(root);
  
	//Initialize audio channels
  
	//Initialize stack sizes
	stackSize = 0;

	//Initialize skip state
	skipState = ENTRY;

	//Initialize cooldown
	//cooldown = (int)(CLASHLENGTH / MAXSTACKSIZE);

	//Initialize moose
    player = Moose::alloc(5, 6);
    opp = Moose::alloc(5, 6);
	player->refillHand();
	opp->refillHand();
	prevHand = player->getHand().size();

	oppAI = AI::alloc(opp, player, AIType::Smart);
	sb = SceneBuilder1::alloc(assets, dimen, root, player, opp, "christmoose", 3);

	sb->deactivateHand();

	//Initialize AI
	//oppAI = AI::AI(opp,player);

	//Draw
    
    setActive(_active);
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;

}

bool GameScene::init(const std::shared_ptr<AssetManager>& assets, const std::shared_ptr<Moose> playerMoose, const std::shared_ptr<Moose> oppMoose, const AIType ai) {
	// Initialize the scene to a locked width
	Size dimen = computeActiveSize();
	dimen *= SCENE_WIDTH / dimen.width; // Lock the game to a reasonable resolution
	if (assets == nullptr) {
		return false;
	}
	else if (!Scene::init(dimen)) {
		return false;
	}
	_assets = assets;

	auto game_music = _assets->get<Sound>(MUSIC_TRAILER);
	AudioChannels::get()->queueMusic(game_music, true, game_music->getVolume());

	//Root node the drawer can build off of
	root = Node::alloc();
	addChild(root);

	//Initialize audio channels

	//Initialize stack sizes
	stackSize = 0;

	//Initialize skip state
	skipState = ENTRY;

	//Initialize cooldown
	//cooldown = (int)(CLASHLENGTH / MAXSTACKSIZE);

	//Initialize moose
	player = playerMoose;
	opp = oppMoose;
	//player->refillHand();
	//opp->refillHand();
	prevHand = player->getHand().size();

	oppAI = AI::alloc(opp, player, ai);
	sb = SceneBuilder1::alloc(assets, dimen, root, player, opp, "christmoose", 3);
	sb->setPreview(false);
	sb->deactivateHand();

	//Initialize AI
	//oppAI = AI::AI(opp,player);

	//Draw

	setActive(_active);

	// XNA nostalgia
	Application::get()->setClearColor(Color4f::CORNFLOWER);
	return true;

}


/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    _assets = nullptr;
    Scene::dispose();

	//Stop playing audio
	AudioChannels::get()->stopMusic();
}

void GameScene::initStacks(vector<Chicken> playerOrder, vector<Chicken> oppOrder) {
	vector<Chicken>::iterator pl = playerOrder.begin();
	vector<Chicken>::iterator op = oppOrder.begin();
	while (pl != playerOrder.end() && op != oppOrder.end()) {
		//play both vector::front() Chickens and resolve them
		player->getStack().add(*pl);
		opp->getStack().add(*op);

		//Resolve effects
		int initState = 0;
		while (initState != EXIT) {
			// Resolves the special chicken effects
			tie(initState, ignore) = player->getStack().specialChickenEffect(opp->getStack(), initState);
			if (player->getStack().getWitchenPlayed()) {
				auto source = _assets->get<Sound>(SOUND_WITCHEN);
				if (!AudioChannels::get()->isActiveEffect(SOUND_WITCHEN)) {
					AudioChannels::get()->playEffect(SOUND_WITCHEN, source, false, source->getVolume());
				}
			}
		}

		++pl;
		++op;
		stackSize++;
	}
}

/**
 * The method called to update the game mode.
 *
 * This method contains any gameplay code that is not an OpenGL call.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void GameScene::update(float timestep) {
    isSound = !sb->getSoundToggle(); // false if sound SHOULD PLAY
    if (!isSound && AudioChannels::get()->currentMusic() != NULL){AudioChannels::get()->stopMusic();} // need to check it's not already stopped?
    else if (isSound && AudioChannels::get()->currentMusic() == NULL){
        auto game_music = _assets->get<Sound>(MUSIC_TRAILER);
        AudioChannels::get()->queueMusic(game_music, true, game_music->getVolume());
    }
    
	if (cooldown > 0) {
		cooldown-= timestep*FRAMESPERTIME;
		sb->updateGameScene(timestep);
		return;
	}

	if (player->getOrder().size() > player->getStack().getSize() && opp->getOrder().size() > opp->getStack().getSize() && !isClashing) {
		//CULog("playerOrder size: %d, oppOrder size: %d, calling initStacks", player->getPlayOrder().size(), opp->getPlayOrder().size());
		initStacks(player->getOrder(), opp->getOrder());
	}

	sb->updateInput(timestep);

	if (prevHand > player->getHand().size() && !isClashing) { // Replace with if chicken is dragged to play area
		if (skipState == ENTRY) {
			opp->addToStackFromHand(oppAI->getPlay());

			//CULog("OPP %s", opp->getStack().getTop()->toString().c_str());
			//CULog("PLAY %s", test.toString().c_str());
			skipState = NONE; // Gets the state machine out of the entry state
		}
		if (skipState != EXIT)
			// Resolves the special chicken effects
			tie(skipState, cooldown) = player->getStack().specialChickenEffect(opp->getStack(), skipState);
			if (player->getStack().getWitchenPlayed()) {
				auto source = _assets->get<Sound>(SOUND_WITCHEN);
				if (!AudioChannels::get()->isActiveEffect(SOUND_WITCHEN) && isSound) {
					AudioChannels::get()->playEffect(SOUND_WITCHEN, source, false, source->getVolume());
				}
			}
		if (skipState == EXIT) {
			// Resolves special chickens that affect the hands
			handEffect();
			prevHand--;
			stackSize++;
			skipState = ENTRY; // Returns the state machine to the entry state
		}

		setNumChickensWillDiePreview();
		//CULog("SKIP: %d",skipState);
	}

	if (sb->getPreview() && !isClashing) { //replace with if Preview button is pressed
		//Play the button sfx
		string sfx = rand() % 2 ? SOUND_BUTTON_A : SOUND_BUTTON_B;
		auto source = _assets->get<Sound>(sfx);
		if (!AudioChannels::get()->isActiveEffect(SOUND_BUTTON_A) && !AudioChannels::get()->isActiveEffect(SOUND_BUTTON_B) && isSound) {
			AudioChannels::get()->playEffect(sfx, source, false, source->getVolume());
		}

		cooldown = CLASHLENGTH;
	}

	if (isClashing) {
		if (!player->getStack().empty() && !opp->getStack().empty()) {
			if (!firstClash) {
				int result = player->getStack().compare(opp->getStack());
				if (result == 1) {
					opp->setNumChickensWillDiePreview(opp->getNumChickensWillDiePreview() - 1);
				}
				else if (result == 0) {
					player->setNumChickensWillDiePreview(player->getNumChickensWillDiePreview() - 1);
					opp->setNumChickensWillDiePreview(opp->getNumChickensWillDiePreview() - 1);
				}
				else if (result == -1) {
					player->setNumChickensWillDiePreview(player->getNumChickensWillDiePreview() - 1);
				}
			}
			if (!player->getStack().empty() && !opp->getStack().empty()) {
				element pEle = player->getStack().getBottom().getElement();
				element oEle = opp->getStack().getBottom().getElement();
				sb->chickDefeat(pEle, oEle, player->getStack().compareWithoutRemove(opp->getStack()));
			}
      
			firstClash = false;
			cooldown = CLASHLENGTH;
		}
		else {
			player->eraseChickens();
			opp->eraseChickens();
			player->refillHand();
			opp->refillHand();

			prevHand = player->getHand().size();
			stackSize = 0;

			player->setNumChickensWillDiePreview(0);
			opp->setNumChickensWillDiePreview(0);
			player->resetVecChickensClashPreview();
			opp->resetVecChickensClashPreview();

			player->takeDamage(opp->getStack().getDamage());
			opp->takeDamage(player->getStack().getDamage());
			sb->mooseDefeat(player->getStack().getDamage() - opp->getStack().getDamage());

			player->getStack().clear();
			opp->getStack().clear();
			isClashing = false;

			if (player->getHealth() <= 0) {
				didLose = true;
				didWin = false;
				CULog("lost! lmao");
			}

			if (opp->getHealth() <= 0) {
				didLose = false;
				didWin = true;
				CULog("win");
			}
		}
	} else if (stackSize == MAXSTACKSIZE) { // Called before a clash to let the finished stacks be drawn
		isClashing = true;
		firstClash = true;
		cooldown = CLASHLENGTH*1.5;

		//Play the clashing sfx
		auto source = _assets->get<Sound>(SOUND_BELL);
		if (!AudioChannels::get()->isActiveEffect(SOUND_BELL) && isSound) {
			AudioChannels::get()->playEffect(SOUND_BELL, source, false, source->getVolume());
		}
	}
	
	sb->updateGameScene(timestep);
}


/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void GameScene::setActive(bool value) {
    _active = value;
    int pos = LISTENER_ID;
    cooldown = CLASHLENGTH/2;
    if (cooldown > 0) {
        cooldown--;
        return;
    }
	sb->activateHand();
    /* For(auto it = _buttons.begin(); it != _buttons.end(); ++it) {
        if (value && !it->second->isActive()) {
            it->second->activate(pos++);
        } else {
            it->second->deactivate();
        }
    } */
}

void GameScene::setNumChickensWillDiePreview() {
	Stack p = Stack(player->getStack());
	Stack o = Stack(opp->getStack());

	int ppos = 0;
	int opos = 0;

	int size = p.getSize();

	while (!p.empty() && !o.empty()) {
		int result = p.compare(o);

		switch (result) {
		case -1: //opp win
			opp->setVecChickensClashPreview(opos, ppos);
			ppos ++;
			break;
		case 0: //tie
			player->setVecChickensClashPreview(ppos, opos);
			opp->setVecChickensClashPreview(opos, ppos);
			ppos++;
			opos++;
			break;
		case 1: //win
			player->setVecChickensClashPreview(ppos, opos);
			opos++;
			break;
		default:
			break;
		}
	}

	player->setNumChickensWillDiePreview(size - p.getSize());
	opp->setNumChickensWillDiePreview(size - o.getSize());
}

void GameScene::handEffect() {
	special pLast = player->getOrder().back().getSpecial();
	special oLast = opp->getOrder().back().getSpecial();

	if (pLast == special::PartyFowl || oLast == special::PartyFowl)
		return;

	if (pLast == special::Mirror)
		pLast = oLast;
	if (oLast == special::Mirror)
		oLast = pLast;

	if (pLast == special::Spy) {
		player->draw();
		//please for the love of god fix this later
		prevHand++;
	}
	if (oLast == special::Spy)
		opp->draw();
}

Size GameScene::computeActiveSize() const {
	Size dimen = Application::get()->getDisplaySize();
	//float ratio1 = dimen.width / dimen.height;
	//float ratio2 = ((float)SCENE_WIDTH) / ((float)SCENE_HEIGHT);
	
	//dimen *= SCENE_WIDTH / dimen.width;

	dimen *= SCENE_HEIGHT / dimen.height;
	return dimen;
}
