#ifndef __TUTORIAL_SB_H__
#define __TUTORIAL_SB_H__

#include "SceneBuilder1.h"

using namespace cugl;

//Drawing constants


class TutorialSB : public SceneBuilder1 {
protected:
	//What step the tutorial is currently on
	int step;
	//check to see if buttons in tutorial are pressed
	bool tutbuttonpressed = false;

	//Texture Allocations
	//Chicken Highlights
	std::shared_ptr<Texture> spylight;
	std::shared_ptr<Texture> grasslight;

	//Tutorials
	std::shared_ptr<Texture> tutor1;
	std::shared_ptr<Texture> tutor2;
	std::shared_ptr<Texture> tutor3;
	std::shared_ptr<Texture> tutor4;
	std::shared_ptr<Texture> tutor5;
	std::shared_ptr<Texture> tutor6;
	std::shared_ptr<Texture> tutor7;

	//Hightlight Canvases
	std::shared_ptr<AnimationNode> grasslightcanvas;
	std::shared_ptr<AnimationNode> spylightcanvas;

	//Tutorial Canvases
	std::shared_ptr<AnimationNode> tutcanvas1;
	std::shared_ptr<Node> tutcanvas2;
	std::shared_ptr<Node> tutcanvas3;
	std::shared_ptr<Node> tutcanvas4;
	std::shared_ptr<Node> tutcanvas5;
	std::shared_ptr<AnimationNode> tutcanvas6;
	std::shared_ptr<Node> tutcanvas7;

	//Button list for tutorials
	std::shared_ptr<Button> tutbutton3;
	std::shared_ptr<Button> tutbutton5;
	std::shared_ptr<Button> tutbutton6;
	std::shared_ptr<Button> tutbutton7;

public:
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

	//Allocate a tutorial scene builder
	static std::shared_ptr<TutorialSB> alloc(const std::shared_ptr<cugl::AssetManager>& assets, const cugl::Size size, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, string costume, int levelNum) {
		std::shared_ptr<TutorialSB> result = std::make_shared<TutorialSB>();
		return (result->init(assets, size, root, player, opp, costume, levelNum) ? result : nullptr);
	}

	//Build the game scene
	void updateGameScene(float timestep, bool isClashing);

	//Advance the tutorial 
};


#endif /* __TUTORIAL_SB_H__ */