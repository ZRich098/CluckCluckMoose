//
//  PDGameScene.h
//  Poly Demo
//
//  This is the most important class in this demo.  This class manages the
//  gameplay for this demo. This controller mainly transforms input into gameplay.
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
#ifndef __PD_GAME_SCENE_H__
#define __PD_GAME_SCENE_H__
#include <cugl/cugl.h>
#include <vector>
#include "PDInput.h"


/**
 * This class is the primary gameplay constroller for the demo.
 *
 * A world has its own objects, assets, and input controller.  Thus this is
 * really a mini-GameEngine in its own right.  As in 3152, we separate it out
 * so that we can have a separate mode for the loading screen.
 */
class GameScene : public cugl::Scene {
protected:
	/** The asset manager for this game mode. */
	std::shared_ptr<cugl::AssetManager> _assets;

	// CONTROLLERS
	/** Controller for abstracting out input across multiple platforms */
	PolyInput _input;

	// VIEW
	/** Reference to the root of the scene graph */
	std::shared_ptr<cugl::Node> _worldnode;

	/** The scroll bounds on the demo */
	cugl::Rect _bounds;


#pragma mark Input Handling
    /**
     * Pans the screen by the amount given by the input
     *
     * @param delta The pan offset
     */
	void panScreen(const cugl::Vec2& delta);

public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new game world with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
	GameScene() : Scene() {}

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
	~GameScene() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
	void dispose();

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
	bool init(const std::shared_ptr<cugl::AssetManager>& assets);

#pragma mark -
#pragma mark Gameplay Handling
    /**
     * The method called to update the game mode.
     *
     * This method contains any gameplay code that is not an OpenGL call.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
	void update(float timestep);
    
    /**
     * Resets the status of the scene to the beginning.
     */
    void reset();
};

#endif /* __PD_GAME_SCENE_H__ */
