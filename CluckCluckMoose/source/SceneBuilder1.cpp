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
#define SCENE_WIDTH 576
#define SCENE_HEIGHT 1024

//Drawing constants
#define MOOSE_HEIGHT 275
#define FORE_HEIGHT 125
#define MOOSE_X_OFFSET 50
#define HAND_SCALE 0.5f
#define STACK_SCALE 0.5f
#define BUTTON_X_SPACING 175
#define BUTTON_X_OFFSET 0
#define BUTTON_Y_SPACING 100
#define BUTTON_Y_OFFSET -75
#define STACK_X_OFFSET 75
#define STACK_Y_OFFSET 550
#define STACK_Y_SPACING 75
#define INFO_X_OFFSET 7
#define INFO_Y_OFFSET 100
#define INFO_SCALE 0.5
#define HEALTH_BAR_Y_OFFSET 925
#define HEALTH_BLOCK_SPACING 24
#define HEART_X_OFFSET 225
#define BAR_DISTANCE 165

//Chicken Textures
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

//Info Textures
std::shared_ptr<Texture> infoF;
std::shared_ptr<Texture> infoW;
std::shared_ptr<Texture> infoG;
std::shared_ptr<Texture> infoReaper;
std::shared_ptr<Texture> infoBomb;
std::shared_ptr<Texture> infoMirror;
std::shared_ptr<Texture> infoNinja;
std::shared_ptr<Texture> infoParty;
std::shared_ptr<Texture> infoSpy;
std::shared_ptr<Texture> infoThick;
std::shared_ptr<Texture> infoWitch;

//Health textures
std::shared_ptr<Texture> bar;
std::shared_ptr<Texture> pHeart;
std::shared_ptr<Texture> oHeart;
std::shared_ptr<Texture> pBlock;
std::shared_ptr<Texture> oBlock;



//Main canvas to draw stuff to
std::shared_ptr<Node> layer;

//Canvas for background
std::shared_ptr<Node> backCanvas;

//Canvas for moose
std::shared_ptr<Node> mooseCanvas;

//Canvas for foreground
std::shared_ptr<Node> frontCanvas;

//Canvas for info cards
std::shared_ptr<Node> infoCanvas;

//Canvas for buttons
std::shared_ptr<Node> buttonCanvas;

//Canvas for clash button
std::shared_ptr<Node> clashButtonCanvas;

//Canvas for health bar
std::shared_ptr<Node> healthCanvas;

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

	//Get info textures
	infoF = _assets->get<Texture>("fireInfo");
	infoW = _assets->get<Texture>("waterInfo");
	infoG = _assets->get<Texture>("grassInfo");
	infoReaper = _assets->get<Texture>("reapInfo");
	infoBomb = _assets->get<Texture>("tickInfo");
	infoMirror = _assets->get<Texture>("mirrorInfo");
	infoNinja = _assets->get<Texture>("shurInfo");
	infoParty = _assets->get<Texture>("partyInfo");
	infoSpy = _assets->get<Texture>("spyInfo");
	infoThick = _assets->get<Texture>("thickInfo");
	infoWitch = _assets->get<Texture>("witchInfo");

	//Get health textures
	bar = _assets->get<Texture>("healthBar");
	pHeart = _assets->get<Texture>("pHeart");
	oHeart = _assets->get<Texture>("oHeart");
	pBlock = _assets->get<Texture>("pBlock");
	oBlock = _assets->get<Texture>("oBlock");


	layer = assets->get<Node>("game");
	layer->setContentSize(dimen);
	layer->doLayout(); // This rearranges the children to fit the screen
	root->addChild(layer);

	//Create background node
	backCanvas = Node::alloc();
	layer->addChild(backCanvas);

	//Init stack nodes
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<Texture> text;
		text = textureF;
		std::shared_ptr<PolygonNode> poly;
		poly = buildChicken(text, layer, STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING), true);

		pstackNodes.push_back(poly);
		texturesPStack.push_back(text);
	}


	for (int i = 0; i < 5; i++) {
		std::shared_ptr<Texture> text;
		text = textureF;
		std::shared_ptr<PolygonNode> poly;
		poly = buildChicken(text, layer, SCENE_WIDTH - STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING), false);

		ostackNodes.push_back(poly);
		texturesOStack.push_back(text);
	}

	//Create a node for drawing moose
	mooseCanvas = Node::alloc();
	layer->addChild(mooseCanvas);

	//Create foreground node
	frontCanvas = Node::alloc();
	layer->addChild(frontCanvas);

	//Add clash button canvas
	clashButtonCanvas = Node::alloc();
	layer->addChild(clashButtonCanvas);
	clashButtonCanvas->setPosition(SCENE_WIDTH / 2, 150);

	//Create an info node
	infoCanvas = Node::alloc();
	layer->addChild(infoCanvas);

	//Add button canvas
	buttonCanvas = Node::alloc();
	layer->addChild(buttonCanvas);
	buttonCanvas->setPosition(SCENE_WIDTH / 2, 150);

	//Add health canvas
	healthCanvas = Node::alloc();
	layer->addChild(healthCanvas);

	

	


	//reset drawing between frames
	backCanvas->removeAllChildren();
	mooseCanvas->removeAllChildren();
	frontCanvas->removeAllChildren();
	buttonCanvas->removeAllChildren();

	//Draw background
	std::shared_ptr<Texture> texturebg = _assets->get<Texture>("farmbg");
	std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
	background->setScale(0.7f); // Magic number to rescale asset
	background->setAnchor(Vec2::ANCHOR_CENTER);
	background->setPosition(SCENE_WIDTH/2, SCENE_HEIGHT/2);
	backCanvas->addChild(background);

	
	
	//Draw player moose
	std::shared_ptr<Texture> textureM = _assets->get<Texture>("moose");
	std::shared_ptr<PolygonNode> moose1 = PolygonNode::allocWithTexture(textureM);
	moose1->setScale(0.2f); // Magic number to rescale asset
	moose1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	moose1->setPosition(-MOOSE_X_OFFSET, MOOSE_HEIGHT);
	moose1->flipHorizontal(false);
	mooseCanvas->addChild(moose1);

	//Draw opponent moose
	std::shared_ptr<PolygonNode> moose2 = PolygonNode::allocWithTexture(textureM);
	moose2->setScale(0.2f); // Magic number to rescale asset
	moose2->setAnchor(Vec2::ANCHOR_BOTTOM_RIGHT);
	moose2->setPosition(SCENE_WIDTH + MOOSE_X_OFFSET, MOOSE_HEIGHT);
	moose2->flipHorizontal(true);
	mooseCanvas->addChild(moose2);

	//Draw foreground
	std::shared_ptr<Texture> texturefg = _assets->get<Texture>("farmfg");
	std::shared_ptr<PolygonNode> foreground = PolygonNode::allocWithTexture(texturefg);
	foreground->setScale(0.7f); // Magic number to rescale asset
	foreground->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
	foreground->setPosition(SCENE_WIDTH/2, FORE_HEIGHT);
	frontCanvas->addChild(foreground);

	//Draw info
	std::shared_ptr<PolygonNode> info = PolygonNode::allocWithTexture(infoF);
	info->setScale(INFO_SCALE);
	info->setAnchor(Vec2::ANCHOR_CENTER);
	info->setPosition(SCENE_WIDTH / 2 + INFO_X_OFFSET, SCENE_HEIGHT/2 + INFO_Y_OFFSET);
	infoCanvas->addChild(info);
	infoCanvas->setVisible(false);
	

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
		butt->setScale(HAND_SCALE, HAND_SCALE);

		butt->setAnchor(Vec2::ANCHOR_CENTER);
		if (i < 3) {
			butt->setPosition((i-1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET);
		}
		else {
			butt->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING);
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
				if (butt->getPositionX() < MOOSE_X_OFFSET/2 && butt->getPositionY() > MOOSE_HEIGHT) {
					playerGlobe->addToStackFromHand(i);
				}
				if (i < 3) {
					butt->setPosition((i - 1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET);
				}
				else {
					butt->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING);
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

	

	//Init the clash preview button

	std::shared_ptr<Texture> textureFight = _assets->get<Texture>("preview");

	std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(textureFight);
	id->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> butt = Button::alloc(id);
	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setScale(0.4, 0.4);

	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setPosition(0, SCENE_HEIGHT/3);
	butt->setListener([=](const std::string& name, bool down) {
		if (down) {
			previewSet = true;
		}
	});

	clashButtonCanvas->addChild(butt);
	//ensure keys are unique
	butt->activate(99);

	//Draw initial health
	//Bar
	std::shared_ptr<PolygonNode> hBar = PolygonNode::allocWithTexture(bar);
	hBar->setAnchor(Vec2::ANCHOR_CENTER);
	hBar->setScale(0.7);
	hBar->setPosition(SCENE_WIDTH / 2, HEALTH_BAR_Y_OFFSET);
	healthCanvas->addChild(hBar);
	//Hearts
	std::shared_ptr<PolygonNode> playerH = PolygonNode::allocWithTexture(pHeart);
	playerH->setAnchor(Vec2::ANCHOR_CENTER);
	playerH->setScale(1.0);
	playerH->setPosition(SCENE_WIDTH / 2 - HEART_X_OFFSET, HEALTH_BAR_Y_OFFSET);
	healthCanvas->addChild(playerH);
	std::shared_ptr<PolygonNode> oppH = PolygonNode::allocWithTexture(oHeart);
	oppH->setAnchor(Vec2::ANCHOR_CENTER);
	oppH->setScale(1.0);
	oppH->setPosition(SCENE_WIDTH / 2 + HEART_X_OFFSET, HEALTH_BAR_Y_OFFSET);
	healthCanvas->addChild(oppH);
	//Blocks
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<PolygonNode> playerB = PolygonNode::allocWithTexture(pBlock);
		playerB->setAnchor(Vec2::ANCHOR_CENTER);
		playerB->setScale(0.8);
		playerB->setPosition(SCENE_WIDTH / 2 - BAR_DISTANCE/2 - (i*HEALTH_BLOCK_SPACING), HEALTH_BAR_Y_OFFSET);
		healthCanvas->addChild(playerB);
	}
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<PolygonNode> oppB = PolygonNode::allocWithTexture(oBlock);
		oppB->setAnchor(Vec2::ANCHOR_CENTER);
		oppB->setScale(0.8);
		oppB->setPosition(SCENE_WIDTH / 2 + BAR_DISTANCE / 2 + (i*HEALTH_BLOCK_SPACING), HEALTH_BAR_Y_OFFSET);
		healthCanvas->addChild(oppB);
	}
	return true;
}

std::shared_ptr<PolygonNode> SceneBuilder1::buildChicken(std::shared_ptr<Texture> texture, std::shared_ptr<Node> node, int posX, int posY, bool flip) {
	
	std::shared_ptr<PolygonNode> chick = PolygonNode::allocWithTexture(texture);
	chick->setScale(STACK_SCALE); // Magic number to rescale asset
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

				buttons[i]->setPosition(layer->screenToNodeCoords(_input.getCurTouch()) - Vec2(SCENE_WIDTH / 2, 150));
				std::shared_ptr<Texture> infoText;
				special cel = playerGlobe->getHandAt(i).getSpecial();
				switch (cel) {
				case special::BasicFire:
					infoText = infoF;
					break;
				case special::BasicWater:
					infoText = infoW;
					break;
				case special::BasicGrass:
					infoText = infoG;
					break;
				case special::Reaper:
					infoText = infoReaper;
					break;
				case special::Bomb:
					infoText = infoBomb;
					break;
				case special::Mirror:
					infoText = infoMirror;
					break;
				case special::Ninja:
					infoText = infoNinja;
					break;
				case special::PartyFowl:
					infoText = infoParty;
					break;
				case special::Spy:
					infoText = infoSpy;
					break;
				case special::Thicken:
					infoText = infoThick;
					break;
				case special::Witchen:
					infoText = infoWitch;
					break;
				default:
					switch (playerGlobe->getHandAt(i).getElement()) {
					case element::Fire:
						infoText = infoF;
						break;
					case element::Water:
						infoText = infoW;
						break;
					case element::Grass:
						infoText = infoG;
						break;
					}
				}
				std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoText);
				newPoly->setScale(INFO_SCALE);
				newPoly->setAnchor(Vec2::ANCHOR_CENTER);
				newPoly->setPosition(SCENE_WIDTH / 2 + INFO_X_OFFSET, SCENE_HEIGHT / 2 + INFO_Y_OFFSET);
				infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);

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
		case special::Witchen:
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
		newUp->flipHorizontal(true);
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
		case special::Witchen:
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
		case special::Witchen:
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

	//Update the info card
	if (heldButton == nullptr) {
		infoCanvas->setVisible(false);
	}
	else {
		infoCanvas->setVisible(true);
	}

	//Update the opponent health bar
    for (int i = 1; i < 6; i++) {
        if (oppGlobe->getHealth() < i) {
            std::shared_ptr<Node> child = healthCanvas->getChild(i + 7);
            child->setVisible(false);
        }
    }

	//Update the player health bar
	for (int i = 1; i < 6; i++) {
		if (playerGlobe->getHealth() < i) {
			std::shared_ptr<Node> child = healthCanvas->getChild(i + 2);
			child->setVisible(false);
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
