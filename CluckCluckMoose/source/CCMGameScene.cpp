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
#include "CCMGameScene.h"

using namespace cugl;

/** The ID for the button listener */
#define LISTENER_ID 2
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

//stack size
int stackSize;

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
bool GameScene::init(const std::shared_ptr<AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }
    
    _assets = assets;
    _input.init();
    auto layer = assets->get<Node>("game");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    addChild(layer);
    
    auto kids = layer->getChildren();
    std::shared_ptr<FloatLayout> layout = std::dynamic_pointer_cast<FloatLayout>(layer->getLayout());

	//Create a node for drawing moose
	std::shared_ptr<Node> mooseCanvas = Node::alloc();
	layer->addChild(mooseCanvas);
	//Draw player moose
	std::shared_ptr<Texture> textureM = _assets->get<Texture>("moose");
	std::shared_ptr<PolygonNode> moose1 = PolygonNode::allocWithTexture(textureM);
	moose1->setScale(0.3f); // Magic number to rescale asset
	moose1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	moose1->setPosition(0, -100);
	moose1->flipHorizontal(false);
	mooseCanvas->addChild(moose1);
	//Draw opponent moose
	std::shared_ptr<PolygonNode> moose2 = PolygonNode::allocWithTexture(textureM);
	moose2->setScale(0.3f); // Magic number to rescale asset
	moose2->setAnchor(Vec2::ANCHOR_BOTTOM_RIGHT);
	moose2->setPosition(SCENE_WIDTH, -100);
	moose2->flipHorizontal(true);
	mooseCanvas->addChild(moose2);




	
	//Create a node for drawing chickens at each level of stacking
	std::shared_ptr<Node> chickenCanvas1 = Node::alloc();
	layer->addChild(chickenCanvas1);
	chickenCanvas1->setPosition(-300, 0);

	std::shared_ptr<Node> chickenCanvas2 = Node::alloc();
	layer->addChild(chickenCanvas2);
	chickenCanvas2->setPosition(-300, 100);

	std::shared_ptr<Node> chickenCanvas3 = Node::alloc();
	layer->addChild(chickenCanvas3);
	chickenCanvas3->setPosition(-300, 200);

	stackSize = 0;




	// Get chicken textures.
	std::shared_ptr<Texture> textureF = _assets->get<Texture>("fire");
	std::shared_ptr<Texture> textureW = _assets->get<Texture>("water");
	std::shared_ptr<Texture> textureG = _assets->get<Texture>("grass");

	//Find and move the end turn button
	layer->getChildByName("endTurn")->setPosition(SCENE_WIDTH / 2, 300);


    for(auto it = kids.begin(); it != kids.end(); ++it) {
        std::shared_ptr<Button> buttn = std::dynamic_pointer_cast<Button>(*it);
        _buttons[buttn->getName()] = buttn;
        buttn->setListener([=](const std::string& name, bool down) {
			if (down) {
				std::shared_ptr<Node> workingCanvas;
				if (stackSize == 0) {
					workingCanvas = chickenCanvas1;
				}
				else if (stackSize ==1) {
					workingCanvas = chickenCanvas2;
				}
				else if (stackSize == 2) {
					workingCanvas = chickenCanvas3;
				}
				else {

				}
				if (buttn->getName() == "chicken1") {
					if (workingCanvas != nullptr) {
						makeChicken(assets, textureF, workingCanvas, SCENE_WIDTH / 2, 300, true);
					}
				}
				else if (buttn->getName() == "chicken2") {
					if (workingCanvas != nullptr) {
						makeChicken(assets, textureW, workingCanvas, SCENE_WIDTH / 2, 300, true);
					}
				}
				else if (buttn->getName() == "chicken3") {
					if (workingCanvas != nullptr) {
						makeChicken(assets, textureG, workingCanvas, SCENE_WIDTH / 2, 300, true);
					}
				}
				else if (buttn->getName() == "endTurn") {
					stackSize++;
				}
				else {

				}
                CULog("%s\n", buttn->getName().c_str());
                CULog("(%f, %f)\n", _input.getCurTouch().x, _input.getCurTouch().y);
            }
        });
    }
    
    setActive(_active);
    
    // XNA nostalgia
    Application::get()->setClearColor(Color4f::CORNFLOWER);
    return true;
}

void GameScene::makeChicken(const std::shared_ptr<AssetManager>& assets, std::shared_ptr<Texture> texture, std::shared_ptr<Node> node, int posX, int posY, bool flip) {
	int ct = node->getChildCount();
	if (ct > 0) {
		node->removeAllChildren();
	}
	std::shared_ptr<PolygonNode> chick = PolygonNode::allocWithTexture(texture);
	chick->setScale(0.2f); // Magic number to rescale asset
	chick->setAnchor(Vec2::ANCHOR_CENTER);
	chick->setPosition(posX, posY);
	chick->flipHorizontal(flip);
	node->addChild(chick);
}



/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    _assets = nullptr;
    _input.dispose();
    _buttons.clear();
    Scene::dispose();
}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void GameScene::setActive(bool value) {
    _active = value;
    int pos = LISTENER_ID;
    for(auto it = _buttons.begin(); it != _buttons.end(); ++it) {
        if (value && !it->second->isActive()) {
            it->second->activate(pos++);
        } else {
            it->second->deactivate();
        }
    }
}
