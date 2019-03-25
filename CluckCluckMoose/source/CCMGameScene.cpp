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
#define SCENE_WIDTH 1024
/** length of time in frames for a clash between chickens */
#define CLASHLENGTH 50
/** maximum size of chicken stack */
#define MAXSTACKSIZE 5

/** Values representing the relevant states for special chicken effect */
#define ENTRY -1
#define NONE 0
#define EXIT 3

//stack size
int stackSize;

//previous hand size for tracking placing a chicken
int prevHand;

//number of frames in between clashes
int cooldown;

//a state machine to decide whether or not to skip a player's special chicken call for animation/feedback purposes.
//-1 is the entry state, 0 is no skip, 1 is skip player, 2 is skip opponent, 3 is the exit state
int skipState;

//bool to signify a clash is in progress
bool isClashing;


//bool to signify a clash preview is in progress
bool isPreviewing;



//SceneBuilder
std::shared_ptr<SceneBuilder1> sb;

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
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }


	//Root node the drawer can build off of
	root = Node::alloc();
	addChild(root);
  
  
	//Initialize stack sizes
	stackSize = 0;

	//Initialize skip state
	skipState = ENTRY;

	//Initialize cooldown
	//cooldown = (int)(CLASHLENGTH / MAXSTACKSIZE);


	//Initialize moose
//    player = Moose::Moose(3, 3);
    player = Moose::alloc(5, 5);
//    opp = Moose::Moose(3, 3);
    opp = Moose::alloc(5, 5);
	player->refillHand();
	opp->refillHand();
	prevHand = player->getHand().size();

	oppAI = AI::alloc(opp, player, AIType::Smart);
	sb = SceneBuilder1::alloc(assets, dimen, root, player, opp);

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
}

/**
 * The method called to update the game mode.
 *
 * This method contains any gameplay code that is not an OpenGL call.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void GameScene::update(float timestep) {
	if (cooldown > 0) {
		cooldown--;
		return;
	}

	sb->updateInput(timestep);

	if (prevHand > player->getHand().size() && !isClashing) { // Replace with if chicken is dragged to play area
		if (skipState == ENTRY) {
			//player->addToStackFromHand( The index of the chicken played ) if input works
			opp->addToStackFromHand(oppAI->getPlay());

			//CULog("OPP %s", opp->getStack().getTop()->toString().c_str());
			//CULog("PLAY %s", test.toString().c_str());
			skipState = NONE; // Gets the state machine out of the entry state
		}
		if (skipState != EXIT)
			// Resolves the special chicken effects
			tie(skipState, cooldown) = player->getStack().specialChickenEffect(opp->getStack(), skipState);
		if (skipState == EXIT) {
			prevHand--;
			stackSize++;
			skipState = ENTRY; // Returns the state machine to the entry state
		}
		//CULog("SKIP: %d",skipState);
	}

	if (sb->getPreview() && !isPreviewing) { //replace with if Preview button is pressed
		isPreviewing = true;

		playerPreviewStack = player->getStack();
		oppPreviewStack = opp->getStack();
		isClashing = true;
		cooldown = CLASHLENGTH;
	}

	if (isClashing) {
		if (!player->getStack().empty() && !opp->getStack().empty()) {
			player->getStack().compare(opp->getStack());
			cooldown = CLASHLENGTH;
		}
		else if (isPreviewing) {
			//player->setStack(playerPreviewStack);
			//opp->setStack(oppPreviewStack);
			player->getStack().clear();
			opp->getStack().clear();
			for (int i = 0; i < playerPreviewStack.getSize(); i++) {
				player->getStack().add(playerPreviewStack.at(i));
			}
			for (int i = 0; i < oppPreviewStack.getSize(); i++) {
				opp->getStack().add(oppPreviewStack.at(i));
			}
			isPreviewing = false;
			isClashing = false;
			cooldown = CLASHLENGTH;

			sb->setPreview(false);
		}
		else {
			player->discardCards();
			opp->discardCards();
			player->refillHand();
			opp->refillHand();
			prevHand = player->getHand().size();
			stackSize = 0;

			//player->takeDamage(opp->getStack().getSize());
			//opp->takeDamage(player->getStack().getSize());

			player->getStack().clear();
			opp->getStack().clear();
			isClashing = false;
		}
	} else if (stackSize == MAXSTACKSIZE) { // Called before a clash to let the finished stacks be drawn
		isClashing = true;
		cooldown = CLASHLENGTH*1.5;
	}
	
	sb->updateGameScene();
}


/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void GameScene::setActive(bool value) {
    _active = value;
    int pos = LISTENER_ID;
    /* For(auto it = _buttons.begin(); it != _buttons.end(); ++it) {
        if (value && !it->second->isActive()) {
            it->second->activate(pos++);
        } else {
            it->second->deactivate();
        }
    } */
}

/* void GameScene::specialChickenEffect(Stack &player, Stack &opp) {
	//lambda function, make cooldown an argument if/when it becomes necessary
	auto setSkip = [](int v) { cooldown = 0; return (skipState == 0 ? skipState = v : skipState = 3); };

	// Note that this method of skipping WILL cause issues in the event
	// that there is a special chicken that occurs after a ninja chicken
	special s1 = special::BasicFire;
	special s2 = special::BasicFire;
	if (skipState == 0) {
		s1 = player.getTop().getSpecial();
		s2 = opp.getTop().getSpecial();
	}
	else if (skipState == 1) {
		s2 = opp.getTop().getSpecial();
	}
	else if (skipState == 2) {
		s1 = player.getTop().getSpecial();
	}
	else // This should never be reached, but will catch it if necessary
		return;

	CULog("\n%s", player.stackString().c_str());
	// Reaper, Bomb, and Basics are all represented by element and damage and do not need special effects

	if (s1 == special::PartyFowl || s2 == special::PartyFowl) {
		//CULog("resolving Party");
		Chicken target = player.getTop();
		//s1 == special::PartyFowl ? target = opp.getTop() : target = getTop();
		if (s1 == special::PartyFowl && s2 != special::PartyFowl) {
			//CULog("Setting opp as target");
			target = opp.getTop();
		}
		else if (s1 != special::PartyFowl && s2 == special::PartyFowl) {
			//CULog("Setting player as target");
			target = player.getTop();
		}
		switch (target.getSpecial()) {
		case special::Reaper:
			target.setChicken(element::Water, special::BasicWater);
			break;
		case special::Mirror:
			target.setChicken(element::Fire, special::BasicFire);
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
		case special::Consigliere:
			target.setChicken(element::Water, special::BasicWater);
			break;
		default:
			//CULog("reached default");
			break;
		}

		setSkip(3);
		return;
	}

	if (s1 == special::Mirror && s2 == special::Mirror) {
		player.getTop().setChicken(element::Fire, special::BasicFire);
		opp.getTop().setChicken(element::Fire, special::BasicFire);
		setSkip(3);
		return;
	}
	else if (s1 == special::Mirror) {
		//CULog("player mirror");
		s1 = s2;
		player.getTop().setChicken(opp.getTop().getElement(), opp.getTop().getSpecial(), opp.getTop().getDamage());
		setSkip(0);
		return;
	}
	else if (s2 == special::Mirror) {
		//CULog("opp mirror");
		s2 = s1;
		opp.getTop().setChicken(player.getTop().getElement(), player.getTop().getSpecial(), player.getTop().getDamage());
		setSkip(0);
		return;
	}

	//potentially TODO special::Peek

	if (s1 == special::Consigliere && player.getSize() >= 2) {
		player.at(player.getSize() - 2).cycle();
		setSkip(1);
		return;
	}
	if (s2 == special::Consigliere && opp.getSize() >= 2) {
		opp.at(opp.getSize() - 2).cycle();
		setSkip(2);
		return;
	}

	if (s1 == special::Scientist && player.getSize() >= 2) {
		player.swap(player.getSize() - 2, player.getSize() - 1);
		setSkip(1);
		return;
	}
	if (s2 == special::Scientist && opp.getSize() >= 2) {
		opp.swap(opp.getSize() - 2, opp.getSize() - 1);
		setSkip(2);
		return;
	}

	if (s1 == special::Thicken) {
		player.insert(0, player.getTop());
		player.removeTop();
		setSkip(1);
		return;
	}
	if (s2 == special::Thicken) {
		opp.insert(0, opp.getTop());
		opp.removeTop();
		setSkip(2);
		return;
	}

	//TODO special::Hide

	//potentially TODO special::Extra

	if (s1 == special::Spy) {
		//Can be made from here
	}
	if (s2 == special::Spy) {
		//see above
	}

	if (s1 == special::Ninja && s2 == special::Ninja) {
		player.swap(0, player.getSize() - 1);
		opp.swap(0, opp.getSize() - 1);
		setSkip(3);
		return;
	}
	else if (s1 == special::Ninja) {
		opp.swap(0, opp.getSize() - 1);
		setSkip(3);
		return;
	}
	else if (s2 == special::Ninja) {
		player.swap(0, player.getSize() - 1);
		setSkip(3);
		return;
	}

	// Exits state machine if no special chicken is found
	skipState = 3;

	CULog("\n%s", player.stackString().c_str());
} */
