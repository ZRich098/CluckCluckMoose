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
#ifndef __CCM_GAME_SCENE_H__
#define __CCM_GAME_SCENE_H__

#include <cugl/cugl.h>
#include <unordered_map>

#include "CCMInput.h"
#include "Moose.h"
#include "AI.h"
//#include "unistd.h"

/**
 * A scene for demoing a layout manager
 */
class GameScene : public cugl::Scene {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    CCMInput _input;
    
    std::unordered_map<std::string,std::shared_ptr<cugl::Button>> _buttons;
    
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new scene with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    GameScene() {}
    
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
    virtual void dispose() override;

	/**
	* Draw a chicken to screen
	*/
	void makeChicken(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::Texture> texture, std::shared_ptr<cugl::Node> node, int posX, int posY, bool flip);

	/**
	* Draw a chicken to screen
	*/
	void draw(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::Node> node);

    
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
    
    static std::shared_ptr<GameScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<GameScene> result = std::make_shared<GameScene>();
        return (result->init(assets) ? result : nullptr);
    }
    
    /**
     * Sets whether the scene is currently active
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

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
	 * Resets the status of the game so that we can play again.
	 */
	//void reset();
};

#endif /* __CCM_GAME_SCENE_H__ */
