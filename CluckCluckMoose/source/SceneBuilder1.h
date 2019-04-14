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

class SceneBuilder1 {
protected:
	/** The asset manager for this scene builder. */
	std::shared_ptr<cugl::AssetManager> _assets;

	CCMInput _input;

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
	bool init(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Size size, const std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp);

	//Allocate a scene builder
	static std::shared_ptr<SceneBuilder1> alloc(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Size size, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp) {
		std::shared_ptr<SceneBuilder1> result = std::make_shared<SceneBuilder1>();
		return (result->init(assets, size, root, player, opp) ? result : nullptr);
	}

#pragma mark -
#pragma mark Scene Building Methods
	//Build the game scene
	void updateGameScene(float timestep);

	//Build an individual chicken using a texture and attach to a node.  Clear everything from the node beforehand.
	std::shared_ptr<cugl::AnimationNode> buildChicken(std::shared_ptr<cugl::Texture> texture, std::shared_ptr<cugl::Node> node, int posX, int posY, bool flip);



#pragma mark -
#pragma mark Input Methods
	//Update input
	void updateInput(float timestep);


#pragma mark -
#pragma mark Accessors
	//gets if preview button has been pressed
	bool getPreview() { return previewSet; }

#pragma mark -
#pragma mark Setters
	//sets previewSet to the given value
	void setPreview(bool preview);
};


#endif /* __CCM_GAME_SCENE_H__ */