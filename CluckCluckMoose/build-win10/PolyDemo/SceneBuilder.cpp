//Scene builder file

//Import important files
#include "Moose.h"
#include "SceneBuilder.h"

using namespace cugl;

std::shared_ptr<cugl::AssetManager> _assets;
std::unordered_map<std::string, std::shared_ptr<cugl::Button>> _buttons;

CCMInput _input;

/** The ID for the button listener */
#define LISTENER_ID 2
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024


//Main canvas to draw stuff to
std::shared_ptr<Node> layer;

//Canvases for drawing player chickens
std::shared_ptr<Node> chickenCanvas1;
std::shared_ptr<Node> chickenCanvas2;
std::shared_ptr<Node> chickenCanvas3;

//Canvases for drawing opp chickens
std::shared_ptr<Node> chickenCanvas4;
std::shared_ptr<Node> chickenCanvas5;
std::shared_ptr<Node> chickenCanvas6;

//Canvas fr background
std::shared_ptr<Node> backCanvas;

//Canvas for moose
std::shared_ptr<Node> mooseCanvas;

//Canvas for foreground
std::shared_ptr<Node> frontCanvas;

//Canvas for buttons
std::shared_ptr<Node> buttonCanvas;

//Players
std::shared_ptr<Moose> playerGlobe;
std::shared_ptr<Moose> oppGlobe;




bool SceneBuilder::init(const std::shared_ptr<cugl::AssetManager>& assets, const Size dimen, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp) {

	playerGlobe = player;
	oppGlobe = opp;

	_assets = assets;
	_input.init();
	layer = assets->get<Node>("game");
	layer->setContentSize(dimen);
	layer->doLayout(); // This rearranges the children to fit the screen
	root->addChild(layer);

	auto kids = layer->getChildren();
	std::shared_ptr<FloatLayout> layout = std::dynamic_pointer_cast<FloatLayout>(layer->getLayout());
	for (auto it = kids.begin(); it != kids.end(); ++it) {
		std::shared_ptr<Button> butt = std::dynamic_pointer_cast<Button>(*it);
		_buttons[butt->getName()] = butt;
		butt->setListener([=](const std::string& name, bool down) {
			if (down) {
				try {
					CULog("%s\n", butt->getName().c_str());
					if (butt->getName() == "chicken1") {
						player->addToStackFromHand(0);
					}
					else if (butt->getName() == "chicken2") {
						player->addToStackFromHand(1);
					}
					else if (butt->getName() == "chicken3") {
						player->addToStackFromHand(2);
					}
				}
				catch (std::out_of_range e) {}
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


	// Get chicken textures.
	std::shared_ptr<Texture> textureF = _assets->get<Texture>("fire");
	std::shared_ptr<Texture> textureW = _assets->get<Texture>("water");
	std::shared_ptr<Texture> textureG = _assets->get<Texture>("grass");

	return true;


}

void SceneBuilder::buildChicken(std::shared_ptr<Texture> texture, std::shared_ptr<Node> node, int posX, int posY, bool flip) {
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

void SceneBuilder::buildGameScene() {

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
	std::shared_ptr<Texture> textureF = _assets->get<Texture>("fire");
	std::shared_ptr<Texture> textureW = _assets->get<Texture>("water");
	std::shared_ptr<Texture> textureG = _assets->get<Texture>("grass");

	vector <Chicken> hand = playerGlobe->getHand();


	for (int i = 0; i < hand.size(); i++) {
		std::shared_ptr<Button> button;
		std::shared_ptr<Texture> text;
		element cel = playerGlobe->getHandAt(i).getElement();
		if (cel == (element::Fire)) {
			text = textureF;
		}
		else if (cel == (element::Water)) {
			text = textureW;
		}
		else {
			text = textureG;
		}
		std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(text);

		id->setScale(0.1, 0.1);


		id->setAnchor(Vec2::ANCHOR_CENTER);
		id->setPosition(i * 100 - 100, 0);

		buttonCanvas->addChild(id);
	}

	vector <Chicken> pstack = playerGlobe->getStack();

	for (int i = 0; i < pstack.size(); i++) {
		std::shared_ptr<Texture> text;
		element cel = playerGlobe->getStackAt(i).getElement();
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
			buildChicken(text, chickenCanvas1, 100, 300, true);
		}
		else if (i == 1) {
			buildChicken(text, chickenCanvas2, 100, 300, true);
		}
		else {
			buildChicken(text, chickenCanvas3, 100, 300, true);
		}
	}

	vector <Chicken> ostack = oppGlobe->getStack();

	for (int i = 0; i < ostack.size(); i++) {
		std::shared_ptr<Texture> text;
		element cel = oppGlobe->getStackAt(i).getElement();
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
			buildChicken(text, chickenCanvas4, 750, 300, false);
		}
		else if (i == 1) {
			buildChicken(text, chickenCanvas5, 750, 300, false);
		}
		else {
			buildChicken(text, chickenCanvas6, 750, 300, false);
		}
	}


}
