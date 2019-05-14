#pragma once
//Scene builder is responsible for constructing the scene graphs for different game modes and handling input.
#ifndef __CCM_SCENE_BUILDER1_H__
#define __CCM_SCENE_BUILDER1_H__

#include <cugl/cugl.h>
#include <unordered_map>
#include <stdlib.h>

#include "CCMInput.h"
#include "Moose.h"
#include "TutorialMoose.h"

using namespace cugl;

/** Sfx for the game*/
#define CHICKEN_SCREECH		"screech"

/** The ID for the button listener */
#define LISTENER_ID 2
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 576
#define SCENE_HEIGHT 1024

//Drawing constants
#define MOOSE_HEIGHT 300
#define FORE_HEIGHT 125
#define MOOSE_X_OFFSET 25
#define HAND_SCALE 0.5f
#define STACK_SCALE 0.5f
#define BUTTON_X_SPACING 175
#define BUTTON_X_OFFSET 0
#define BUTTON_Y_SPACING 90
#define BUTTON_Y_OFFSET -55
#define STACK_X_OFFSET 100
#define STACK_Y_OFFSET 550
#define STACK_Y_SPACING 75
#define INFO_X_OFFSET 7
#define INFO_Y_OFFSET 100
#define INFO_SCALE 0.5
#define HEALTH_BAR_Y_FACTOR 12
#define HEALTH_BLOCK_SPACING 20
#define HEART_X_OFFSET 180
#define BAR_DISTANCE 90
#define CHICKEN_FILMSTRIP_LENGTH 8
#define SIGN_FILMSTRIP_LENGTH 28
#define HEART_SCALE 0.4
#define BLOCK_X_SCALE 0.3
#define BLOCK_Y_SCALE 0.3
#define HBAR_SCALE 0.25
#define ELT_Y_OFFSET 75
#define ELT_INFO_SCALE 0.7
#define ELT_INFO_X_OFFSET 30
#define ELT_NUM_X_OFFSET 40
#define ELT_NUM_Y_OFFSET 5
#define ELT_NUM_SCALE 0.7
#define ELT_NUM_SPACING 50
#define SHADOW_SCALE 0.6
#define SHADOW_OFFSET 50
#define STAMP_X_OFFSET 5
#define STAMP_Y_OFFSET -10
#define STAMP_SCALE 0.1
#define WIN_LOSS_SCALE 0.5
#define WIN_LOSS_Y_OFFSET 100
#define WIN_LOSS_B_SCALE 0.5
#define WIN_LOSS_B_Y_OFFSET -225
#define WIN_BUTTON_X_SPACING 140
#define LOSS_BUTTON_X_SPACING 175
#define CHICKEN_SHOT_ROWS 1
#define CHICKEN_SHOT_COLS 1
#define DEATH_ANIM_COLS 8
#define INFO_DELAY 15
#define MOOSE_SCALE 0.5f

class SceneBuilder1 {
protected:
	//Button list for player hand
	std::vector<std::shared_ptr<Button>> buttons;
	std::vector<std::shared_ptr<AnimationNode>> buttonTextures;

	//Texture list for determining which textures need to be replaced in the hand
	std::vector<std::shared_ptr<Texture>> texturesHand;

	//List of player stack nodes
	std::vector<std::shared_ptr<AnimationNode>> pstackNodes;
	//Texture list for determining which textures need to be replaced in the player stack
	std::vector<std::shared_ptr<Texture>> texturesPStack;

	//List of opponent stack nodes
	std::vector<std::shared_ptr<AnimationNode>> ostackNodes;
	//Texture list for determining which textures need to be replaced in the opponent stack
	std::vector<std::shared_ptr<Texture>> texturesOStack;


	//Track held chicken
	std::shared_ptr<Button> heldButton;


	//Track Stack chicken held down
	int sInfoInd;

	//Track which buttons map to which cards in player hand. card x at index i is the xth card in the opponent's hand, shown by button i 
	std::vector<int> handMap;

	//Track previous chicken distribution
	std::vector<int> prevDist;

	//Track scale for drawing UI elements
	float healthYScale;

	//Button list for pause menu
	std::vector<std::shared_ptr<Button>> pausebuttons;

	//List of stamp nodes for player and opponent
	std::vector<std::shared_ptr<PolygonNode>> pStamps;
	std::vector<std::shared_ptr<PolygonNode>> oStamps;

	//Control variables for menu navigation
	bool isPaused;
	bool nextLevel;
	bool goHome;
	bool retry;
	bool soundToggle;
	bool soundChanged;
	bool hasWon;
	bool hasLost;

	//Chicken Textures
	std::shared_ptr<Texture> textureF;
	std::shared_ptr<Texture> textureW;
	std::shared_ptr<Texture> textureG;
	std::shared_ptr<Texture> textureReaper;
	std::shared_ptr<Texture> textureBomb;
	std::shared_ptr<Texture> textureMirror;
	std::shared_ptr<Texture> textureNinja;
	std::shared_ptr<Texture> textureParty;
	std::shared_ptr<Texture> textureSpy;
	std::shared_ptr<Texture> textureThick;
	std::shared_ptr<Texture> textureWitch;

	//Info Textures
	std::shared_ptr<Texture> infoF;
	std::shared_ptr<Texture> infoW;
	std::shared_ptr<Texture> infoG;
	std::shared_ptr<Texture> infoReaper;
	std::shared_ptr<Texture> infoBomb;
	std::shared_ptr<Texture> infoMirror;
	std::shared_ptr<Texture> infoNinja;
	std::shared_ptr<Texture> infoParty;
	std::shared_ptr<Texture> infoSpy;
	std::shared_ptr<Texture> infoThick;
	std::shared_ptr<Texture> infoWitch;

	//Health textures
	std::shared_ptr<Texture> bar;
	std::shared_ptr<Texture> pHeart;
	std::shared_ptr<Texture> oHeart;
	std::shared_ptr<Texture> pBlock;
	std::shared_ptr<Texture> oBlock;

	//Number textures
	std::shared_ptr<Texture> num0;
	std::shared_ptr<Texture> num1;
	std::shared_ptr<Texture> num2;
	std::shared_ptr<Texture> num3;
	std::shared_ptr<Texture> num4;

	//Shadow Texture
	std::shared_ptr<Texture> shadow;

	//Type stamp textures
	std::shared_ptr<Texture> fstamp;
	std::shared_ptr<Texture> gstamp;
	std::shared_ptr<Texture> wstamp;

	//Win/loss screen textures
	std::shared_ptr<Texture> wlOverlay;
	std::shared_ptr<Texture> victory;
	std::shared_ptr<Texture> homeButton;
	std::shared_ptr<Texture> defeat;
	std::shared_ptr<Texture> redo;
	std::shared_ptr<Texture> nextlvl;

	//transition texture
	std::shared_ptr<Texture> smokeTrans;

	//chicken death textures
	std::shared_ptr<Texture> waterTrans;
	std::shared_ptr<Texture> grassTrans;
	std::shared_ptr<Texture> fireTrans;


	//Main canvas to draw stuff to
	std::shared_ptr<Node> layer;

	//Canvas for background
	std::shared_ptr<Node> backCanvas;

	//Canvas for moose
	std::shared_ptr<Node> mooseCanvas;

	//Canvas for foreground
	std::shared_ptr<Node> frontCanvas;

	//Canvas for info cards
	std::shared_ptr<Node> infoCanvas;

	//Canvas for stack info
	std::shared_ptr<Node> stackInfoCanvas;

	//Canvas for buttons
	std::shared_ptr<Node> buttonCanvas;

	//Canvas for clash button
	std::shared_ptr<Node> clashButtonCanvas;

	//Canvas for health bar
	std::shared_ptr<Node> healthCanvas;

	//Canvas for elemental info
	std::shared_ptr<Node> eltInfoCanvas;

	//Win screen canvas
	std::shared_ptr<Node> winCanvas;

	//Win screen canvas
	std::shared_ptr<Node> loseCanvas;

	//Canvas for pause button
	std::shared_ptr<Node> pauseButtonCanvas;

	//Canvas for misc. UI (i.e. pause)
	std::shared_ptr<Node> pauseMenuCanvas;

	std::shared_ptr<Node> clashSignCanvas;


	//Preview tracking
	bool previewSet;
	//Tint tracking
	bool prevTint;

	//Frame tracking for flapping animations
	int  thisFrame = 0;
	float timeAmount = 0;
	float timeBtnFrames = 0.1;
	//keeps track of which frame chicken flapping is on in the hand
	std::vector<int> flappingFrame;


	//Frame tracking for attacking animations

	//the number of frames it takes for a chicken shot to reach the middle of the screen
	int middleScreenFrame = 8;
	//the frame that a chicken shot is currently at, -1 if no shot on screen
	int shotProgress = -1;
	//0 index is current player death animation frame, 1 is opponent
	std::vector<int> dyingFrame;
	std::vector<int> pSmokeFrame;
	std::vector<int> eSmokeFrame;
	//determines if the player chicken attacking is going to win
	int playerChickenWins;
	//element of player chicken
	element pType;
	//element of enemy chicken
	element eType;


	//Input timer to determine if the player wants info or wants to play a chicken
	std::vector<int> timers;
	int heldButtInd;

	int signframe = 1;
	int signCount = 500;
	bool signDone = false;
	std::shared_ptr<Texture> textsign;
	std::shared_ptr<AnimationNode> sign;

	//Screen dimensions
	float screenHeight;
	float screenWidth;

	/** The asset manager for this scene builder. */
	std::shared_ptr<cugl::AssetManager> _assets;

	CCMInput _input;

	//Players
	std::shared_ptr<Moose> playerGlobe;
	std::shared_ptr<Moose> oppGlobe;

	std::shared_ptr<cugl::Node> selectedChicken;

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
	virtual bool init(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Size size, const std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, string costume, int levelNum);

	//Allocate a scene builder
	static std::shared_ptr<SceneBuilder1> alloc(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Size size, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, string costume, int levelNum) {
		std::shared_ptr<SceneBuilder1> result = std::make_shared<SceneBuilder1>();
		return (result->init(assets, size, root, player, opp, costume, levelNum) ? result : nullptr);
	}

#pragma mark -
#pragma mark Scene Building Methods
	//Build the game scene
	virtual void updateGameScene(float timestep, bool isClashing);

	//Build an individual chicken using a texture and attach to a node.  Clear everything from the node beforehand.
	std::shared_ptr<cugl::AnimationNode> buildChicken(std::shared_ptr<cugl::Texture> texture, std::shared_ptr<cugl::Node> node, int posX, int posY, bool flip);




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
	void setHome(bool val);
//	void setRestart(bool val);

	void setLevelNum(int level);
	void setOppCost(string costume);

	void setPlayer(std::shared_ptr<Moose> newPlayer) { playerGlobe = newPlayer; };
	void setOpp(std::shared_ptr<Moose> newOpp) { oppGlobe = newOpp; setOppCost(newOpp->getCostume()); };
};


#endif /* __CCM_GAME_SCENE_H__ */
