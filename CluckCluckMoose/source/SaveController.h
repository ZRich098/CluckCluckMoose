#ifndef __SAVE_CONTROLLER_H__
#define __SAVE_CONTROLLER_H__
#include <cugl/cugl.h>
#include <vector>
#include <cugl/assets/CUAsset.h>
#include <cugl/io/CUJsonReader.h>

#include "MooseFile.h"
#include "Moose.h"
#include "AI.h"

using namespace cugl;

class SaveController : public Asset {
private:
	/** The root node of this level */
	std::shared_ptr<Moose> player;	 /** BE SURE TO CALL loadPlayerMoose BEFORE ACCESSING*/
	std::shared_ptr<Moose> opponent; /** BE SURE TO CALL loadOpponentMoose BEFORE ACCESSING*/

#define LEVEL_FIELD      "Level"
#define VOLUME_FIELD     "Volume"
#define SKIN_FIELD       "SkinName"
#define PURCHASES_FIELD  "Purchases"
#define TAG_FIELD        "Tag"
public:
#pragma mark -
#pragma mark Constructors
	/** Creates a new, empty SaveController. */
	SaveController();
	/** Destroys this SaveController. */
	virtual ~SaveController();

	/** Initializes this SaveController. */
	void init();

#pragma mark -
#pragma mark Loaders
	/** Loads the player's save game from a json file
		Returns true if succeeds, else returns false
		*/
	int loadSaveGame(const std::shared_ptr<JsonValue>& json);

	/** Loads the Player Moose from a json file
		Returns true if succeeds, else returns false 
		*/
	std::shared_ptr<Moose> loadPlayerMoose(const std::shared_ptr<JsonValue>& json);

	/** Loads the Opponent Moose from a json file
	Returns true if succeeds, else returns false
	*/
	std::shared_ptr<Moose> loadOpponentMoose(const std::shared_ptr<JsonValue>& json);

	/** Loads the AI type from a json file
	Returns a shared pointer to the AI */
	AIType loadAI(const std::shared_ptr<JsonValue>& json);

	/** Loads the level tag from a json file
	Returns true if succeeds, else returns false
	*/
	int loadLevelTag(const std::shared_ptr<JsonValue>& json);

#pragma mark - 
#pragma mark Savers
	/** Saves the overall current state of the game to a json file */
	void saveGame(int level);
	/** Saves the current level's game state to a json file */
	void saveLevel(std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, std::shared_ptr<AI> ai, int level);

#pragma mark -
#pragma mark Accessors
	/** Returns the loaded player moose
		CALL LoadPlayerMoose FIRST BEFORE CALLING THIS METHOD
	*/
	const std::shared_ptr<Moose>& getPlayer() { return player; };

	/** Returns the loaded opponent moose
		CALL LoadOpponentMoose FIRST BEFORE CALLING THIS METHOD
	*/
	const std::shared_ptr<Moose>& getOpponent() { return opponent; };

};
#endif /* __SAVE_CONTROLLER_H__ */