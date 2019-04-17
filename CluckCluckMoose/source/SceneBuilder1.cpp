//Scene builder file

/*  Input scheme manipulation:
For drag, uncomment line 477 and change the conditional on 353 to check whether the chicken is in the bounding box to play (butt->getPosition().x < <tunable int a> && butt->getPosition().y > <tunable int b>)
For touch and hold, uncomment lines 347-349, 788-790, and 812-826 and change the conditional to check if the timer for the button >0 but less than the threshold (timers[i] < 30 && timers[i] > 1)
*/

//Import important files
#include "Moose.h"
#include "SceneBuilder1.h"


using namespace cugl;

/** Sfx for the game*/
#define CHICKEN_SCREECH		"screech"

std::shared_ptr<AssetManager> _assets;

//Button list for player hand
std::vector<std::shared_ptr<Button>> buttons;
std::vector<std::shared_ptr<AnimationNode>> buttonTextures;

//Texture list for determining which textures need to be replaced in the hand
std::vector<std::shared_ptr<Texture>> texturesHand;

//List of player stack nodes
std::vector<std::shared_ptr<AnimationNode>> pstackNodes;
//Texture list for determining which textures need to be replaced in the player stack
std::vector<std::shared_ptr<Texture>> texturesPStack;

//List of opponent stack nodes
std::vector<std::shared_ptr<AnimationNode>> ostackNodes;
//Texture list for determining which textures need to be replaced in the opponent stack
std::vector<std::shared_ptr<Texture>> texturesOStack;

//Track held chicken
std::shared_ptr<Button> heldButton;

//Track previous chicken distribution
std::vector<int> prevDist;

//Track scale for drawing UI elements
float healthYScale;



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
#define HEALTH_BAR_Y_FACTOR 12
#define HEALTH_BLOCK_SPACING 19
#define HEART_X_OFFSET 180
#define BAR_DISTANCE 120
#define CHICKEN_FILMSTRIP_LENGTH 8
#define HEART_SCALE 0.3
#define BLOCK_X_SCALE 0.29
#define BLOCK_Y_SCALE 0.2
#define HBAR_SCALE 0.5
#define ELT_Y_OFFSET 75
#define ELT_INFO_SCALE 0.7
#define ELT_INFO_X_OFFSET 30
#define ELT_NUM_X_OFFSET 40
#define ELT_NUM_Y_OFFSET 5
#define ELT_NUM_SCALE 0.7
#define ELT_NUM_SPACING 50

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

//Number textures
std::shared_ptr<Texture> num0;
std::shared_ptr<Texture> num1;
std::shared_ptr<Texture> num2;
std::shared_ptr<Texture> num3;
std::shared_ptr<Texture> num4;



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

//Canvas for elemental info
std::shared_ptr<Node> eltInfoCanvas;

//Players
std::shared_ptr<Moose> playerGlobe;
std::shared_ptr<Moose> oppGlobe;

//Preview tracking
bool previewSet;
//values for animation
int  thisFrame = 0;
float timeAmount = 0;
float timeBtnFrames = 0.1;
//std::vector<std::shared_ptr<int>> flappingFrame;
std::vector<int> flappingFrame;

//Input timer to determine if the player wants info or wants to play a chicken
std::vector<int> timers;
int heldButtInd;


bool SceneBuilder1::init(const std::shared_ptr<cugl::AssetManager>& assets, const Size dimen, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp) {

	root->removeAllChildren();

	playerGlobe = player;
	oppGlobe = opp;

	heldButton = nullptr;
	for (int i = 0; i < 6; i++) {
		timers.push_back(0);
	}
	heldButtInd = -1;

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

	//init flapping booleans to 0
	for(int i =0; i< 6; i++){
        int f = 0;
	    flappingFrame.push_back(f);
	}


	//Init stack nodes
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<Texture> text;
    
		/* text = textureF;
		std::shared_ptr<PolygonNode> poly;
		poly = buildChicken(text, layer, STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING), true);
        poly->setVisible(false);
		pstackNodes.push_back(poly); */
    
		text = textureW;
		//this is std library for c++
		std::shared_ptr<AnimationNode> anim;
		//get changed to animation nodes EMMMAAA
		anim = buildChicken(text, layer, STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING), true);
		anim->setVisible(false);
		pstackNodes.push_back(anim);
		texturesPStack.push_back(text);
	}

	//origin is bottom left


	//init enemy chicken nodes
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<Texture> text;
    
    /*text = textureF;
		std::shared_ptr<PolygonNode> poly;*/
  
		text = textureW;
		std::shared_ptr<AnimationNode> poly;
    
		poly = buildChicken(text, layer, SCENE_WIDTH - STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING), false);
        poly->setVisible(false);
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

	//Add elt info canvas
	eltInfoCanvas = Node::alloc();
	layer->addChild(eltInfoCanvas);

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
		text = textureW;

		std::shared_ptr<AnimationNode> id = AnimationNode::alloc(text,1,CHICKEN_FILMSTRIP_LENGTH,CHICKEN_FILMSTRIP_LENGTH);
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
				if (timers[i] > 30) {
					infoCanvas->setVisible(true);
				}

			}
			if (!down) {
				if (timers[i] < 30 && timers[i] > 1) {
					playerGlobe->addToStackFromHand(i);

					//Play chicken cluck sfx
					auto source = _assets->get<Sound>(CHICKEN_SCREECH);
					if (!AudioChannels::get()->isActiveEffect(CHICKEN_SCREECH)) {
						AudioChannels::get()->playEffect(CHICKEN_SCREECH, source, false, source->getVolume());
					}
				}
				heldButton = nullptr;
				heldButtInd = -1;
				/*if (i < 3) {
					butt->setPosition((i - 1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET);
				}
				else {
					butt->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING);
				} */
			}
		});

		buttonCanvas->addChild(butt);
		//i+2 to ensure keys are unique
		butt->activate(i + 2);
		//CULog("Button %d made", i);
		buttons.push_back(butt);
		buttonTextures.push_back(id);
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
	//Scale factor * scene height makes the health bar appear in consistent locations, independent of device
	healthYScale = (((float)(HEALTH_BAR_Y_FACTOR - 1)) / ((float)HEALTH_BAR_Y_FACTOR)) * SCENE_HEIGHT;
	CULog("%d", healthYScale);
	//Bar
	std::shared_ptr<PolygonNode> hBar = PolygonNode::allocWithTexture(bar);
	hBar->setAnchor(Vec2::ANCHOR_CENTER);
	hBar->setScale(HBAR_SCALE);
	hBar->setPosition(SCENE_WIDTH / 2, healthYScale);
	healthCanvas->addChild(hBar);
	//Hearts
	std::shared_ptr<PolygonNode> playerH = PolygonNode::allocWithTexture(pHeart);
	playerH->setAnchor(Vec2::ANCHOR_CENTER);
	playerH->setScale(HEART_SCALE);
	playerH->setPosition(SCENE_WIDTH / 2 - HEART_X_OFFSET, healthYScale);
	healthCanvas->addChild(playerH);
	std::shared_ptr<PolygonNode> oppH = PolygonNode::allocWithTexture(oHeart);
	oppH->setAnchor(Vec2::ANCHOR_CENTER);
	oppH->setScale(HEART_SCALE);
	oppH->setPosition(SCENE_WIDTH / 2 + HEART_X_OFFSET, healthYScale);
	healthCanvas->addChild(oppH);
	//Blocks
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<PolygonNode> playerB = PolygonNode::allocWithTexture(pBlock);
		playerB->setAnchor(Vec2::ANCHOR_CENTER);
		playerB->setScale(BLOCK_X_SCALE, BLOCK_Y_SCALE);
		playerB->setPosition(SCENE_WIDTH / 2 - BAR_DISTANCE/2 - (i*HEALTH_BLOCK_SPACING), healthYScale);
		healthCanvas->addChild(playerB);
	}
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<PolygonNode> oppB = PolygonNode::allocWithTexture(oBlock);
		oppB->setAnchor(Vec2::ANCHOR_CENTER);
		oppB->setScale(BLOCK_X_SCALE, BLOCK_Y_SCALE);
		oppB->setPosition(SCENE_WIDTH / 2 + BAR_DISTANCE / 2 + (i*HEALTH_BLOCK_SPACING), healthYScale);
		healthCanvas->addChild(oppB);
	}
	
	//Add elemental information
	std::shared_ptr<Texture> eltInfoText = _assets->get<Texture>("groupedElts");
	std::shared_ptr<PolygonNode> eltInfo = PolygonNode::allocWithTexture(eltInfoText);
	eltInfo->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	eltInfo->setScale(ELT_INFO_SCALE);
	eltInfo->setPosition(SCENE_WIDTH / 2 - ELT_INFO_X_OFFSET, healthYScale - ELT_Y_OFFSET);
	eltInfoCanvas->addChild(eltInfo);

	//Add numeric values (also alloc textures here)
	num0 = _assets->get<Texture>("num0");
	num1 = _assets->get<Texture>("num1");
	num2 = _assets->get<Texture>("num2");
	num3 = _assets->get<Texture>("num3");
	num4 = _assets->get<Texture>("num4");
	std::shared_ptr<PolygonNode> fireNum = PolygonNode::allocWithTexture(num1);
	std::shared_ptr<PolygonNode> waterNum = PolygonNode::allocWithTexture(num1);
	std::shared_ptr<PolygonNode> grassNum = PolygonNode::allocWithTexture(num1);
	fireNum->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	fireNum->setScale(ELT_NUM_SCALE);
	fireNum->setPosition(SCENE_WIDTH / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET);
	eltInfoCanvas->addChild(fireNum);
	waterNum->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	waterNum->setScale(ELT_NUM_SCALE);
	waterNum->setPosition(SCENE_WIDTH / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET - ELT_NUM_SPACING);
	eltInfoCanvas->addChild(waterNum);
	grassNum->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	grassNum->setScale(ELT_NUM_SCALE);
	grassNum->setPosition(SCENE_WIDTH / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET - (2*ELT_NUM_SPACING));
	eltInfoCanvas->addChild(grassNum);

	//Initialize distribution
	prevDist.push_back(1);
	prevDist.push_back(1);
	prevDist.push_back(1);

	return true;
}

std::shared_ptr<AnimationNode> SceneBuilder1::buildChicken(std::shared_ptr<Texture> texture, std::shared_ptr<Node> node, int posX, int posY, bool flip) {

	std::shared_ptr<AnimationNode> chick = AnimationNode::alloc(texture,1,CHICKEN_FILMSTRIP_LENGTH);
	chick->setScale(STACK_SCALE); // Magic number to rescale asset
	chick->setAnchor(Vec2::ANCHOR_CENTER);
	chick->setPosition(posX, posY);
	chick->flipHorizontal(flip);
	node->addChild(chick);
	return chick;
}


void SceneBuilder1::updateGameScene(float timestep) {
	timeAmount +=timestep;
	bool isNextFrame = (timeAmount > timeBtnFrames);
	if(timeAmount > timeBtnFrames)
	{
	    isNextFrame = true;
		timeAmount = 0;
		if(thisFrame>=CHICKEN_FILMSTRIP_LENGTH -1)
			thisFrame = 0;
		thisFrame++;
	}


	vector <Chicken> hand = playerGlobe->getHand();


	for (int i = 0; i < 6; i++) {
		if (i < hand.size()) {
			buttons[i]->setVisible(true);
			buttons[i]->activate(i + 2);
			if (buttons[i] == heldButton) {
				heldButtInd = i;
				//buttons[i]->setPosition(layer->screenToNodeCoords(_input.getCurTouch()) - Vec2(SCENE_WIDTH / 2, 150));
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

		std::shared_ptr<AnimationNode> newUp = AnimationNode::alloc(text,1,CHICKEN_FILMSTRIP_LENGTH,CHICKEN_FILMSTRIP_LENGTH);
		//animates bottom chickens in coop
		int curFrame = (flappingFrame[i]);
//        int curFrame = 0;
        int nextFrame = curFrame + 1;

        //decides if a flapping animation should start
		if(isNextFrame && (curFrame!=0|| std::rand()%50==0)){

		    if(nextFrame >= CHICKEN_FILMSTRIP_LENGTH -1) {
		        nextFrame = 0;

            }
            flappingFrame[i] = nextFrame;
            newUp->setFrame(nextFrame);
		}
		else{
		    newUp->setFrame(nextFrame-1);

		}

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

		}
        std::shared_ptr<AnimationNode> chick = AnimationNode::alloc(text,1,CHICKEN_FILMSTRIP_LENGTH);
        chick->setScale(STACK_SCALE); // Magic number to rescale asset
        chick->setAnchor(Vec2::ANCHOR_CENTER);
        chick->setPosition(pstackNodes[i]->getPositionX(), pstackNodes[i]->getPositionY());
        chick->flipHorizontal(true);

        layer->swapChild(pstackNodes[i], chick, false);
        pstackNodes[i] = chick;

        texturesPStack[i] = text;

        pstackNodes[i]->setFrame(thisFrame);

	}

	Stack ostack = oppGlobe->getStack();

	for (int i = 0; i < 5; i++) {
		if (i < ostack.getSize()) {
			ostackNodes[i]->setVisible(true);
//            ostackNodes[i]->setFrame(thisFrame);
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
//			ostackNodes[i]->setTexture(text);
//			texturesOStack[i] = text;
		}
        std::shared_ptr<AnimationNode> chick = AnimationNode::alloc(text,1,CHICKEN_FILMSTRIP_LENGTH);
        chick->setScale(STACK_SCALE); // Magic number to rescale asset
        chick->setAnchor(Vec2::ANCHOR_CENTER);
        chick->setPosition(ostackNodes[i]->getPositionX(), ostackNodes[i]->getPositionY());
        chick->flipHorizontal(false);

        layer->swapChild(ostackNodes[i], chick, false);
        ostackNodes[i] = chick;

        texturesPStack[i] = text;

        ostackNodes[i]->setFrame(thisFrame);
	}

	//Update the info card
	if (heldButton == nullptr) {
		infoCanvas->setVisible(false);
	}
	else {
		if (timers[heldButtInd] > 30) {
			infoCanvas->setVisible(true);
		}
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

	//Update the opponent type distribution
	for (int i = 0; i < 3; i++) {
		int currDistI = (oppGlobe->getChickenElementDistribution())[i];
		int prevDistI = (prevDist[i]);
		if (currDistI != prevDistI) {
			std::shared_ptr<Texture> text;
			if (currDistI == 0) {
				text = num0;
			}
			else if (currDistI == 1) {
				text = num1;
			}
			else if (currDistI == 2) {
				text = num2;
			}
			else if (currDistI == 3) {
				text = num3;
			}
			else if (currDistI == 4) {
				text = num4;
			}
			prevDist[i] = currDistI;
			
				std::shared_ptr<PolygonNode> swapPoly = PolygonNode::allocWithTexture(text);
				swapPoly->setAnchor(Vec2::ANCHOR_TOP_CENTER);
				swapPoly->setScale(ELT_NUM_SCALE);
				swapPoly->setPosition(SCENE_WIDTH / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET - (i*ELT_NUM_SPACING));
				swapPoly->setVisible(true);
				std::shared_ptr<Node> oldChild = eltInfoCanvas->getChild(i + 1);
				eltInfoCanvas->swapChild(oldChild, swapPoly, false);
		}
	}
}

void SceneBuilder1::updateInput(float timestep) {
	_input.update(timestep);
	if (heldButtInd >= 0) {
		for (int i = 0; i < 6; i++) {
			if (i == heldButtInd) {
				timers[i]++;
			}
			else {
				timers[i] = 0;
			}
		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			timers[i] = 0;
		}
	}

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

void SceneBuilder1::deactivateHand() {
	vector <Chicken> hand = playerGlobe->getHand();

	for (int i = 0; i < 6; i++) {
		if (i < hand.size()) {
			buttons[i]->setVisible(false);
			buttons[i]->deactivate();
		}
	}
}

void SceneBuilder1::activateHand() {
	vector <Chicken> hand = playerGlobe->getHand();

	for (int i = 0; i < 6; i++) {
		if (i < hand.size()) {
			buttons[i]->setVisible(true);
			buttons[i]->activate(i + 2);
		}
	}
}