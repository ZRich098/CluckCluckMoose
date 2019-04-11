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
#include "SceneBuilder1.h"
//#include "unistd.h"

/**
 * A scene for demoing a layout manager
 */
class GameScene : public cugl::Scene {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    
    //std::unordered_map<std::string,std::shared_ptr<cugl::Button>> _buttons;
    
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
	 * Returns the active screen size of this scene.
	 *
	 * This method is for graceful handling of different aspect
	 * ratios
	 */
	cugl::Size computeActiveSize() const;
    
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

#pragma mark -
#pragma mark Special Handling

	//Special
	/** Does some effect on the player's or opponent's hand based on the last played chicken*/
	void handEffect();
	/** Does special chicken effects of the chickens on the top of both stacks. */
	//void specialChickenEffect(Stack &player, Stack &opp);
	/** Sets the number of chickens in both moose's stack that will die if clashes were to occur with the current stacks*/
	void setNumChickensWillDiePreview();
};

#endif /* __CCM_GAME_SCENE_H__ */
