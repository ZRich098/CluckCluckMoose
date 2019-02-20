//
//  PDGameMode.h
//  Poly Demo
//
//  This is the most important class in this demo.  This class manages the
//  gameplay for this demo.  It also handles collision detection. There is not
//  much to do for collisions; our ObstacleWorld class takes care of all
//  of that for us.  This controller mainly transforms input into gameplay.
//
//  WARNING: There are a lot of shortcuts in this design that will do not adapt
//  well to data driven design.  This demo has a lot of simplifications to make
//  it a bit easier to see how everything fits together.  However, the model
//  classes and how they are initialized will need to be changed if you add
//  dynamic level loading.
//
//  Author: Walker White and Anthony Perello
//  Version: 1/29/17
//
#include "PDGameScene.h"
#include <cugl/2d/CUPathNode.h>

#include <ctime>
#include <string>
#include <iostream>
#include <sstream>

using namespace cugl;

/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024


#pragma mark -
#pragma mark Constructors
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
    
    // Start up the input handler
    _assets = assets;
    _input.init();

	// Set the bounds for panning
	_bounds.size   = dimen;
	_bounds.origin.x = 0;
	_bounds.origin.y = 0;

    // Acquire the scene built by the asset loader and resize it the scene
    _worldnode = _assets->get<Node>("world");
    _worldnode->setPosition(dimen.width/2.0f,dimen.height/2.0f);
    addChild(_worldnode,0);

	//displayPolygons();
	_active = true;

	// XNA nostalgia
	Application::get()->setClearColor(Color4f::CORNFLOWER);
	return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
	if (_active) {
        removeAllChildren();
		_input.dispose();
		_worldnode = nullptr;
		_active = false;
	}
}


#pragma mark -
#pragma mark Level Layout

/**
 * Resets the status of the game so that we can play again.
 *
 * This method disposes of the world and creates a new one.
 */
void GameScene::reset() {
    _input.clear();
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    
	_worldnode->setPosition(dimen.width/2.0f,dimen.height/2.0f);
}


#pragma mark -
#pragma mark Gameplay Handling
/**
 * Executes the core gameplay loop of this world.
 *
 * This method contains the specific update code for this mini-game.
 * This method is called after input is read, but before collisions are resolved.
 * Since a physics world does not exist in this demo, this method simply
 * handles player input.
 *
 * @param  delta    Number of seconds since last animation frame
 */
void GameScene::update(float dt) {
	_input.update(dt);

	// Process the toggled key commands
    if (_input.didReset()) {
        reset();
    } else if (_input.didExit())  {
		CULog("Shutting down");
		Application::get()->quit();
    } else {
        panScreen(_input.getPanDelta());
    }
}

/**
 * This class is the primary gameplay constroller for the demo.
 *
 * A world has its own objects, assets, and input controller.  Thus this is
 * really a mini-GameEngine in its own right.  As in 3152, we separate it out
 * so that we can have a separate mode for the loading screen.
 */
void GameScene::panScreen(const cugl::Vec2& delta) {
	if (delta.lengthSquared() == 0) {
		return;
	}

	float x = _worldnode->getPosition().x+delta.x;
	float y = _worldnode->getPosition().y+delta.y;
	if (!_bounds.contains(Vec2(x,y))) {
		x = std::max(x,_bounds.origin.x);
		x = std::min(x,_bounds.origin.x+_bounds.size.width);
		y = std::max(y,_bounds.origin.y);
		y = std::min(y,_bounds.origin.y+_bounds.size.height);
	}
	_worldnode->setPosition(x,y);
}
