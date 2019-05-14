#ifndef __TUTORIAL_SB_H__
#define __TUTORIAL_SB_H__

#include "SceneBuilder1.h"

using namespace cugl;

//Drawing constants
#define GRASS_FILMSTRIP_LENGTH 13
#define SPY_FILMSTRIP_LENGTH 16
#define TUTOR1_LENGTH 17
#define TUTOR6_LENGTH 10

/* 
	Tutorial Steps

	0. Begin Tutorial: show tutor 1 and highlight grass chicken.  Disable all chicken buttons but grass.
	1. Grass chicken dragged to stack, stop highlighting it.  Stop showing tutorial 1. Show tutorial 2.
	2. Preview pressed.  Stop showing tutorial 2.
	3. Preview let go.  Show tutorial 3.  
	4. Tutorial 3 pressed.  Stop showing tutorial 3.  Show tutorial 4.  Highlight spy Chicken.  
	5. Spy card seen. Stop showing Tutorial 4.  stop highlighting spy. 
	6. Spy card let go. Show tutorial 5.  
	7. Tutorial 5 tapped.  Stop showing tutorial 5. Wait till stack size reaches 5
	8. Stack size reached 5.  Show tutorial 6.  
	9. Tutorial 6 tapped.  stop showing tutorial 6.  Wait for clash animation to end.
	10. Showing Tutorial 7. 
	11. Tutorial 7 tapped.  ending tutorial.  
*/
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
	void advanceTutorial();
};


#endif /* __TUTORIAL_SB_H__ */