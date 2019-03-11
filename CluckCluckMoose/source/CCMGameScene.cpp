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
/** length of time in milliseconds for clashes */
#define CLASHLENGTH 400
/** maximum size of chicken stack */
#define MAXSTACKSIZE 5

//stack size
int stackSize;

//previous hand size for tracking placing a chicken
int prevHand;

//number of frames in between clashes
int clashCD;

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

	//Initialize clash cooldown
	clashCD = (int) (CLASHLENGTH / MAXSTACKSIZE);


	//Initialize moose
//    player = Moose::Moose(3, 3);
    player = Moose::alloc(5, 5);
//    opp = Moose::Moose(3, 3);
    opp = Moose::alloc(5, 5);
	player->refillHand();
	opp->refillHand();
	prevHand = player->getHand().size();

	oppAI = AI::alloc(opp, player, AIType::Dumb);
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

	sb->updateInput(timestep);
	sb->buildGameScene();


	

	if (prevHand > player->getHand().size()) { // Replace with if chicken is dragged to play area
		prevHand--;
		//player->addToStackFromHand( The index of the chicken played ) if input works
		opp->addToStackFromHand(oppAI->getPlay());
		//Chicken *test = player->getStack().getTop();
		//CULog("OPP %s", opp->getStack().getTop()->toString().c_str());
		//CULog("PLAY %s", test->toString().c_str());
		//CULog("%p",test);
		//test->cycle();
		//player->getStack().changeChickenInStackElement(element::Fire, player->getStack().getSize() - 1);
		//player->getStack().specialChickenEffect(opp->getStack()); // Resolves the special chicken effects
		stackSize++;
		if (stackSize == MAXSTACKSIZE) {
			isClashing = true;
		}
		// Called a second time since opponents last chicken is not shown before a clash for whatever reason
		sb->buildGameScene();
	}

	if (false) { //replace with if Preview button is pressed
		isPreviewing = true;

		playerPreviewStack = player->getStack();
		oppPreviewStack = opp->getStack();
		isClashing = true;
	}

	if (clashCD == 0) {
		if (!player->getStack().empty() && !opp->getStack().empty() && isClashing) {
			//        sleep(CLASHLENGTH);
			int result = player->getStack().getBottom()->compare(*opp->getStack().getBottom());
			if (result == -1)
			{
				CULog("opp win");
				player->removeBottomFromStackToDiscard();
			}
			else if (result == 1)
			{
				CULog("player win");
				opp->removeBottomFromStackToDiscard();
			}
			else
			{
				CULog("tie");
				player->removeBottomFromStackToDiscard();
				opp->removeBottomFromStackToDiscard();
			}
		}
		else if (isClashing && isPreviewing && stackSize != 0) {
			player->setStack(playerPreviewStack);
			opp->setStack(oppPreviewStack);
			isPreviewing = false;
			isClashing = false;
		}
		else if (isClashing && stackSize != 0) {
			//        sleep(CLASHLENGTH);
			player->refillHand();
			opp->refillHand();
			prevHand = player->getHand().size();
			stackSize = 0;

			player->clearStackToDiscard();
			opp->clearStackToDiscard();
			isClashing = false;
		}
	}
	if (clashCD > 0) {
		clashCD--;
	}
	else {
		clashCD = (int) (CLASHLENGTH / MAXSTACKSIZE);
	}
	sb->buildGameScene();
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
