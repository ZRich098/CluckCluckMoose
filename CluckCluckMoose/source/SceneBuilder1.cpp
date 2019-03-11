//Scene builder file

//Import important files
#include "Moose.h"
#include "SceneBuilder1.h"

using namespace cugl;

std::shared_ptr<cugl::AssetManager> _assets;
//std::unordered_map<std::string, std::shared_ptr<cugl::Button>> _buttons;

//CCMInput _input;

//Track input
Vec2 inputTrack;



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
std::shared_ptr<Node> chickenCanvas7;
std::shared_ptr<Node> chickenCanvas8;

//Canvases for drawing opp chickens
std::shared_ptr<Node> chickenCanvas4;
std::shared_ptr<Node> chickenCanvas5;
std::shared_ptr<Node> chickenCanvas6;
std::shared_ptr<Node> chickenCanvas9;
std::shared_ptr<Node> chickenCanvas10;

//Canvas for background
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




bool SceneBuilder1::init(const std::shared_ptr<cugl::AssetManager>& assets, const Size dimen, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp) {

	playerGlobe = player;
	oppGlobe = opp;

	inputTrack = Vec2();

	_assets = assets;
	_input.init();

	layer = assets->get<Node>("game");
	layer->setContentSize(dimen);
	layer->doLayout(); // This rearranges the children to fit the screen
	root->addChild(layer);


	//Create a node for drawing moose
	mooseCanvas = Node::alloc();
	layer->addChild(mooseCanvas);



	//Create a node for drawing chickens at each level of stacking
	chickenCanvas1 = Node::alloc();
	layer->addChild(chickenCanvas1);
	chickenCanvas1->setPosition(100, 0);

	chickenCanvas2 = Node::alloc();
	layer->addChild(chickenCanvas2);
	chickenCanvas2->setPosition(100, 150);

	chickenCanvas3 = Node::alloc();
	layer->addChild(chickenCanvas3);
	chickenCanvas3->setPosition(100, 300);

	chickenCanvas4 = Node::alloc();
	layer->addChild(chickenCanvas4);
	chickenCanvas4->setPosition(100, 0);

	chickenCanvas5 = Node::alloc();
	layer->addChild(chickenCanvas5);
	chickenCanvas5->setPosition(100, 150);

	chickenCanvas6 = Node::alloc();
	layer->addChild(chickenCanvas6);
	chickenCanvas6->setPosition(100, 300);

	chickenCanvas7 = Node::alloc();
	layer->addChild(chickenCanvas7);
	chickenCanvas7->setPosition(100, 450);

	chickenCanvas8 = Node::alloc();
	layer->addChild(chickenCanvas8);
	chickenCanvas8->setPosition(100, 600);

	chickenCanvas9 = Node::alloc();
	layer->addChild(chickenCanvas9);
	chickenCanvas9->setPosition(100, 450);

	chickenCanvas10 = Node::alloc();
	layer->addChild(chickenCanvas10);
	chickenCanvas10->setPosition(100, 600);


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

void SceneBuilder1::buildChicken(std::shared_ptr<Texture> texture, std::shared_ptr<Node> node, int posX, int posY, bool flip) {
	int ct = node->getChildCount();
	if (ct > 0) {
		node->removeAllChildren();
	}
	std::shared_ptr<PolygonNode> chick = PolygonNode::allocWithTexture(texture);
	chick->setScale(0.3f); // Magic number to rescale asset
	chick->setAnchor(Vec2::ANCHOR_CENTER);
	chick->setPosition(posX, posY);
	chick->flipHorizontal(flip);
	node->addChild(chick);
}

void SceneBuilder1::buildGameScene() {

	//reset drawing between frames
	mooseCanvas->removeAllChildren();
	chickenCanvas1->removeAllChildren();
	chickenCanvas2->removeAllChildren();
	chickenCanvas3->removeAllChildren();
	chickenCanvas4->removeAllChildren();
	chickenCanvas5->removeAllChildren();
	chickenCanvas6->removeAllChildren();
	chickenCanvas7->removeAllChildren();
	chickenCanvas8->removeAllChildren();
	chickenCanvas9->removeAllChildren();
	chickenCanvas10->removeAllChildren();
	buttonCanvas->removeAllChildren();

	//Draw player moose
	std::shared_ptr<Texture> textureM = _assets->get<Texture>("moose");
	std::shared_ptr<PolygonNode> moose1 = PolygonNode::allocWithTexture(textureM);
	moose1->setScale(0.4f); // Magic number to rescale asset
	moose1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	moose1->setPosition(-100, 0);
	moose1->flipHorizontal(false);
	mooseCanvas->addChild(moose1);

	//Draw opponent moose
	std::shared_ptr<PolygonNode> moose2 = PolygonNode::allocWithTexture(textureM);
	moose2->setScale(0.4f); // Magic number to rescale asset
	moose2->setAnchor(Vec2::ANCHOR_BOTTOM_RIGHT);
	moose2->setPosition(SCENE_WIDTH + 100, 0);
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
		id->setAnchor(Vec2::ANCHOR_CENTER);
		id->setScale(0.2, 0.2);
		id->flipHorizontal(true);
		std::shared_ptr<Button> butt = Button::alloc(id);
		butt->setAnchor(Vec2::ANCHOR_CENTER);
		butt->setScale(0.9, 0.9);

		


		butt->setAnchor(Vec2::ANCHOR_CENTER);
		if (i < 3) {
			butt->setPosition(i * 200 - 200, 50);
		}
		else {
			butt->setPosition((i-3) * 200 - 200, -100);
		}
		if (_input.isActive()) {
			CULog("active");
		}
		butt->setListener([=](const std::string& name, bool down) {
			if (down) {
				playerGlobe->addToStackFromHand(i);
			}
		});

		buttonCanvas->addChild(butt);
		butt->activate(0);


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
			buildChicken(text, chickenCanvas1, 100, 525, true);
		}
		else if (i == 1) {
			buildChicken(text, chickenCanvas2, 100, 525, true);
		}
		else {
			buildChicken(text, chickenCanvas3, 100, 525, true);
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
			buildChicken(text, chickenCanvas4, 750, 525, false);
		}
		else if (i == 1) {
			buildChicken(text, chickenCanvas5, 750, 525, false);
		}
		else {
			buildChicken(text, chickenCanvas6, 750, 525, false);
		}
	}
}

void SceneBuilder1::updateInput(float timestep) {
	_input.update(timestep);
	
}

//Dispose of the scene builder
void SceneBuilder1::dispose() {
	_assets = nullptr;
	_input.dispose();
	//_buttons.clear();
}
