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
#include <cugl/audio/CUAudioChannels.h>

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

	std::shared_ptr<Moose> player;
	std::shared_ptr<Moose> opp;

	std::shared_ptr<AI> oppAI;

	//previous hand size for tracking placing a chicken
	int prevHand;

	//stack size
	int stackSize;

	//SceneBuilder
	std::shared_ptr<SceneBuilder1> sb;
    
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
	bool init(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<Moose> playerMoose, std::shared_ptr<Moose> oppMoose, AIType ai, int levelNum);
    
    static std::shared_ptr<GameScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<GameScene> result = std::make_shared<GameScene>();
        return (result->init(assets) ? result : nullptr);
    }

	static std::shared_ptr<GameScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<Moose> playerMoose, std::shared_ptr<Moose> oppMoose, AIType ai, int levelNum) {
		std::shared_ptr<GameScene> result = std::make_shared<GameScene>();
		return (result->init(assets, playerMoose, oppMoose, ai, levelNum) ? result : nullptr);
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

	/**
	 * Initializes Stacks given playOrder vectors for both Moose
	 *
	 * @param playerOrder the playOrder of the player Moose
	 * @param oppOrder the playOrder of the opponent Moose
	 */
	void initStacks(vector<Chicken> playerOrder, vector<Chicken> oppOrder);


#pragma mark -
#pragma mark Accessors
	/** Get the player Moose */
	std::shared_ptr<Moose> getPlayer() { return player; };
	/** Get the opponent Moose */
	std::shared_ptr<Moose> getOpp() { return opp; };
	/** Get the opponent AI */
	std::shared_ptr<AI> getAI() { return oppAI; };

	bool getHome() { return sb->getHome(); }

#pragma mark -
#pragma mark Mutators
	/** 
	 * Set the player Moose to be the given Moose
	 *  
	 * @param newPlayer the Moose to set player as 
	 */
	void setPlayer(std::shared_ptr<Moose> newPlayer) { player = newPlayer; sb->setPlayer(newPlayer); prevHand = player->getHand().size(); stackSize = player->getStack().getSize(); };
	/** 
	 * Set the opponent Moose to be the given Moose
	 *
	 * @param newOpp the Moose to set opp as
	 */
	void setOpp(std::shared_ptr<Moose> newOpp) { opp = newOpp; sb->setOpp(newOpp); };

	/**
	 * Set the AI to the be the given AI type
	 * @param newAI the AI type to set AI as
	 */
	void setAI(std::shared_ptr<Moose> newPlayer, std::shared_ptr<Moose> newOpp, AIType newAI) { oppAI->setPlayer(newPlayer); oppAI->setOpp(newOpp); oppAI->setType(newAI); };

    void setHome(bool val) { sb->setHome(val); };

	void setLevel(int levelNum) { sb->setLevelNum(levelNum); };
    
    void deactivatePause() {sb->deactivatePause(); };
    
    void activatePause() {sb->activatePause(); };

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

	/** returns true if a special chicken effect changes either stack */
	bool specialChanges(Stack player, Stack opp);
};

#endif /* __CCM_GAME_SCENE_H__ */
