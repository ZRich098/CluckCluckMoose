//Scene builder file

//Import important files
#include "Moose.h"
#include "SceneBuilder1.h"

using namespace cugl;

std::shared_ptr<AssetManager> _assets;

//Button list for player hand
std::vector<std::shared_ptr<Button>> buttons;

//Texture list for determining which textures need to be replaced in the hand
std::vector<std::shared_ptr<Texture>> texturesHand;

//List of player stack nodes
std::vector<std::shared_ptr<PolygonNode>> pstackNodes;
//Texture list for determining which textures need to be replaced in the player stack
std::vector<std::shared_ptr<Texture>> texturesPStack;

//List of opponent stack nodes
std::vector<std::shared_ptr<PolygonNode>> ostackNodes;
//Texture list for determining which textures need to be replaced in the opponent stack
std::vector<std::shared_ptr<Texture>> texturesOStack;

//Track held chicken
std::shared_ptr<Button> heldButton;


/** The ID for the button listener */
#define LISTENER_ID 2
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

//Textures
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


//Main canvas to draw stuff to
std::shared_ptr<Node> layer;

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

	root->removeAllChildren();

	playerGlobe = player;
	oppGlobe = opp;

	heldButton = nullptr;

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


	//Add button canvas
	buttonCanvas = Node::alloc();
	layer->addChild(buttonCanvas);
	//buttonCanvas->setPosition(SCENE_WIDTH / 2, 150);

	// Get chicken textures.
	textureF = _assets->get<Texture>("fire");
	textureW = _assets->get<Texture>("water");
	textureG = _assets->get<Texture>("grass");
	textureReaper = _assets->get<Texture>("reaper");
	textureBomb = _assets->get<Texture>("bomb");
	textureMirror = _assets->get<Texture>("mirror");
	textureNinja = _assets->get<Texture>("ninja");
	textureParty = _assets->get<Texture>("party");
	textureSpy = _assets->get<Texture>("spy");
	textureThick = _assets->get<Texture>("thicken");
	textureWitch = _assets->get<Texture>("witchen");


	//reset drawing between frames
	backCanvas->removeAllChildren();
	mooseCanvas->removeAllChildren();
	frontCanvas->removeAllChildren();
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

	//Init appropriately sized buttons
	for (int i = 0; i < 6; i++) {
		std::shared_ptr<Button> button;
		std::shared_ptr<Texture> text;
		text = textureF;
		
		std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(text);
		id->setAnchor(Vec2::ANCHOR_CENTER);
		id->flipHorizontal(true);
		std::shared_ptr<Button> butt = Button::alloc(id);
		butt->setAnchor(Vec2::ANCHOR_CENTER);
		butt->setScale(0.5, 0.5);

		butt->setAnchor(Vec2::ANCHOR_CENTER);
		if (i < 3) {
			butt->setPosition((i-1)*200 + 500, 250);
		}
		else {
			butt->setPosition((i - 4) * 200 + 500, 100);
		}
		if (_input.isActive()) {
			//CULog("active");
		}
		butt->setListener([=](const std::string& name, bool down) {
			if (down) {
				
				heldButton = butt;
			
			}
			if (!down) {
				heldButton = nullptr;
				if (butt->getPositionX() < 200 && butt->getPositionY() > 100) {
					playerGlobe->addToStackFromHand(i);
				}
				if (i < 3) {
					butt->setPosition(Vec2((i - 1) * 200 + 500, 250));
				}
				else {
					butt->setPosition(Vec2((i - 4) * 200 + 500, 100));
				}
			}
		});

		buttonCanvas->addChild(butt);
		//i+2 to ensure keys are unique
		butt->activate(i + 2);
		//CULog("Button %d made", i);
		buttons.push_back(butt);
		texturesHand.push_back(text);
	}

	//Init stack nodes
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<Texture> text;
		text = textureF;
		std::shared_ptr<PolygonNode> poly;
		
		if (i == 0) {
			poly = buildChicken(text, layer, 150, 975, true);
		}
		else if (i == 1) {
			poly = buildChicken(text, layer, 150, 1085, true);
		}
		else if (i == 2) {
			poly = buildChicken(text, layer, 150, 1195, true);
		}
		else if (i == 3) {
			poly = buildChicken(text, layer, 150, 1305, true);
		}
		else {
			poly = buildChicken(text, layer, 150, 1415, true);
		}
		pstackNodes.push_back(poly);
		texturesPStack.push_back(text);
	}


	for (int i = 0; i < 5; i++) {
		std::shared_ptr<Texture> text;
		text = textureF;
		std::shared_ptr<PolygonNode> poly;

		if (i == 0) {
			poly = buildChicken(text, layer, 900, 975, false);
		}
		else if (i == 1) {
			poly = buildChicken(text, layer, 900, 1085, false);
		}
		else if (i == 2) {
			poly = buildChicken(text, layer, 900, 1195, false);
		}
		else if (i == 3) {
			poly = buildChicken(text, layer, 900, 1305, false);

		}
		else {
			poly = buildChicken(text, layer, 900, 1415, false);

		}
		
		ostackNodes.push_back(poly);
		texturesOStack.push_back(text);
	}

	//Init the clash preview button

	std::shared_ptr<Texture> textureFight = _assets->get<Texture>("preview");

	std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(textureFight);
	id->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> butt = Button::alloc(id);
	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setScale(0.5, 0.5);

	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setPosition(500, 800);
	butt->setListener([=](const std::string& name, bool down) {
		if (down) {
			previewSet = true;
		}
	});

	buttonCanvas->addChild(butt);
	//ensure keys are unique
	butt->activate(99);

	return true;
}

std::shared_ptr<PolygonNode> SceneBuilder1::buildChicken(std::shared_ptr<Texture> texture, std::shared_ptr<Node> node, int posX, int posY, bool flip) {
	
	std::shared_ptr<PolygonNode> chick = PolygonNode::allocWithTexture(texture);
	chick->setScale(0.8f); // Magic number to rescale asset
	chick->setAnchor(Vec2::ANCHOR_CENTER);
	chick->setPosition(posX, posY);
	chick->flipHorizontal(flip);
	node->addChild(chick);

	return chick;
}

void SceneBuilder1::updateGameScene() {

	
	vector <Chicken> hand = playerGlobe->getHand();

	for (int i = 0; i < 6; i++) {
		if (i < hand.size()) {
			buttons[i]->setVisible(true);
			buttons[i]->activate(i + 2);
			if (buttons[i] == heldButton) {
				buttons[i]->setPosition(layer->screenToNodeCoords(_input.getCurTouch()));
			}
		}
		else {
			buttons[i]->setVisible(false);
			buttons[i]->deactivate();
		}
	}


	for (int i = 0; i < hand.size(); i++) {
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

		std::shared_ptr<Node> upchld = buttons[i]->getChild(0);
		
		std::shared_ptr<PolygonNode> newUp = PolygonNode::allocWithTexture(text);
		buttons[i]->swapChild(upchld, newUp, false);
		
	}

	Stack pstack = playerGlobe->getStack();

	for (int i = 0; i < 5; i++) {
		if (i < pstack.getSize()) {
			pstackNodes[i]->setVisible(true);
		}
		else {
			pstackNodes[i]->setVisible(false);
		}
	}

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
		if (texturesPStack[i] != text) {
			pstackNodes[i]->setTexture(text);
			texturesPStack[i] = text;
		}
	}

	Stack ostack = oppGlobe->getStack();

	for (int i = 0; i < 5; i++) {
		if (i < ostack.getSize()) {
			ostackNodes[i]->setVisible(true);
		}
		else {
			ostackNodes[i]->setVisible(false);
		}
	}

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
			switch (oppGlobe->getStackAt(i).getElement()) {
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
		if (texturesOStack[i] != text) {
			ostackNodes[i]->setTexture(text);
			texturesOStack[i] = text;
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

void SceneBuilder1::setPreview(bool preview) {
	previewSet = preview;
}
