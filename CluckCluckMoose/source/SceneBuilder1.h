#pragma once
#pragma once
//Scene builder is responsible for constructing the scene graphs for different game modes and handling input.
#ifndef __CCM_SCENE_BUILDER1_H__
#define __CCM_SCENE_BUILDER1_H__

#include <cugl/cugl.h>
#include <unordered_map>
#include <stdlib.h>

#include "CCMInput.h"
#include "Moose.h"

// Sfx for the game
#define SOUND_BUTTON_A        "button_a"
#define SOUND_BUTTON_B        "button_b"

class SceneBuilder1 {
protected:
	/** The asset manager for this scene builder. */
	std::shared_ptr<cugl::AssetManager> _assets;

	CCMInput _input;

	//Players
	std::shared_ptr<Moose> playerGlobe;
	std::shared_ptr<Moose> oppGlobe;

	std::shared_ptr<cugl::Node> selectedChicken;

	bool previewSet;

public:
#pragma mark -
#pragma mark Constructors

	/**
	 * Creates a new scene with the default values.
	 *
	 * This constructor does not allocate any objects or start the controller.
	 * This allows us to use a controller without a heap pointer.
	 */
	SceneBuilder1() {}

	/**
	 * Disposes of all (non-static) resources allocated to this mode.
	 *
	 * This method is different from dispose() in that it ALSO shuts off any
	 * static resources, like the input controller.
	 */
	~SceneBuilder1() { dispose(); }

	/**
	 * Disposes of all (non-static) resources allocated to this mode.
	 */
	void dispose();

	/**
	 * Initializes the scene builder
	 *
	 * The constructor does not allocate any objects or memory.  This allows
	 * us to have a non-pointer reference to this controller, reducing our
	 * memory allocation.  Instead, allocation happens in this method.
	 *
	 * @param assets    The (loaded) assets for this game mode
	 *
	 * @return true if the controller is initialized properly, false otherwise.
	 */
	bool init(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Size size, const std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, string costume, int levelNum);

	//Allocate a scene builder
	static std::shared_ptr<SceneBuilder1> alloc(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Size size, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, string costume, int levelNum) {
		std::shared_ptr<SceneBuilder1> result = std::make_shared<SceneBuilder1>();
		return (result->init(assets, size, root, player, opp, costume, levelNum) ? result : nullptr);
	}

#pragma mark -
#pragma mark Scene Building Methods
	//Build the game scene
	void updateGameScene(float timestep, bool isClashing);

	//Build an individual chicken using a texture and attach to a node.  Clear everything from the node beforehand.
	std::shared_ptr<cugl::AnimationNode> buildChicken(std::shared_ptr<cugl::Texture> texture, std::shared_ptr<cugl::Node> node, int posX, int posY, bool flip);

    void playButtonSound(int sound);


#pragma mark -
#pragma mark Input Methods
	//Update input
	void updateInput(float timestep);
	//winResult -1 means player loss, 0 means tie, 1 means player win
	void chickDefeat(element playerType, element opponentType, int winResult);
	// positive means opponent loses health, negative means player loses health
	void mooseDefeat(int healthChange);


#pragma mark -
#pragma mark Accessors
	//gets if preview button has been pressed
	bool getPreview() { return previewSet; }
//	bool getHome() { return goHome; }
//	bool getRestart() { return pauseRestartDown; }
//	bool getSettings() { return pauseSettingsDown; }
	
	//returns true if the layer has pressed the go home button
	bool getHome();
	//returns true if the player has pressed the retry button
	bool getRedo();
	//returns true if the player has pressed the next level button
	bool getNextLevel();
    //returns true if sound should be off
    bool getSoundToggle();

	//returns true if the pause menu is turned on
	bool getPaused();

#pragma mark -
#pragma mark Setters
	//sets previewSet to the given value
	void setPreview(bool preview);
	void deactivateHand();
	void activateHand();
	void deactivatePause();
	void activatePause();
    void deactivateWin();
    void activateWin();
    void deactivateLose();
    void activateLose();
	void setHome(bool val);
//	void setRestart(bool val);

	void setLevelNum(int level);
	void setOppCost(string costume);

	void setPlayer(std::shared_ptr<Moose> newPlayer) { playerGlobe = newPlayer; };
	void setOpp(std::shared_ptr<Moose> newOpp) { oppGlobe = newOpp; setOppCost(newOpp->getCostume()); };
};


#endif /* __CCM_GAME_SCENE_H__ */
