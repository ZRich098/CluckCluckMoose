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
#include "Moose.h"

using namespace cugl;

/** The ID for the button listener */
#define LISTENER_ID 2
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

//stack size
int stackSize;

//Canvases for drawing player chickens
std::shared_ptr<Node> chickenCanvas1;
std::shared_ptr<Node> chickenCanvas2;
std::shared_ptr<Node> chickenCanvas3;

//Canvases for drawing opp chickens
std::shared_ptr<Node> chickenCanvas4;
std::shared_ptr<Node> chickenCanvas5;
std::shared_ptr<Node> chickenCanvas6;

//Canvas for moose
std::shared_ptr<Node> mooseCanvas;

//Canvas for buttons
std::shared_ptr<Node> buttonCanvas;


//Moose Players
Moose player;
Moose opp;


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
	for (auto it = kids.begin(); it != kids.end(); ++it) {
		std::shared_ptr<Button> butt = std::dynamic_pointer_cast<Button>(*it);
		_buttons[butt->getName()] = butt;
		butt->setListener([=](const std::string& name, bool down) {
			if (down) {
				CULog("%s\n", butt->getName().c_str());
				if (butt->getName() == "chicken1") {
					player.addToStackFromHand(0);
				}
				else if (butt->getName() == "chicken2") {
					player.addToStackFromHand(1);
				}
				else if (butt->getName() == "chicken3") {
					player.addToStackFromHand(2);
				}
			}
		});
	}

	//Create a node for drawing moose
	mooseCanvas = Node::alloc();
	layer->addChild(mooseCanvas);
	


	//Create a node for drawing chickens at each level of stacking
	chickenCanvas1 = Node::alloc();
	layer->addChild(chickenCanvas1);
	chickenCanvas1->setPosition(100, 0);

	chickenCanvas2 = Node::alloc();
	layer->addChild(chickenCanvas2);
	chickenCanvas2->setPosition(100, 100);

	chickenCanvas3 = Node::alloc();
	layer->addChild(chickenCanvas3);
	chickenCanvas3->setPosition(100, 200);

	chickenCanvas4 = Node::alloc();
	layer->addChild(chickenCanvas4);
	chickenCanvas4->setPosition(100, 0);

	chickenCanvas5 = Node::alloc();
	layer->addChild(chickenCanvas5);
	chickenCanvas5->setPosition(100, 100);

	chickenCanvas6 = Node::alloc();
	layer->addChild(chickenCanvas6);
	chickenCanvas6->setPosition(100, 200);

	//Add button canvas
	buttonCanvas = Node::alloc();
	layer->addChild(buttonCanvas);
	buttonCanvas->setPosition(SCENE_WIDTH / 2, 150);
	
	//Initialize stack sizes
	stackSize = 0;






	// Get chicken textures.
	std::shared_ptr<Texture> textureF = _assets->get<Texture>("fire");
	std::shared_ptr<Texture> textureW = _assets->get<Texture>("water");
	std::shared_ptr<Texture> textureG = _assets->get<Texture>("grass");

	//Initialize moose
	player = Moose::Moose(3, 3);
	opp = Moose::Moose(3, 3);
	player.refillHand();
	opp.refillHand();

	player.addToStackFromHand(0);
	opp.addToStackFromHand(0);



	GameScene::draw(assets, layer);

    
    
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

void GameScene::draw(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::Node> node) {
	
	//reset drawing between frames
	mooseCanvas->removeAllChildren();
	chickenCanvas1->removeAllChildren();
	chickenCanvas2->removeAllChildren();
	chickenCanvas3->removeAllChildren();
	chickenCanvas4->removeAllChildren();
	chickenCanvas5->removeAllChildren();
	chickenCanvas6->removeAllChildren();
	buttonCanvas->removeAllChildren();
	
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
	
	// Get chicken textures.
	std::shared_ptr<Texture> textureF = assets->get<Texture>("fire");
	std::shared_ptr<Texture> textureW = assets->get<Texture>("water");
	std::shared_ptr<Texture> textureG = assets->get<Texture>("grass");

	vector <Chicken> hand = player.getHand();

	
	for (int i = 0; i < hand.size(); i++) {
		std::shared_ptr<Button> button;
		std::shared_ptr<Texture> text;
		element cel = player.getHandAt(i).getElement();
		if (cel == (element::Fire)) {
			text = textureF;
		}
		else if (cel == (element::Water)) {
			text=textureW;
		}
		else {
			text=textureG;
		}
		std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(text);

		id->setScale(0.1, 0.1);
		

		id->setAnchor(Vec2::ANCHOR_CENTER);
		id->setPosition(i*100 - 100, 0);

		buttonCanvas->addChild(id);
	}

	vector <Chicken> pstack = player.getStack();

	for (int i = 0; i < pstack.size(); i++) {
		std::shared_ptr<Texture> text;
		element cel = player.getStackAt(i).getElement();
		if (cel == (element::Fire)) {
			text = textureF;
		}
		else if (cel == (element::Water)) {
			text = textureW;
		}
		else {
			text = textureG;
		}
		if (i == 0) {
			makeChicken(assets, text, chickenCanvas1, 100, 300, true);
		}
		else if (i == 1) {
			makeChicken(assets, text, chickenCanvas2, 100, 300, true);
		} else {
			makeChicken(assets, text, chickenCanvas3, 100, 300, true);
		}
	}

	vector <Chicken> ostack = opp.getStack();

	for (int i = 0; i < ostack.size(); i++) {
		std::shared_ptr<Texture> text;
		element cel = opp.getStackAt(i).getElement();
		if (cel == (element::Fire)) {
			text = textureF;
		}
		else if (cel == (element::Water)) {
			text = textureW;
		}
		else {
			text = textureG;
		}
		if (i == 0) {
			makeChicken(assets, text, chickenCanvas4, 750, 300, false);
		}
		else if (i == 1) {
			makeChicken(assets, text, chickenCanvas5, 750, 300, false);
		}
		else {
			makeChicken(assets, text, chickenCanvas6, 750, 300, false);
		}
	}


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
