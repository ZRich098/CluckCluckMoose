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
    player = Moose::alloc(5, 6);
//    opp = Moose::Moose(3, 3);
    opp = Moose::alloc(5, 6);
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
			// Resolves special chickens that affect the hands
			handEffect();
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

			player->clearHandToDiscard();
			opp->clearHandToDiscard();
			// refills before discarding to prevent specials being obtained twice in a row
			player->refillHand();
			opp->refillHand();
			player->discardStack();
			opp->discardStack();

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
