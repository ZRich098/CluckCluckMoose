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

//Preview tracking
bool previewSet;


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

	//Create background node
	backCanvas = Node::alloc();
	layer->addChild(backCanvas);

	//Create a node for drawing moose
	mooseCanvas = Node::alloc();
	layer->addChild(mooseCanvas);

	//Create foreground node
	frontCanvas = Node::alloc();
	layer->addChild(frontCanvas);

	//Create a node for drawing chickens at each level of stacking
	chickenCanvas1 = Node::alloc();
	layer->addChild(chickenCanvas1);
	chickenCanvas1->setPosition(50, 400);

	chickenCanvas2 = Node::alloc();
	layer->addChild(chickenCanvas2);
	chickenCanvas2->setPosition(50, 510);

	chickenCanvas3 = Node::alloc();
	layer->addChild(chickenCanvas3);
	chickenCanvas3->setPosition(50, 620);

	chickenCanvas4 = Node::alloc();
	layer->addChild(chickenCanvas4);
	chickenCanvas4->setPosition(150, 400);

	chickenCanvas5 = Node::alloc();
	layer->addChild(chickenCanvas5);
	chickenCanvas5->setPosition(150, 510);

	chickenCanvas6 = Node::alloc();
	layer->addChild(chickenCanvas6);
	chickenCanvas6->setPosition(150, 620);

	chickenCanvas7 = Node::alloc();
	layer->addChild(chickenCanvas7);
	chickenCanvas7->setPosition(50, 730);

	chickenCanvas8 = Node::alloc();
	layer->addChild(chickenCanvas8);
	chickenCanvas8->setPosition(50, 840);

	chickenCanvas9 = Node::alloc();
	layer->addChild(chickenCanvas9);
	chickenCanvas9->setPosition(150, 730);

	chickenCanvas10 = Node::alloc();
	layer->addChild(chickenCanvas10);
	chickenCanvas10->setPosition(150, 840);

	//Add button canvas
	buttonCanvas = Node::alloc();
	layer->addChild(buttonCanvas);
	buttonCanvas->setPosition(SCENE_WIDTH / 2, 150);

	// Get chicken textures.
	std::shared_ptr<Texture> textureF = _assets->get<Texture>("fire");
	std::shared_ptr<Texture> textureW = _assets->get<Texture>("water");
	std::shared_ptr<Texture> textureG = _assets->get<Texture>("grass");
	std::shared_ptr<Texture> textureReaper = _assets->get<Texture>("reaper");
	std::shared_ptr<Texture> textureBomb = _assets->get<Texture>("bomb");
	std::shared_ptr<Texture> textureMirror = _assets->get<Texture>("mirror");
	std::shared_ptr<Texture> textureNinja = _assets->get<Texture>("ninja");
	std::shared_ptr<Texture> textureParty = _assets->get<Texture>("party");
	std::shared_ptr<Texture> textureSpy = _assets->get<Texture>("spy");
	std::shared_ptr<Texture> textureThick = _assets->get<Texture>("thicken");
	std::shared_ptr<Texture> textureWitch = _assets->get<Texture>("witchen");

	// Get button textures.
	std::shared_ptr<Texture> textureFight = _assets->get<Texture>("preview");
	previewSet = false;

	return true;
}

void SceneBuilder1::buildChicken(std::shared_ptr<Texture> texture, std::shared_ptr<Node> node, int posX, int posY, bool flip) {
	int ct = node->getChildCount();
	if (ct > 0) {
		node->removeAllChildren();
	}
	std::shared_ptr<PolygonNode> chick = PolygonNode::allocWithTexture(texture);
	chick->setScale(0.8f); // Magic number to rescale asset
	chick->setAnchor(Vec2::ANCHOR_CENTER);
	chick->setPosition(posX, posY);
	chick->flipHorizontal(flip);
	node->addChild(chick);
}

void SceneBuilder1::buildGameScene() {

	//reset drawing between frames
	backCanvas->removeAllChildren();
	mooseCanvas->removeAllChildren();
	frontCanvas->removeAllChildren();
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

	//Draw background
	std::shared_ptr<Texture> texturebg = _assets->get<Texture>("farmbg");
	std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
	background->setScale(1.2f); // Magic number to rescale asset
	background->setAnchor(Vec2::ANCHOR_CENTER);
	background->setPosition(SCENE_WIDTH/2, 512);
	backCanvas->addChild(background);
	
	//Draw player moose
	std::shared_ptr<Texture> textureM = _assets->get<Texture>("moose");
	std::shared_ptr<PolygonNode> moose1 = PolygonNode::allocWithTexture(textureM);
	moose1->setScale(0.3f); // Magic number to rescale asset
	moose1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	moose1->setPosition(-75, 550);
	moose1->flipHorizontal(false);
	mooseCanvas->addChild(moose1);

	//Draw opponent moose
	std::shared_ptr<PolygonNode> moose2 = PolygonNode::allocWithTexture(textureM);
	moose2->setScale(0.3f); // Magic number to rescale asset
	moose2->setAnchor(Vec2::ANCHOR_BOTTOM_RIGHT);
	moose2->setPosition(SCENE_WIDTH + 75, 550);
	moose2->flipHorizontal(true);
	mooseCanvas->addChild(moose2);

	//Draw foreground
	std::shared_ptr<Texture> texturefg = _assets->get<Texture>("farmfg");
	std::shared_ptr<PolygonNode> foreground = PolygonNode::allocWithTexture(texturefg);
	foreground->setScale(1.0f); // Magic number to rescale asset
	foreground->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
	foreground->setPosition(SCENE_WIDTH/2, 300);
	frontCanvas->addChild(foreground);

	// Get chicken textures.
	std::shared_ptr<Texture> textureF = _assets->get<Texture>("fire");
	std::shared_ptr<Texture> textureW = _assets->get<Texture>("water");
	std::shared_ptr<Texture> textureG = _assets->get<Texture>("grass");
	std::shared_ptr<Texture> textureReaper = _assets->get<Texture>("reaper");
	std::shared_ptr<Texture> textureBomb = _assets->get<Texture>("bomb");
	std::shared_ptr<Texture> textureMirror = _assets->get<Texture>("mirror");
	std::shared_ptr<Texture> textureNinja = _assets->get<Texture>("ninja");
	std::shared_ptr<Texture> textureParty = _assets->get<Texture>("party");
	std::shared_ptr<Texture> textureSpy = _assets->get<Texture>("spy");
	std::shared_ptr<Texture> textureThick = _assets->get<Texture>("thicken");
	std::shared_ptr<Texture> textureWitch = _assets->get<Texture>("witchen");

	vector <Chicken> hand = playerGlobe->getHand();

	for (int i = 0; i < hand.size(); i++) {
		std::shared_ptr<Button> button;
		std::shared_ptr<Texture> text;
		special cel = playerGlobe->getHandAt(i).getSpecial();
		switch (cel) {
		case special::BasicFire:
			text = textureF;
			break;
		case special::BasicWater:
			text = textureW;
			break;
		case special::BasicGrass:
			text = textureG;
			break;
		case special::Reaper:
			text = textureReaper;
			break;
		case special::Bomb:
			text = textureBomb;
			break;
		case special::Mirror:
			text = textureMirror;
			break;
		case special::Ninja:
			text = textureNinja;
			break;
		case special::PartyFowl:
			text = textureParty;
			break;
		case special::Spy:
			text = textureSpy;
			break;
		case special::Thicken:
			text = textureThick;
			break;
		case special::Consigliere:
			text = textureWitch;
			break;
		default:
			switch (playerGlobe->getHandAt(i).getElement()) {
			case element::Fire:
				text = textureF;
				break;
			case element::Water:
				text = textureW;
				break;
			case element::Grass:
				text = textureG;
				break;
			}
		}
		std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(text);
		id->setAnchor(Vec2::ANCHOR_CENTER);
		id->flipHorizontal(true);
		std::shared_ptr<Button> butt = Button::alloc(id);
		butt->setAnchor(Vec2::ANCHOR_CENTER);
		butt->setScale(0.5, 0.5);

		butt->setAnchor(Vec2::ANCHOR_CENTER);
		if (i < 3) {
			butt->setPosition(i * 200 - 200, 150);
		}
		else {
			butt->setPosition((i-3) * 200 - 200, 0);
		}
		if (_input.isActive()) {
			//CULog("active");
		}
		butt->setListener([=](const std::string& name, bool down) {
			if (down) {
				playerGlobe->addToStackFromHand(i);
			}
		});

		buttonCanvas->addChild(butt);
		//i+2 to ensure keys are unique
		butt->activate(i+2);
		//CULog("Button %d made", i);
	}

	Stack pstack = playerGlobe->getStack();

	for (int i = 0; i < pstack.getSize(); i++) {
		std::shared_ptr<Texture> text;
		special cel = playerGlobe->getStackAt(i).getSpecial();
		switch (cel) {
		case special::BasicFire:
			text = textureF;
			break;
		case special::BasicWater:
			text = textureW;
			break;
		case special::BasicGrass:
			text = textureG;
			break;
		case special::Reaper:
			text = textureReaper;
			break;
		case special::Bomb:
			text = textureBomb;
			break;
		case special::Mirror:
			text = textureMirror;
			break;
		case special::Ninja:
			text = textureNinja;
			break;
		case special::PartyFowl:
			text = textureParty;
			break;
		case special::Spy:
			text = textureSpy;
			break;
		case special::Thicken:
			text = textureThick;
			break;
		case special::Consigliere:
			text = textureWitch;
			break;
		default:
			switch (playerGlobe->getStackAt(i).getElement()) {
			case element::Fire:
				text = textureF;
				break;
			case element::Water:
				text = textureW;
				break;
			case element::Grass:
				text = textureG;
				break;
			}
		}
		if (i == 0) {
			buildChicken(text, chickenCanvas1, 100, 575, true);
		}
		else if (i == 1) {
			buildChicken(text, chickenCanvas2, 100, 575, true);
		}
		else if (i == 2){
			buildChicken(text, chickenCanvas3, 100, 575, true);
		}
		else if (i == 3) {
			buildChicken(text, chickenCanvas7, 100, 575, true);
		}
		else {
			buildChicken(text, chickenCanvas8, 100, 575, true);
		}
	}

	Stack ostack = oppGlobe->getStack();

	for (int i = 0; i < ostack.getSize(); i++) {
		std::shared_ptr<Texture> text;
		special cel = oppGlobe->getStackAt(i).getSpecial();
		switch (cel) {
		case special::BasicFire:
			text = textureF;
			break;
		case special::BasicWater:
			text = textureW;
			break;
		case special::BasicGrass:
			text = textureG;
			break;
		case special::Reaper:
			text = textureReaper;
			break;
		case special::Bomb:
			text = textureBomb;
			break;
		case special::Mirror:
			text = textureMirror;
			break;
		case special::Ninja:
			text = textureNinja;
			break;
		case special::PartyFowl:
			text = textureParty;
			break;
		case special::Spy:
			text = textureSpy;
			break;
		case special::Thicken:
			text = textureThick;
			break;
		case special::Consigliere:
			text = textureWitch;
			break;
		default:
			switch (oppGlobe->getHandAt(i).getElement()) {
			case element::Fire:
				text = textureF;
				break;
			case element::Water:
				text = textureW;
				break;
			case element::Grass:
				text = textureG;
				break;
			}
		}
		if (i == 0) {
			buildChicken(text, chickenCanvas4, 750, 575, false);
		}
		else if (i == 1) {
			buildChicken(text, chickenCanvas5, 750, 575, false);
		}
		else if (i == 2) {
			buildChicken(text, chickenCanvas6, 750, 575, false);
		}
		else if (i == 3) {
			buildChicken(text, chickenCanvas9, 750, 575, false);

		}
		else {
			buildChicken(text, chickenCanvas10, 750, 575, false);

		}
	}

	std::shared_ptr<Texture> textureFight = _assets->get<Texture>("preview");

	std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(textureFight);
	id->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> butt = Button::alloc(id);
	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setScale(0.5, 0.5);

	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setPosition(0, 600);
	butt->setListener([=](const std::string& name, bool down) {
		if (down) {
			previewSet = true;
		}
	});

	buttonCanvas->addChild(butt);
	//i+2 to ensure keys are unique
	butt->activate(hand.size()+2);
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
