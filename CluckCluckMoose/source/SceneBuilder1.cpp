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


//Track Stack chicken held down
int sInfoInd;

//Track which buttons map to which cards in player hand. card x at index i is the xth card in the opponent's hand, shown by button i 
std::vector<int> handMap;

//Track previous chicken distribution
std::vector<int> prevDist;

//Track scale for drawing UI elements
float healthYScale;

//Button list for pause menu
std::vector<std::shared_ptr<Button>> pausebuttons;

//Button list for win menu
std::vector<std::shared_ptr<Button>> winbuttons;

//Button list for lose menu
std::vector<std::shared_ptr<Button>> losebuttons;

//List of stamp nodes for player and opponent
std::vector<std::shared_ptr<PolygonNode>> pStamps;
std::vector<std::shared_ptr<PolygonNode>> oStamps;

//Control variables for menu navigation
bool isPaused;
bool nextLevel;
bool goHome;
bool retry;
bool soundToggle;
bool soundChanged;
bool hasWon;
bool hasLost;


/** The ID for the button listener */
#define LISTENER_ID 2
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 576
#define SCENE_HEIGHT 1024

//Drawing constants
#define MOOSE_HEIGHT 300
#define FORE_HEIGHT 125
#define MOOSE_X_OFFSET 25
#define HAND_SCALE 0.5f
#define STACK_SCALE 0.5f
#define BUTTON_X_SPACING 175
#define BUTTON_X_OFFSET 0
#define BUTTON_Y_SPACING 90
#define BUTTON_Y_OFFSET -55
#define STACK_X_OFFSET 100
#define STACK_Y_OFFSET 550
#define STACK_Y_SPACING 72
#define INFO_X_OFFSET 7
#define INFO_Y_OFFSET 100
#define INFO_SCALE 0.5
#define HEALTH_BAR_Y_FACTOR 12
#define HEALTH_BLOCK_SPACING 20
#define HEART_X_OFFSET 180
#define BAR_DISTANCE 90
#define CHICKEN_FILMSTRIP_LENGTH 8
#define SIGN_FILMSTRIP_LENGTH 28
#define HEART_SCALE 0.4
#define BLOCK_X_SCALE 0.3
#define BLOCK_Y_SCALE 0.3
#define HBAR_SCALE 0.25
#define ELT_Y_OFFSET 75
#define ELT_INFO_SCALE 0.7
#define ELT_INFO_X_OFFSET 30
#define ELT_NUM_X_OFFSET 40
#define ELT_NUM_Y_OFFSET 5
#define ELT_NUM_SCALE 0.7
#define ELT_NUM_SPACING 50
#define SHADOW_SCALE 0.6
#define SHADOW_OFFSET 50
#define STAMP_X_OFFSET 5
#define STAMP_Y_OFFSET -10
#define STAMP_SCALE 0.1
#define WIN_LOSS_SCALE 0.5
#define WIN_LOSS_Y_OFFSET 100
#define WIN_LOSS_B_SCALE 0.5
#define WIN_LOSS_B_Y_OFFSET -225
#define WIN_BUTTON_X_SPACING 140
#define LOSS_BUTTON_X_SPACING 175
#define CHICKEN_SHOT_ROWS 1
#define CHICKEN_SHOT_COLS 1
#define STILL_SIGN_TIME 1.5
#define DEATH_ANIM_COLS 8
#define INFO_DELAY 15
#define MOOSE_SCALE 0.5f
#define BULLET_SPEED 100
#define MOOSE_DAMAGE_TIME 8 //number of frames a moose shows its damage

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
std::shared_ptr<Texture> numMinus;

//Shadow Texture
std::shared_ptr<Texture> shadow;

//Type stamp textures
std::shared_ptr<Texture> fstamp;
std::shared_ptr<Texture> gstamp;
std::shared_ptr<Texture> wstamp;

//Win/loss screen textures
std::shared_ptr<Texture> wlOverlay;
std::shared_ptr<Texture> victory;
std::shared_ptr<Texture> homeButton;
std::shared_ptr<Texture> defeat;
std::shared_ptr<Texture> redo;
std::shared_ptr<Texture> nextlvl;

//transition texture
std::shared_ptr<Texture> smokeTrans;

//projectile texture
std::shared_ptr<Texture> fireShot;
std::shared_ptr<Texture> waterShot;
std::shared_ptr<Texture> grassShot;

//chicken death textures
std::shared_ptr<Texture> waterTrans;
std::shared_ptr<Texture> grassTrans;
std::shared_ptr<Texture> fireTrans;


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

//Canvas for stack info
std::shared_ptr<Node> stackInfoCanvas;

//Canvas for buttons
std::shared_ptr<Node> buttonCanvas;

//Canvas for clash button
std::shared_ptr<Node> clashButtonCanvas;

//Canvas for health bar
std::shared_ptr<Node> healthCanvas;

//Canvas for elemental info
std::shared_ptr<Node> eltInfoCanvas;

//Win screen canvas
std::shared_ptr<Node> winCanvas;

//Win screen canvas
std::shared_ptr<Node> loseCanvas;

//Canvas for pause button
std::shared_ptr<Node> pauseButtonCanvas;

//Canvas for misc. UI (i.e. pause)
std::shared_ptr<Node> pauseMenuCanvas;

std::shared_ptr<Node> clashSignCanvas;


//Preview tracking
bool previewSet;
//Tint tracking
bool prevTint;

//Frame tracking for flapping animations
int  thisFrame = 0;
float timeAmount = 0;
float timeBtnFrames = 0.1;
//keeps track of which frame chicken flapping is on in the hand
std::vector<int> flappingFrame;

bool oLose = false; //true if opponent moose takes damage
bool pLose = false; //true if player moose takes damage
int damageTaken = 0;
int tintedTime = 0; //timer for how long a moose stays tinted



//Frame tracking for attacking animations

//the world position of the middle of the screen
int middleScreen = 0;
//the frame that a chicken shot is currently at, -1 if no shot on screen
int pShotProgress = -1;
int eShotProgress = -1;

//0 index is current player death animation frame, 1 is opponent
std::vector<int> dyingFrame;
std::vector<int> pSmokeFrame;
std::vector<int> eSmokeFrame;
//-1 means player loss, 0 means tie, 1 means player win
int playerChickenWins;
//element of player chicken
element pType;
//element of enemy chicken
element eType;


//Input timer to determine if the player wants info or wants to play a chicken
std::vector<int> timers;
int heldButtInd;

int signframe = 1;
int signCount = STILL_SIGN_TIME;
bool signDone = false;
std::shared_ptr<Texture> textsign;
std::shared_ptr<AnimationNode> sign;

//Screen dimensions
float screenHeight;
float screenWidth;


bool SceneBuilder1::init(const std::shared_ptr<cugl::AssetManager>& assets, const Size dimen, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, string costume, int levelNum) {

	root->removeAllChildren();

	//Set screen size
	screenHeight = dimen.height;
	screenWidth = dimen.width;

	middleScreen = screenWidth / 2;

	playerGlobe = player;
	oppGlobe = opp;

	isTutor = tutor;
	if (tutor) {
		step = 0;
	}
	else {
		step = -1;
	}

    isPaused = false;
	nextLevel = false;
	goHome = false;
	retry = false;
	hasWon = false;
	hasLost = false;

	prevTint = false;

	heldButton = nullptr;
	sInfoInd = -1;

	for (int i = 0; i < 6; i++) {
		timers.push_back(0);
	}
	heldButtInd = -1;


	for (int i = 0; i < 6; i++) {
		handMap.push_back(i);
	}
	for (int i = 0; i < 6; i++) {
		revHandMap.push_back(i);
	}
	retry = false;
	goHome = false;
	soundToggle = false;
    soundChanged = false;

	_assets = assets;
	_input.init();

	//Get tutorial textures.
	tutor1 = _assets->get<Texture>("tutor_1");
	tutor2 = _assets->get<Texture>("tutor_2");
	tutor3 = _assets->get<Texture>("tutor_3");
	tutor4 = _assets->get<Texture>("tutor_4");
	tutor5 = _assets->get<Texture>("tutor_5");
	tutor6 = _assets->get<Texture>("tutor_6");
	tutor7 = _assets->get<Texture>("tutor_7");
	spylight = _assets->get<Texture>("tutor_spy");
	grasslight = _assets->get<Texture>("tutor_grass");

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

	smokeTrans = _assets->get<Texture>("smokeTrans");

	//Get health textures
	bar = _assets->get<Texture>("healthBar");
	pHeart = _assets->get<Texture>("pHeart");
	oHeart = _assets->get<Texture>("oHeart");
	pBlock = _assets->get<Texture>("pBlock");
	oBlock = _assets->get<Texture>("oBlock");

	//get Shadow Texture
	shadow = _assets->get<Texture>("shadow");

	//get Stamp textures
	fstamp = _assets->get<Texture>("fireLabel");
	gstamp = _assets->get<Texture>("grassLabel");
	wstamp = _assets->get<Texture>("waterLabel");

	//get win/loss screen textures
	victory = _assets->get<Texture>("victoryScreen");
	wlOverlay = _assets->get<Texture>("wlOverlay");
	homeButton = _assets->get<Texture>("homeButton");
	redo = _assets->get<Texture>("redo");
	nextlvl = _assets->get<Texture>("nextLvl");
	defeat = _assets->get<Texture>("defeatScreen");

	//get chicken death textures
	smokeTrans = _assets->get<Texture>("smokeTrans");
	waterTrans = _assets->get<Texture>("waterTrans");
	fireTrans = _assets->get<Texture>("fireTrans");
	grassTrans = _assets->get<Texture>("grassTrans");

	waterShot = _assets->get<Texture>("projectiles");
	fireShot = _assets->get<Texture>("projectiles");
	grassShot = _assets->get<Texture>("projectiles");

	layer = assets->get<Node>("game");
	layer->setContentSize(dimen);
	layer->doLayout(); // This rearranges the children to fit the screen
	layer->removeFromParent();
	root->addChild(layer);

	//Create background node
	backCanvas = Node::alloc();
	layer->addChild(backCanvas);

	//init flapping ints to 0
	for(int i =0; i< 6; i++){
        int f = 0;
	    flappingFrame.push_back(f);
	}

	//init dying frames to -1
	for (int i = 0; i < 2; i++) {
		int f = -1;
		dyingFrame.push_back(f);
	}

	for (int i = 0; i < 5; i++) {
		int f = -1;
		eSmokeFrame.push_back(f);
		pSmokeFrame.push_back(f);
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
		anim = buildChicken(text, layer, STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING), true);
		anim->setVisible(false);
		pstackNodes.push_back(anim);
		texturesPStack.push_back(text);

		


		//Init type stamp nodes
		std::shared_ptr<PolygonNode> stamp = PolygonNode::allocWithTexture(fstamp);
		stamp->setAnchor(Vec2::ANCHOR_CENTER);
		stamp->setPosition(STACK_X_OFFSET + STAMP_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING) + STAMP_Y_OFFSET);
		stamp->setScale(STAMP_SCALE);
		stamp->setVisible(false);
		pStamps.push_back(stamp);
		layer->addChild(stamp);
	}

	//Placeholder Text for stack buttons
	std::shared_ptr<Texture> textPh = _assets->get<Texture>("firePh");
	
	//Init player stack info buttons
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<PolygonNode> sbNode = PolygonNode::allocWithTexture(textPh);
		sbNode->setAnchor(Vec2::ANCHOR_CENTER);
		sbNode->flipHorizontal(true);
		std::shared_ptr<Button> stackButton = Button::alloc(sbNode);
		stackButton->setAnchor(Vec2::ANCHOR_CENTER);
		stackButton->setScale(HAND_SCALE, 0.3);
		stackButton->setPosition(STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING));
		stackButton->setListener([=](const std::string& name, bool down) {
			if (down) {
				if (i < playerGlobe->getStack().getSize() && sInfoInd == -1) {
					sInfoInd = i;
				}
			}
			if (!down) {
				sInfoInd = -1;
			}
		});
		stackButton->activate(i+8);
		stackButton->setVisible(false);
		layer->addChild(stackButton);
	}

	//origin is bottom left


	//init enemy chicken nodes
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<Texture> text;

    /*text = textureF;
		std::shared_ptr<PolygonNode> poly;*/

		text = textureW;
		std::shared_ptr<AnimationNode> poly;
    
		poly = buildChicken(text, layer, screenWidth - STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING), false);

        poly->setVisible(false);
		ostackNodes.push_back(poly);
		texturesOStack.push_back(text);

		//Init type stamp nodes
		std::shared_ptr<PolygonNode> stamp = PolygonNode::allocWithTexture(fstamp);
		stamp->setAnchor(Vec2::ANCHOR_CENTER);
		stamp->setPosition(screenWidth - STACK_X_OFFSET - STAMP_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING) + STAMP_Y_OFFSET);
		stamp->setScale(STAMP_SCALE);
		stamp->setVisible(false);
		oStamps.push_back(stamp);
		layer->addChild(stamp);
	}

	//Init opponent stack info buttons
	for (int i = 0; i < 5; i++) {
		std::shared_ptr<PolygonNode> sbNode = PolygonNode::allocWithTexture(textPh);
		sbNode->setAnchor(Vec2::ANCHOR_CENTER);
		sbNode->flipHorizontal(false);
		std::shared_ptr<Button> stackButton = Button::alloc(sbNode);
		stackButton->setAnchor(Vec2::ANCHOR_CENTER);
		stackButton->setScale(HAND_SCALE, 0.3);
		stackButton->setPosition(screenWidth - STACK_X_OFFSET, STACK_Y_OFFSET + (i*STACK_Y_SPACING));
		stackButton->setListener([=](const std::string& name, bool down) {
			if (down) {
				if (i < oppGlobe->getStack().getSize() && sInfoInd == -1) {
					sInfoInd = i+5;
				}
			}
			if (!down) {
				sInfoInd = -1;
			}
		});
		stackButton->activate(i + 13);
		stackButton->setVisible(false);
		layer->addChild(stackButton);
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
	clashButtonCanvas->setPosition(screenWidth / 2, 150);

	//Add elt info canvas
	eltInfoCanvas = Node::alloc();
	layer->addChild(eltInfoCanvas);

	//Create a stack info node
	stackInfoCanvas = Node::alloc();
	layer->addChild(stackInfoCanvas);
	
	//Create an info node
	infoCanvas = Node::alloc();
	layer->addChild(infoCanvas);


	//Add button canvas
	buttonCanvas = Node::alloc();
	layer->addChild(buttonCanvas);
	buttonCanvas->setPosition(screenWidth / 2, 150);
    
    //Add clash sign canvas
    clashSignCanvas = Node::alloc();
    layer->addChild(clashSignCanvas);

	//Add health canvas
	healthCanvas = Node::alloc();
	layer->addChild(healthCanvas);


	//Add pause button canvas
	pauseButtonCanvas = Node::alloc();
	layer->addChild(pauseButtonCanvas);

    //Add pause menu canvas
    pauseMenuCanvas = Node::alloc();
    layer->addChild(pauseMenuCanvas);

	//Add win/loss canvases
	winCanvas = Node::alloc();
	layer->addChild(winCanvas);
	loseCanvas = Node::alloc();
	layer->addChild(loseCanvas);


	//Add tutorialCanvas
	tutorialCanvas = Node::alloc();
	layer->addChild(tutorialCanvas);

	//reset drawing between frames
	backCanvas->removeAllChildren();
	mooseCanvas->removeAllChildren();
	frontCanvas->removeAllChildren();
	buttonCanvas->removeAllChildren();

	//Draw background
	std::shared_ptr<Texture> texturebg;
	if (levelNum < 4) {
		texturebg = _assets->get<Texture>("farmbg");
	}
	else if (levelNum < 7) {
		texturebg = _assets->get<Texture>("forestbg");
	}
	else if (levelNum < 10) {
		texturebg = _assets->get<Texture>("nuclearbg");
	}
	else {
		texturebg = _assets->get<Texture>("thronebg");
	}
	std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
	background->setScale(0.65f); // Magic number to rescale asset
	background->setAnchor(Vec2::ANCHOR_CENTER);
	background->setPosition(screenWidth/2, screenHeight/2);
	backCanvas->addChild(background);



	//Draw player moose
	
	std::shared_ptr<Texture> textureP = _assets->get<Texture>("moose");
	std::shared_ptr<PolygonNode> moose1 = PolygonNode::allocWithTexture(textureP);
	moose1->setScale(MOOSE_SCALE); // Magic number to rescale asset
	moose1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	moose1->setPosition(-MOOSE_X_OFFSET, MOOSE_HEIGHT);
	moose1->flipHorizontal(false);
	mooseCanvas->addChildWithName(moose1, "player_moose");

	//Draw opponent moose
	std::shared_ptr<PolygonNode> moose2;
	std::shared_ptr<Texture> textureM;
	
	if (opp->getCostume() == "basic_moose") {
		textureM = _assets->get<Texture>("moose");
	}
	else if (opp->getCostume() == "eldrich_moose") {
		textureM = _assets->get<Texture>("elMoose");
	}
	else if (opp->getCostume() == "branchy") {
		textureM = _assets->get<Texture>("branchy");
	}
	else if (opp->getCostume() == "caffeine_moose") {
		textureM = _assets->get<Texture>("cafMoose");
	}
	else if (opp->getCostume() == "emerald_moose") {
		textureM = _assets->get<Texture>("emMoose");
	}
	else if (opp->getCostume() == "elder_moose") {
		textureM = _assets->get<Texture>("oldMoose");
	}
	else if (opp->getCostume() == "moose_majesty") {
		textureM = _assets->get<Texture>("kingMoose");
	}
	else if (opp->getCostume() == "moosesassin") {
		textureM = _assets->get<Texture>("rogMoose");
	}
	else if (opp->getCostume() == "nebula_moose") {
		textureM = _assets->get<Texture>("cosMoose");
	}
	else if (opp->getCostume() == "tentamoose") {
		textureM = _assets->get<Texture>("tenMoose");
	}
	else if (opp->getCostume() == "moosewave") {
		textureM = _assets->get<Texture>("vapMoose");
	}
	else if (opp->getCostume() == "chrismoose") {
		textureM = _assets->get<Texture>("chrMoose");
	}
	else if (opp->getCostume() == "farmer_moose") {
		textureM = _assets->get<Texture>("farmMoose");
	}
	else {
		textureM = _assets->get<Texture>("moose");
	}

	moose2 = PolygonNode::allocWithTexture(textureM);
	moose2->setScale(MOOSE_SCALE); // Magic number to rescale asset
	moose2->setAnchor(Vec2::ANCHOR_BOTTOM_RIGHT);
	moose2->setPosition(screenWidth + MOOSE_X_OFFSET, MOOSE_HEIGHT);
	moose2->flipHorizontal(true);
	mooseCanvas->addChildWithName(moose2, "opp_moose");

	//Draw foreground
	std::shared_ptr<Texture> texturefg;
	if (levelNum < 4) {
		texturefg = _assets->get<Texture>("farmfg");
	}
	else if (levelNum < 7) {
		texturefg = _assets->get<Texture>("forestfg");
	}
	else if (levelNum < 10) {
		texturefg = _assets->get<Texture>("nuclearfg");
	}
	else {
		texturefg = _assets->get<Texture>("thronefg");
	}
	std::shared_ptr<PolygonNode> foreground = PolygonNode::allocWithTexture(texturefg);
	foreground->setScale(0.65f); // Magic number to rescale asset
	foreground->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
	foreground->setPosition(screenWidth/2, FORE_HEIGHT);
	frontCanvas->addChild(foreground);

	//Draw stack info
	std::shared_ptr<PolygonNode> sInfo = PolygonNode::allocWithTexture(infoF);
	sInfo->setScale(INFO_SCALE);
	sInfo->setAnchor(Vec2::ANCHOR_CENTER);
	sInfo->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight/2 + INFO_Y_OFFSET);
	stackInfoCanvas->addChild(sInfo);
	stackInfoCanvas->setVisible(false);

	//Draw info
	std::shared_ptr<PolygonNode> info = PolygonNode::allocWithTexture(infoF);
	info->setScale(INFO_SCALE);
	info->setAnchor(Vec2::ANCHOR_CENTER);
	info->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
	infoCanvas->addChild(info);
	infoCanvas->setVisible(false);

	//Draw shadows
	for (int i = 0; i < 6; i++) {
		std::shared_ptr<PolygonNode> shadowNode = PolygonNode::allocWithTexture(shadow);
		shadowNode->setAnchor(Vec2::ANCHOR_CENTER);
		shadowNode->setScale(SHADOW_SCALE);
		shadowNode->setVisible(false);

		if (i < 3) {
			shadowNode->setPosition((i - 1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET - SHADOW_OFFSET);
		}
		else {
			shadowNode->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING - SHADOW_OFFSET);
		}
		buttonCanvas->addChild(shadowNode);
	}

	//Init Tutorial glows
	grasslightcanvas = AnimationNode::alloc(grasslight, 1, GRASS_FILMSTRIP_LENGTH, GRASS_FILMSTRIP_LENGTH);
	grasslightcanvas->setAnchor(Vec2::ANCHOR_CENTER);
	grasslightcanvas->setPosition(BUTTON_X_OFFSET, BUTTON_Y_OFFSET - GRASSLIGHT_OFFSET);
	grasslightcanvas->setScale(HAND_SCALE, HAND_SCALE);
	buttonCanvas->addChild(grasslightcanvas); //child 0
	grasslightcanvas->setVisible(isTutor);

	spylightcanvas = AnimationNode::alloc(spylight, 1, SPY_FILMSTRIP_LENGTH, SPY_FILMSTRIP_LENGTH);
	spylightcanvas->setAnchor(Vec2::ANCHOR_CENTER);
	spylightcanvas->setPosition(BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING - SPYLIGHT_OFFSET);
	spylightcanvas->setScale(HAND_SCALE, HAND_SCALE);
	buttonCanvas->addChild(spylightcanvas); //child 1
	spylightcanvas->setVisible(false);

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
			butt->setPosition((i - 1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET);
		}
		else {
			butt->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING);
		}

		if (_input.isActive()) {
			//CULog("active");
		}

		butt->setListener([=](const std::string& name, bool down) {
			if (isTutor) {
				if (i == 1 || i == 4) { //grass or spy chicken for tutorial
					if (down) {
						heldButton = buttons[i];
						if (timers[i] > 15) {
							infoCanvas->setVisible(true);
						}
					}
					if (!down) {
						if (timers[i] < 15 && timers[i] > 1) {
							if (step == 0) advanceTutorial();

							if (step == 4) {
								heldButton = nullptr;
								heldButtInd = -1;
								return;
							}

							if (playerGlobe->getStack().getSize() == 4 && step == 7) {
								tutorialstoredclashcard = i;
								advanceTutorial();
								return;
							}
							special chickType = playerGlobe->getHandAt(handMap[i]).getSpecial();
							playerGlobe->addToStackFromHand(handMap[i]);
							if (chickType != special::Spy) {
								handMap[i] = -1;
								for (int j = i + 1; j < 6; j++) {
									handMap[j]--;
								}
							}

							//Play chicken cluck sfx
							auto source = _assets->get<Sound>(CHICKEN_SCREECH);
							if (!AudioChannels::get()->isActiveEffect(CHICKEN_SCREECH)) {
								AudioChannels::get()->playEffect(CHICKEN_SCREECH, source, false, source->getVolume());
							}
						}
						heldButton = nullptr;
						heldButtInd = -1;
					}
				}
				else { //not grass or spy chicken for tutor
					if (down) {
						heldButton = buttons[i];
						if (timers[i] > 15) {
							infoCanvas->setVisible(true);
						}

					}
					if (!down) {
						if (timers[i] < 15 && timers[i] > 1) {
							if (playerGlobe->getStack().getSize() == 4 && step == 7) {
								tutorialstoredclashcard = i;
								advanceTutorial();
								return;
							}
							special chickType = playerGlobe->getHandAt(handMap[i]).getSpecial();
							playerGlobe->addToStackFromHand(handMap[i]);
							if (chickType != special::Spy) {
								handMap[i] = -1;
								for (int j = i + 1; j < 6; j++) {
									handMap[j]--;
								}
							}

							//Play chicken cluck sfx
							auto source = _assets->get<Sound>(CHICKEN_SCREECH);
							if (!AudioChannels::get()->isActiveEffect(CHICKEN_SCREECH)) {
								AudioChannels::get()->playEffect(CHICKEN_SCREECH, source, false, source->getVolume());
							}
						}
						heldButton = nullptr;
						heldButtInd = -1;
					}
				}
			}
			else { //non tutorial button
				if (down) {
					heldButton = buttons[i];
					if (timers[i] > INFO_DELAY) {
						infoCanvas->setVisible(true);
					}

				}
				if (!down) {
					if (timers[i] < INFO_DELAY && timers[i] > 1) {

						special chickType = playerGlobe->getHandAt(handMap[i]).getSpecial();
						playerGlobe->addToStackFromHand(handMap[i]);
						if (chickType != special::Spy) {
							handMap[i] = -1;
							for (int j = i + 1; j < 6; j++) {
								handMap[j]--;
							}
						}


						//Play chicken cluck sfx
						auto source = _assets->get<Sound>(CHICKEN_SCREECH);
						if (!AudioChannels::get()->isActiveEffect(CHICKEN_SCREECH) && !soundToggle) {
							AudioChannels::get()->playEffect(CHICKEN_SCREECH, source, false, source->getVolume());
						}
					}
					heldButton = nullptr;
					heldButtInd = -1;
					/*if (i < 3) {
						buttons[i]->setPosition((i - 1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET);
					}
					else {
						buttons[i]->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING);
					} */
				}
			}
		});

		buttonCanvas->addChild(butt);
		//i+2 to ensure keys are unique
		if (!tutor) {
			butt->activate(i + 2);
		}
		//CULog("Button %d made", i);
		buttons.push_back(butt);
		buttonTextures.push_back(id);
		texturesHand.push_back(text);
	}

	if (tutor) {
		//Disable all but grass chicken for tutorial step 0
		for (int i = 0; i < 6; i++) {
			buttons[i]->setVisible(true);
			if (i != 1) {
				buttons[i]->deactivate();
			}
			else {
				buttons[i]->activate(i + 2);
			}
		}
	}


	//Init the clash preview button

	std::shared_ptr<Texture> textureFight = _assets->get<Texture>("preview");

	std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(textureFight);
	id->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> butt = Button::alloc(id);
	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setScale(0.4, 0.4);

	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setPosition(0, screenHeight/3);
	butt->setListener([=](const std::string& name, bool down) {
		if (down) {
			//previewSet = true;
			prevTint = true;
			if (isTutor && step == 1) advanceTutorial();
		}
		if (!down) {
			prevTint = false;
			if (isTutor && step == 2) advanceTutorial();
		}
	});

	clashButtonCanvas->addChild(butt);
	//ensure keys are unique
	butt->activate(99);

	//Draw initial health
	//Scale factor * scene height makes the health bar appear in consistent locations, independent of device
	healthYScale = (((float)(HEALTH_BAR_Y_FACTOR - 1)) / ((float)HEALTH_BAR_Y_FACTOR)) * screenHeight;
	CULog("%d", healthYScale);

	//Bar
	std::shared_ptr<PolygonNode> hBar = PolygonNode::allocWithTexture(bar);
	hBar->setAnchor(Vec2::ANCHOR_CENTER);
	hBar->setScale(HBAR_SCALE);
	hBar->setPosition(screenWidth / 2, healthYScale);
	healthCanvas->addChild(hBar);

    //Blocks
    for (int i = 0; i < 5; i++) {
        std::shared_ptr<PolygonNode> playerB = PolygonNode::allocWithTexture(pBlock);
        playerB->setAnchor(Vec2::ANCHOR_CENTER);
        playerB->setScale(BLOCK_X_SCALE, BLOCK_Y_SCALE);
        playerB->setPosition(screenWidth / 2 - BAR_DISTANCE/2 - (i*HEALTH_BLOCK_SPACING), healthYScale);
        healthCanvas->addChild(playerB);
    }
    for (int i = 0; i < 5; i++) {
        std::shared_ptr<PolygonNode> oppB = PolygonNode::allocWithTexture(pBlock);
        oppB->setAnchor(Vec2::ANCHOR_CENTER);
        oppB->setScale(BLOCK_X_SCALE, BLOCK_Y_SCALE);
        oppB->setPosition(screenWidth / 2 + BAR_DISTANCE / 2 + (i*HEALTH_BLOCK_SPACING), healthYScale);
        healthCanvas->addChild(oppB);
    }

	//Hearts
	std::shared_ptr<PolygonNode> playerH = PolygonNode::allocWithTexture(pHeart);
	playerH->setAnchor(Vec2::ANCHOR_CENTER);
	playerH->setScale(HEART_SCALE);
	playerH->setPosition(screenWidth / 2 - HEART_X_OFFSET, healthYScale);
	healthCanvas->addChild(playerH);
	std::shared_ptr<PolygonNode> oppH = PolygonNode::allocWithTexture(pHeart);
	oppH->setAnchor(Vec2::ANCHOR_CENTER);
	oppH->setScale(HEART_SCALE);
	oppH->setPosition(screenWidth / 2 + HEART_X_OFFSET, healthYScale);
	healthCanvas->addChild(oppH);

	//Add elemental information
	std::shared_ptr<Texture> eltInfoText = _assets->get<Texture>("groupedElts");
	std::shared_ptr<PolygonNode> eltInfo = PolygonNode::allocWithTexture(eltInfoText);
	eltInfo->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	eltInfo->setScale(ELT_INFO_SCALE);
	eltInfo->setPosition(screenWidth / 2 - ELT_INFO_X_OFFSET, healthYScale - ELT_Y_OFFSET);
	eltInfoCanvas->addChild(eltInfo);

	//Add numeric values (also alloc textures here)
	num0 = _assets->get<Texture>("num0");
	num1 = _assets->get<Texture>("num1");
	num2 = _assets->get<Texture>("num2");
	num3 = _assets->get<Texture>("num3");
	num4 = _assets->get<Texture>("num4");
	numMinus = _assets->get<Texture>("minus");
	std::shared_ptr<PolygonNode> fireNum = PolygonNode::allocWithTexture(num1);
	std::shared_ptr<PolygonNode> waterNum = PolygonNode::allocWithTexture(num1);
	std::shared_ptr<PolygonNode> grassNum = PolygonNode::allocWithTexture(num1);
	fireNum->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	fireNum->setScale(ELT_NUM_SCALE);
	fireNum->setPosition(screenWidth / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET);
	eltInfoCanvas->addChild(fireNum);
	waterNum->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	waterNum->setScale(ELT_NUM_SCALE);
	waterNum->setPosition(screenWidth / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET - ELT_NUM_SPACING);
	eltInfoCanvas->addChild(waterNum);
	grassNum->setAnchor(Vec2::ANCHOR_TOP_CENTER);
	grassNum->setScale(ELT_NUM_SCALE);
	grassNum->setPosition(screenWidth / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET - (2*ELT_NUM_SPACING));
	eltInfoCanvas->addChild(grassNum);

	//Init the tutorial buttons
	tutcanvas1 = AnimationNode::alloc(tutor1, 1, TUTOR1_LENGTH, TUTOR1_LENGTH);
	tutcanvas1->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
	tutcanvas1->setScale(screenWidth / tutcanvas1->getWidth() / 1.5f, screenWidth / tutcanvas1->getWidth() / 1.5f);
	tutorialCanvas->addChild(tutcanvas1);
	tutcanvas1->setVisible(isTutor);

	tutcanvas2 = PolygonNode::allocWithTexture(tutor2);
	tutcanvas2->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	tutcanvas2->setScale(screenWidth / tutcanvas2->getWidth(), screenWidth / tutcanvas2->getWidth());
	tutorialCanvas->addChild(tutcanvas2);
	tutcanvas2->setVisible(false);

	tutcanvas3 = PolygonNode::allocWithTexture(tutor3);
	tutbutton3 = Button::alloc(tutcanvas3);
	tutbutton3->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	tutbutton3->setScale(screenWidth / tutbutton3->getWidth(), screenWidth / tutbutton3->getWidth());
	tutbutton3->setListener([=](const std::string& name, bool down) {
		if (step == 3) {
			if (down) {
				tutbuttonpressed = true;
			}
			else {
				if (tutbuttonpressed) {
					tutbuttonpressed = false;
					advanceTutorial();
				}
			}
		}
	});
	tutorialCanvas->addChild(tutbutton3);
	tutbutton3->setVisible(false);

	tutcanvas4 = PolygonNode::allocWithTexture(tutor4);
	tutcanvas4->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	tutcanvas4->setScale(screenWidth / tutcanvas4->getWidth(), screenWidth / tutcanvas4->getWidth());
	tutcanvas4->setPosition(0, screenHeight * 3 / 5);
	tutorialCanvas->addChild(tutcanvas4);
	tutcanvas4->setVisible(false);


	tutcanvas5 = PolygonNode::allocWithTexture(tutor5);
	tutbutton5 = Button::alloc(tutcanvas5);
	tutbutton5->setAnchor(Vec2::ANCHOR_TOP_LEFT);
	tutbutton5->setScale(screenWidth / tutbutton5->getWidth(), screenWidth / tutbutton5->getWidth());
	tutbutton5->setPosition(0, screenHeight * 1 / 2);
	tutbutton5->setListener([=](const std::string& name, bool down) {
		if (step == 6) {
			if (down) {
				tutbuttonpressed = true;
			}
			else {
				if (tutbuttonpressed) {
					tutbuttonpressed = false;
					advanceTutorial();
				}
			}
		}
	});
	tutorialCanvas->addChild(tutbutton5);
	tutbutton5->setVisible(false);

	tutcanvas6 = AnimationNode::alloc(tutor6, 1, TUTOR6_LENGTH, TUTOR6_LENGTH);
	tutbutton6 = Button::alloc(tutcanvas6);
	tutbutton6->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	tutbutton6->setScale(screenWidth / tutbutton6->getWidth(), screenWidth / tutbutton6->getWidth());
	tutbutton6->setListener([=](const std::string& name, bool down) {
		if (step == 8) {
			if (down) {
				tutbuttonpressed = true;
			}
			else {
				if (tutbuttonpressed) {
					tutbuttonpressed = false;
					advanceTutorial();
				}
			}
		}
	});
	tutorialCanvas->addChild(tutbutton6);
	tutbutton6->setVisible(false);

	tutcanvas7 = PolygonNode::allocWithTexture(tutor7);
	tutbutton7 = Button::alloc(tutcanvas7);
	tutbutton7->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	tutbutton7->setScale(screenWidth / tutbutton7->getWidth(), screenWidth / tutbutton7->getWidth());
	tutbutton7->setListener([=](const std::string& name, bool down) {
		if (step == 10) {
			if (down) {
				tutbuttonpressed = true;
			}
			else {
				if (tutbuttonpressed) {
					tutbuttonpressed = false;
					advanceTutorial();
				}
			}
		}
	});
	tutorialCanvas->addChild(tutbutton7);
	tutbutton7->setVisible(false);

	//Init the pause button
	std::shared_ptr<Texture> texturePause = _assets->get<Texture>("pausebutton");
	std::shared_ptr<PolygonNode> pauseid = PolygonNode::allocWithTexture(texturePause);
	pauseid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pausebutt = Button::alloc(pauseid);
	pausebutt->setScale(0.25, 0.25);
	pausebutt->setAnchor(Vec2::ANCHOR_CENTER);
	pausebutt->setPosition(screenWidth/2, healthYScale);
	pausebutt->setListener([=](const std::string& name, bool down) {
	    if (down && !hasWon && !hasLost) {
	        pauseMenuCanvas->setVisible(true);
            isPaused = true;
            playButtonSound(0);
	    }
	});
	pauseButtonCanvas->addChild(pausebutt);
	pausebutt->activate(200); //ensure keys are unique

	//Draw pause box
    std::shared_ptr<Texture> texturePauseOverlay = _assets->get<Texture>("pauseoverlay");
    std::shared_ptr<PolygonNode> pauseOverlay = PolygonNode::allocWithTexture(texturePauseOverlay);
    pauseOverlay->setAnchor(Vec2::ANCHOR_CENTER);
    pauseOverlay->setPosition(screenWidth / 2, screenHeight/2);
    pauseMenuCanvas->addChild(pauseOverlay);

	std::shared_ptr<Texture> texturePauseBox = _assets->get<Texture>("pausebox");
	std::shared_ptr<PolygonNode> pauseBox = PolygonNode::allocWithTexture(texturePauseBox);
	pauseBox->setScale(0.7, 0.7);
	pauseBox->setAnchor(Vec2::ANCHOR_CENTER);
	pauseBox->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight/2);
    pauseMenuCanvas->addChild(pauseBox);

    std::shared_ptr<Texture> texturePauseRestart = _assets->get<Texture>("pauserestart");
    std::shared_ptr<PolygonNode> pauserestartid = PolygonNode::allocWithTexture(texturePauseRestart);
	pauserestartid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pauseRestart = Button::alloc(pauserestartid);
    pauseRestart->setScale(0.65, 0.65);
    pauseRestart->setAnchor(Vec2::ANCHOR_CENTER);
    pauseRestart->setPosition(screenWidth / 4, screenHeight/2 + 50);
	pauseRestart->setListener([=](const std::string& name, bool down) {
        if (down) {
            retry = true;
            playButtonSound(0);
        }});
    pauseMenuCanvas->addChild(pauseRestart);
	pauseRestart->activate(201); //ensure keys are unique
	pausebuttons.push_back(pauseRestart); // 0

    std::shared_ptr<Texture> texturePauseHome = _assets->get<Texture>("pausehome");
    std::shared_ptr<PolygonNode> pausehomeid = PolygonNode::allocWithTexture(texturePauseHome);
	pausehomeid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pauseHome = Button::alloc(pausehomeid);
    pauseHome->setScale(0.65, 0.65);
    pauseHome->setAnchor(Vec2::ANCHOR_CENTER);
    pauseHome->setPosition(screenWidth / 2 , screenHeight/2 + 50);
	pauseHome->setListener([=](const std::string& name, bool down) {
        if (down) {
            goHome = true;
            playButtonSound(1);
        }});
    pauseMenuCanvas->addChild(pauseHome);
	pauseHome->activate(202); //ensure keys are unique
	pausebuttons.push_back(pauseHome); // 1
    
    std::shared_ptr<Texture> texturePauseResume = _assets->get<Texture>("pauseresume");
    std::shared_ptr<PolygonNode> pauseresumeid = PolygonNode::allocWithTexture(texturePauseResume);
    pauseresumeid->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> pauseResume = Button::alloc(pauseresumeid);
    pauseResume->setScale(0.65, 0.65);
    pauseResume->setAnchor(Vec2::ANCHOR_CENTER);
    pauseResume->setPosition(screenWidth/2, screenHeight/2 - INFO_Y_OFFSET);
    pauseResume->setListener([=](const std::string& name, bool down) {
        if (down) {
            isPaused = false;
            playButtonSound(0);
        }});
    pauseMenuCanvas->addChild(pauseResume);
    pauseResume->activate(203); //ensure keys are unique
    pausebuttons.push_back(pauseResume); // 2
    
    std::shared_ptr<Texture> texturePauseSettings = _assets->get<Texture>("pausesoundoff");
    std::shared_ptr<PolygonNode> pausesettingsid = PolygonNode::allocWithTexture(texturePauseSettings);
    pausesettingsid->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> pauseSettings = Button::alloc(pausesettingsid);
    pauseSettings->setScale(0.65, 0.65);
    pauseSettings->setAnchor(Vec2::ANCHOR_CENTER);
    pauseSettings->setPosition(screenWidth*3/4, screenHeight/2 + 50);
    pauseSettings->setListener([=](const std::string& name, bool down) { if (down) {
        soundToggle = soundToggle ? false : true;
        soundChanged = false;
        playButtonSound(1);
    }});
    pauseMenuCanvas->addChild(pauseSettings);
    pauseSettings->activate(204); //ensure keys are unique
    pausebuttons.push_back(pauseSettings); // 3

	deactivatePause();

	//Initialize distribution
	prevDist.push_back(1);
	prevDist.push_back(1);
	prevDist.push_back(1);

	//Init win and loss screens
	std::shared_ptr<PolygonNode> darkOverlay = PolygonNode::allocWithTexture(wlOverlay);
	darkOverlay->setScale(0.7f); // Magic number to rescale asset
	darkOverlay->setAnchor(Vec2::ANCHOR_CENTER);
	darkOverlay->setPosition(screenWidth / 2, screenHeight / 2);
	winCanvas->addChild(darkOverlay);

	std::shared_ptr<PolygonNode> darkOverlay2 = PolygonNode::allocWithTexture(wlOverlay);
	darkOverlay2->setScale(0.7f); // Magic number to rescale asset
	darkOverlay2->setAnchor(Vec2::ANCHOR_CENTER);
	darkOverlay2->setPosition(screenWidth / 2, screenHeight / 2);
	loseCanvas->addChild(darkOverlay2);

	std::shared_ptr<PolygonNode> winScreen = PolygonNode::allocWithTexture(victory);
	winScreen->setScale(WIN_LOSS_SCALE); // Magic number to rescale asset
	winScreen->setAnchor(Vec2::ANCHOR_CENTER);
	winScreen->setPosition(screenWidth / 2, screenHeight / 2 + WIN_LOSS_Y_OFFSET);
	winCanvas->addChild(winScreen);

	std::shared_ptr<PolygonNode> loseScreen = PolygonNode::allocWithTexture(defeat);
	loseScreen->setScale(WIN_LOSS_SCALE); // Magic number to rescale asset
	loseScreen->setAnchor(Vec2::ANCHOR_CENTER);
	loseScreen->setPosition(screenWidth / 2, screenHeight / 2 + WIN_LOSS_Y_OFFSET);
	loseCanvas->addChild(loseScreen);

	winCanvas->setVisible(false);
	loseCanvas->setVisible(false);
    
    textsign = _assets->get<Texture>("clashsign");
    sign = AnimationNode::alloc(textsign,1,SIGN_FILMSTRIP_LENGTH);
    sign->setScale(1.2); // Magic number to rescale asset
    sign->setAnchor(Vec2::ANCHOR_CENTER);
    sign->setPosition(screenWidth/2, healthYScale - 150);
    clashSignCanvas->addChild(sign);
    clashSignCanvas->setVisible(false);
    
    deactivatePause();

	return true;
}



void SceneBuilder1::chickDefeat(element playerType, element opponentType, int winResult) {
	//start animation for chicken fading
	//make child of chicken the element animation
	pShotProgress = 0;
	eShotProgress = 0;
	playerChickenWins = winResult;
	eType = playerType;
	pType = opponentType;
	CULog("somedefeat");
	dyingFrame[1] = -1;
	dyingFrame[0] = -1;
}

void SceneBuilder1::mooseDefeat(int healthChange) {
	if (healthChange > 0) {
		//opponent loses health
		oLose = true;
		pLose = false;
		damageTaken = healthChange;
	}
	else if (healthChange < 0) {
		oLose = false;
		pLose = true;
		damageTaken = -healthChange;
		
	}
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


void SceneBuilder1::updateGameScene(float timestep, bool isClashing) {
    if (isPaused && !pausebuttons[0]->isActive()){ activatePause(); }
    else if (!isPaused && pausebuttons[0]->isActive()){ deactivatePause(); }
    
    if (!soundChanged && isPaused){
        pausebuttons[3]->deactivate();
        pausebuttons.pop_back();
        std::shared_ptr<Texture> texturePauseSettings;
        if (!soundToggle){ texturePauseSettings = _assets->get<Texture>("pausesoundoff"); }
        else{ texturePauseSettings = _assets->get<Texture>("pausesoundon"); }
        std::shared_ptr<PolygonNode> pausesettingsid = PolygonNode::allocWithTexture(texturePauseSettings);
        pausesettingsid->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<Button> pauseSettings = Button::alloc(pausesettingsid);
        pauseSettings->setScale(0.65, 0.65);
        pauseSettings->setAnchor(Vec2::ANCHOR_CENTER);
        pauseSettings->setPosition(screenWidth*3/4, screenHeight/2 + 50);
        pauseSettings->setListener([=](const std::string& name, bool down) { if (down) {
            soundToggle = soundToggle ? false : true;
            soundChanged = false;
        }});
        pauseMenuCanvas->addChild(pauseSettings);
        pauseSettings->activate(204); //ensure keys are unique
        pausebuttons.push_back(pauseSettings); // 3
        
        soundChanged = true;
    }
    
	bool isNextFrame = false;
	if (!isPaused) {
		timeAmount += timestep;
		isNextFrame = (timeAmount > timeBtnFrames);
		if (timeAmount > timeBtnFrames)
		{
			isNextFrame = true;
			timeAmount = 0;
			if (thisFrame >= CHICKEN_FILMSTRIP_LENGTH - 1)
				thisFrame = 0;
			thisFrame++;
		}

		

	}


	//change mooses if taking damage;
	std::shared_ptr<Node> eMoose = mooseCanvas->getChildByName("opp_moose");
	std::shared_ptr<Node> pMoose = mooseCanvas->getChildByName("player_moose");

	if ((oLose || pLose) ) {
		std::shared_ptr<Texture> dText;

		if (damageTaken == 1) {
			dText = num1;
		}
		else if (damageTaken == 2) {
			dText = num2;
		}
		else if (damageTaken == 3) {
			dText = num3;
		}
		else if (damageTaken == 4) {
			dText = num4;
		}
		std::shared_ptr<Node> minus = PolygonNode::allocWithTexture(numMinus);
		std::shared_ptr<Node> damageNum = PolygonNode::allocWithTexture(dText);


		if (isNextFrame) {
			tintedTime += 1;
			if (tintedTime >= MOOSE_DAMAGE_TIME) {
				tintedTime = 0;
				oLose = false;
				pLose = false;
			}
		}
		int vmove = 10 * tintedTime;
		minus->setScale(1.5);
		damageNum->setScale(1.5);

		if (oLose) {
			eMoose->setColor(Color4f::RED);
			pMoose->setColor(Color4f::WHITE);
			//start drawing subtraction text
			if (eMoose->getChildCount() == 0) {
				eMoose->addChildWithTag(damageNum, 1);
				eMoose->addChildWithTag(minus, 2);
			}
			else {
				damageNum = eMoose->getChildByTag(1);
				minus = eMoose->getChildByTag(2);
			}
			//eMoose->removeChildByTag(1);
			
			
			eMoose->setPosition(screenWidth + MOOSE_X_OFFSET , eMoose->getPositionY());

			minus->setPosition( pMoose->getWidth() / 4, pMoose->getHeight() * 2 + 50 +vmove);
			damageNum->setPosition( eMoose->getWidth() / 4 + minus->getWidth(), eMoose->getHeight()*2 + 50 +  vmove);
			

		}
		else if (pLose) {
			pMoose->setColor(Color4f::RED);
			eMoose->setColor(Color4f::WHITE);
			
			if (pMoose->getChildCount() == 0) {
				pMoose->addChildWithTag(damageNum, 1);
				pMoose->addChildWithTag(minus, 2);
			}
			else {
				damageNum = pMoose->getChildByTag(1);
				minus = pMoose->getChildByTag(2);
			};
			
			pMoose->setPosition(-MOOSE_X_OFFSET , pMoose->getPositionY());

			minus->setPosition( pMoose->getWidth()*3 / 4, pMoose->getHeight()*2 + 50 +  vmove);
			damageNum->setPosition( pMoose->getWidth() * 3 / 4 + minus->getWidth(), pMoose->getHeight() * 2 + 50 + vmove);
		}
		
		
		//add to timer
		

	}

	
	 if(!pLose && !oLose) {
		eMoose->setColor(Color4f::WHITE);
		pMoose->setColor(Color4f::WHITE);
		eMoose->removeChildByTag(1);
		eMoose->removeChildByTag(2);
		pMoose->removeChildByTag(1);
		pMoose->removeChildByTag(2);
		

		pMoose->setPosition(-MOOSE_X_OFFSET , pMoose->getPositionY());
		eMoose->setPosition(screenWidth + MOOSE_X_OFFSET, eMoose->getPositionY());

	}
	

	if (isTutor && playerGlobe->getStack().getSize() == 0 && step == 9) {
		advanceTutorial();
	}

	


	vector <Chicken> hand = playerGlobe->getHand();

	if (playerGlobe->getHand().size() == 6) {
		handMap.clear();
		for (int i = 0; i < 6; i++) {
			handMap.push_back(i);
		}
	}

	if (isTutor) {
		if (step == 0) {
			for (int i = 0; i < 6; i++) {
				buttons[i]->setVisible(true);
				buttonCanvas->getChild(i)->setVisible(true);
				if (i == 1) buttons[i]->activate(i + 2);
				if (buttons[i] == heldButton) {
					heldButtInd = i;
					std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoG);
					newPoly->setVisible(false);
					infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
				}
			}
		}
		else if (step == 4 || step == 5) {
			for (int i = 0; i < 6; i++) {
				if (handMap[i] >= 0) {
					buttons[i]->setVisible(true);
					buttonCanvas->getChild(i)->setVisible(true);
					if (!isPaused && !hasWon && !hasLost && i == 4) {
						buttons[i]->activate(i + 2);
					}
					if (i == 4 && buttons[i] == heldButton) {
						heldButtInd = i;
						std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoSpy);
						newPoly->setScale(INFO_SCALE);
						newPoly->setAnchor(Vec2::ANCHOR_CENTER);
						newPoly->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
						infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
					}
					else if (buttons[i] == heldButton) {
						heldButtInd = i;
						std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoG);
						newPoly->setVisible(false);
						infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
					}
				}
				else {
					buttons[i]->setVisible(false);
					buttonCanvas->getChild(i)->setVisible(false);
					buttons[i]->deactivate();
				}
			}
		}
		else if (step != 7) {
			for (int i = 0; i < 6; i++) {
				if (handMap[i] >= 0) {
					buttons[i]->setVisible(true);
					buttonCanvas->getChild(i)->setVisible(true);
					if (buttons[i] == heldButton) {
						heldButtInd = i;
						std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoG);
						newPoly->setVisible(false);
						infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
					}
				}
				else {
					buttons[i]->setVisible(false);
					buttonCanvas->getChild(i)->setVisible(false);
				}
				buttons[i]->deactivate();
			}
		}
		else {
			for (int i = 0; i < 6; i++) {
				if (handMap[i] >= 0) {
					buttons[i]->setVisible(true);
					buttonCanvas->getChild(i)->setVisible(true);
					if (!isPaused && !hasWon && !hasLost) {
						buttons[i]->activate(i + 2);
					}
					if (buttons[i] == heldButton) {
						heldButtInd = i;
						//buttons[i]->setPosition(layer->screenToNodeCoords(_input.getCurTouch()) - Vec2(screenHeight / 2, 150));
						std::shared_ptr<Texture> infoText;
						special cel = playerGlobe->getHandAt(handMap[i]).getSpecial();
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
						newPoly->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
						infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);

					}
				}
				else {
					buttons[i]->setVisible(false);
					buttonCanvas->getChild(i)->setVisible(false);
					buttons[i]->deactivate();
				}
			}
		}

		//tutorial chicken highlights
		if (grasslightcanvas->isVisible() && isNextFrame) {
			int next = grasslightcanvas->getFrame() + 1;
			if (next == GRASS_FILMSTRIP_LENGTH) next = 0;
			grasslightcanvas->setFrame(next);
		}
		if (spylightcanvas->isVisible()) {
			int next = spylightcanvas->getFrame() + 1;
			if (next == SPY_FILMSTRIP_LENGTH) next = 0;
			spylightcanvas->setFrame(next);
		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			if (handMap[i] >= 0) {
				buttons[i]->setVisible(true);
				buttonCanvas->getChild(i)->setVisible(true);
				if (!isPaused && !hasWon && !hasLost) {
					buttons[i]->activate(i + 2);
				}
				if (buttons[i] == heldButton) {
					heldButtInd = i;
					//buttons[i]->setPosition(layer->screenToNodeCoords(_input.getCurTouch()) - Vec2(screenHeight / 2, 150));
					std::shared_ptr<Texture> infoText;
					special cel = playerGlobe->getHandAt(handMap[i]).getSpecial();
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
					newPoly->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
					infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);

				}
			}
			else {
				buttons[i]->setVisible(false);
				buttonCanvas->getChild(i)->setVisible(false);
				buttons[i]->deactivate();
			}
		}
	}
	

	if (sInfoInd != -1 && !isPaused && !hasWon && !hasLost) {
		std::shared_ptr<Texture> sInfoText;
		special cel;
		if (sInfoInd > 4) {
			cel = oppGlobe->getStackAt(sInfoInd - 5).getSpecial();
		}
		else {
			cel = playerGlobe->getStackAt(sInfoInd).getSpecial();
		}
		switch (cel) {
		case special::BasicFire:
			sInfoText = infoF;
			break;
		case special::BasicWater:
			sInfoText = infoW;
			break;
		case special::BasicGrass:
			sInfoText = infoG;
			break;
		case special::Reaper:
			sInfoText = infoReaper;
			break;
		case special::Bomb:
			sInfoText = infoBomb;
			break;
		case special::Mirror:
			sInfoText = infoMirror;
			break;
		case special::Ninja:
			sInfoText = infoNinja;
			break;
		case special::PartyFowl:
			sInfoText = infoParty;
			break;
		case special::Spy:
			sInfoText = infoSpy;
			break;
		case special::Thicken:
			sInfoText = infoThick;
			break;
		case special::Witchen:
			sInfoText = infoWitch;
			break;
		default:
			element el;
			if (sInfoInd > 4) {
				el = oppGlobe->getStackAt(sInfoInd - 5).getElement();
			}
			else {
				el = playerGlobe->getStackAt(sInfoInd).getElement();
			}
			switch (el) {
			case element::Fire:
				sInfoText = infoF;
				break;
			case element::Water:
				sInfoText = infoW;
				break;
			case element::Grass:
				sInfoText = infoG;
				break;
			}
		}
		std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(sInfoText);
		newPoly->setScale(INFO_SCALE);
		newPoly->setAnchor(Vec2::ANCHOR_CENTER);
		newPoly->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
		stackInfoCanvas->swapChild(stackInfoCanvas->getChild(0), newPoly, false);
		stackInfoCanvas->setVisible(true);
	}
	else {
		stackInfoCanvas->setVisible(false);
	}


	for (int i = 0; i < playerGlobe->getHand().size(); i++) {
		//Find which button is mapped to this hand chicken
		int mappedButton;
		CULog("Finding %d th card", i);
		for (int j = 0; j < 6; j++) {
			CULog("%d", handMap[j]);
			if (handMap[j] == i) {
				mappedButton = j;
			}
		}
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

		std::shared_ptr<Node> upchld = buttons[mappedButton]->getChild(0);

		std::shared_ptr<AnimationNode> newUp = AnimationNode::alloc(text, 1, CHICKEN_FILMSTRIP_LENGTH, CHICKEN_FILMSTRIP_LENGTH);
		//animates bottom chickens in coop
		int curFrame = (flappingFrame[i]);
		//        int curFrame = 0;
		int nextFrame = curFrame + 1;

		//decides if a flapping animation should start
		if (isNextFrame && (curFrame != 0 || std::rand() % 50 == 0)) {

			if (nextFrame >= CHICKEN_FILMSTRIP_LENGTH - 1) {
				nextFrame = 0;

			}
			flappingFrame[i] = nextFrame;
			newUp->setFrame(nextFrame);
		}
		else {
			newUp->setFrame(nextFrame - 1);

		}

		newUp->flipHorizontal(true);
		buttons[mappedButton]->swapChild(upchld, newUp, false);


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
		bool isChange = pstackNodes[i]->getTexture() != (text);
		std::shared_ptr<AnimationNode> chick = AnimationNode::alloc(text, 1, CHICKEN_FILMSTRIP_LENGTH);
		chick->setScale(STACK_SCALE); // Magic number to rescale asset
		chick->setAnchor(Vec2::ANCHOR_CENTER);
		chick->setPosition(pstackNodes[i]->getPositionX(), pstackNodes[i]->getPositionY());
		chick->flipHorizontal(true);

		if (prevTint && i < (playerGlobe->getNumChickensWillDiePreview())) {
			chick->setColor(Color4(Vec4(1, 0, 0, 0.5)));
		}


		layer->swapChild(pstackNodes[i], chick, false);
		pstackNodes[i] = chick;

		texturesPStack[i] = text;

		pstackNodes[i]->setFrame(thisFrame);

		if (isChange && !isClashing) {
			pSmokeFrame[i] = 0;
		}


		if (pSmokeFrame[i] >= 5) {
			pSmokeFrame[i] = -1;
		}
		if (isNextFrame && pSmokeFrame[i] != -1) {
			pSmokeFrame[i] ++;
		}

		if (pSmokeFrame[i] != -1) {
			std::shared_ptr<AnimationNode> smoke = AnimationNode::alloc(smokeTrans, 1, 6);
			pstackNodes[i]->addChild(smoke);
			int fo = 6 - 1 - pSmokeFrame[i];
			smoke->setFrame(6 - 1 - pSmokeFrame[i]);
		}

		//START DEATH ANIMATION HERE
	
		//shot starts here
		if (pShotProgress != -1 && i == 0) {

			int elemFrame = 0;
			std::shared_ptr<Texture> shot;
			switch (eType) {
			case element::Fire:
				shot = fireShot;
				elemFrame = 2;
				break;
			case element::Water:
				shot = waterShot;
				elemFrame = 1;
				break;
			case element::Grass:
				shot = grassShot;
				elemFrame = 0;
				break;
			default:
				shot = fireShot;
				elemFrame = 2;
				break;
			}

			std::shared_ptr<AnimationNode> projec = AnimationNode::alloc(shot, 1, 3);
			pstackNodes[i]->addChild(projec);
			projec->setScale(0.6);
			projec->setFrame(elemFrame);

			middleScreen = pstackNodes[i]->getWorldPosition().x + (ostackNodes[i]->getWorldPosition().x - pstackNodes[i]->getWorldPosition().x)/2 ;

			//a shot has begun
			int shotX = BULLET_SPEED * pShotProgress + pstackNodes[i]->getWidth() * 2;
			projec->setPosition(shotX, pstackNodes[0]->getHeight());
			
			if (pShotProgress > 4) {
				if(dyingFrame[0] == -1){
					
					dyingFrame[0] = 0;
				}

				if(dyingFrame[1] == -1)
					dyingFrame[1] = 0;
				
				
				pShotProgress = -1;
				eShotProgress = -1;
			}

			if (isNextFrame && pShotProgress != -1) {

				pShotProgress += 1;
			}
		}

		if (i == 0) {

			std::shared_ptr<Texture> deathText;
			switch (pType) {
			case element::Fire:
				deathText = fireTrans;
				break;
			case element::Water:
				deathText = waterTrans;
				break;
			case element::Grass:
				deathText = grassTrans;
				break;
			default:
				deathText = fireTrans;
				break;
			}

			

			if (isNextFrame && dyingFrame[0] != -1 && playerChickenWins <1) {
				dyingFrame[0] += 1;

				if (dyingFrame[0] >= DEATH_ANIM_COLS) {
					dyingFrame[0] = -2;
					
					//set chicken to not visible!
				}
			}

			if (dyingFrame[0] == -2) {
				pstackNodes[i]->setVisible(false);
			}

			if (dyingFrame[0] > -1 && playerChickenWins <1)
			{
				std::shared_ptr<AnimationNode> poof = AnimationNode::alloc(deathText, 1, DEATH_ANIM_COLS);
				pstackNodes[i]->addChild(poof);
				poof->setFrame(dyingFrame[0]);
				//poof->flipHorizontal(false);

				if (dyingFrame[0] > 4) {
					poof->setScale(STACK_SCALE); // Magic number to rescale asset
					poof->setAnchor(Vec2::ANCHOR_CENTER);
					poof->setPosition(pstackNodes[i]->getPositionX(), pstackNodes[i]->getPositionY());
					
					layer->swapChild(pstackNodes[i], poof, false);
				}
			}

		}


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
		bool isChange = ostackNodes[i]->getTexture() != (text);
		std::shared_ptr<AnimationNode> chick = AnimationNode::alloc(text, 1, CHICKEN_FILMSTRIP_LENGTH);
		chick->setScale(STACK_SCALE); // Magic number to rescale asset
		chick->setAnchor(Vec2::ANCHOR_CENTER);
		chick->setPosition(ostackNodes[i]->getPositionX(), ostackNodes[i]->getPositionY());
		chick->flipHorizontal(false);

		if (prevTint && i < (oppGlobe->getNumChickensWillDiePreview())) {
			chick->setColor(Color4(Vec4(1, 0, 0, 0.5)));
		}

		layer->swapChild(ostackNodes[i], chick, false);
		ostackNodes[i] = chick;

		texturesPStack[i] = text;

		ostackNodes[i]->setFrame(thisFrame);

		if (isChange && !isClashing) {
			eSmokeFrame[i] = 0;
		}


		if (eSmokeFrame[i] >= 5) {
			eSmokeFrame[i] = -1;
		}
		if (isNextFrame && eSmokeFrame[i] != -1) {
			eSmokeFrame[i] ++;
		}

		if (eSmokeFrame[i] != -1) {
			std::shared_ptr<AnimationNode> smoke = AnimationNode::alloc(smokeTrans, 1, 6);
			ostackNodes[i]->addChild(smoke);
			int fo = 6 - 1 - eSmokeFrame[i];
			smoke->setFrame(6 - 1 - eSmokeFrame[i]);
		}

		if (eShotProgress != -1 && i == 0) {

			int elemFrame = 0;
			std::shared_ptr<Texture> shot;
			switch (pType) {
			case element::Fire:
				shot = fireShot;
				elemFrame = 0;
				break;
			case element::Water:
				shot = waterShot;
				elemFrame = 1;
				break;
			case element::Grass:
				shot = grassShot;
				elemFrame = 2;
				break;
			default:
				shot = fireShot;
				elemFrame = 0;
				break;
			}

			std::shared_ptr<AnimationNode> projec = AnimationNode::alloc(shot, 1, 3);
			ostackNodes[i]->addChild(projec);
			projec->setScale(0.6);
			projec->setFrame(elemFrame);

			middleScreen = pstackNodes[i]->getWorldPosition().x + (ostackNodes[i]->getWorldPosition().x - pstackNodes[i]->getWorldPosition().x) / 2;

			//a shot has begun
			int shotX = -BULLET_SPEED * eShotProgress ;
			projec->setPosition(shotX, ostackNodes[0]->getHeight());
			projec->flipHorizontal(true);
			
			if (isNextFrame && eShotProgress != -1) {

				eShotProgress += 1;
			}
		}


		if (i == 0) {

			std::shared_ptr<Texture> deathText;
			switch (eType) {
			case element::Fire:
				deathText = fireTrans;
				break;
			case element::Water:
				deathText = waterTrans;
				break;
			case element::Grass:
				deathText = grassTrans;
				break;
			default:
				deathText = fireTrans;
				break;
			}

			if (isNextFrame && dyingFrame[1] != -1 && playerChickenWins > -1) {
				dyingFrame[1] += 1;

				if (dyingFrame[1] >= DEATH_ANIM_COLS) {
					dyingFrame[1] = -2;

					//set chicken to not visible!
				}
			}

			if (dyingFrame[1] == -2) {
				ostackNodes[i]->setVisible(false);
			}

			if (dyingFrame[1] > -1 && playerChickenWins > -1)
			{
				std::shared_ptr<AnimationNode> poof = AnimationNode::alloc(deathText, 1, DEATH_ANIM_COLS);
				ostackNodes[i]->addChild(poof);
				poof->setFrame(DEATH_ANIM_COLS - 1 - dyingFrame[1]);
				//poof->flipHorizontal(false);

				if (dyingFrame[1] > 4) {
					poof->setScale(STACK_SCALE); // Magic number to rescale asset
					poof->setAnchor(Vec2::ANCHOR_CENTER);
					poof->setPosition(ostackNodes[i]->getPositionX(), ostackNodes[i]->getPositionY());
					poof->flipHorizontal(true);
					layer->swapChild(ostackNodes[i], poof, false);
				}
			}

		}
	}

	if (isTutor) {
		//Tutorial Animations
		if (tutcanvas1->isVisible() && isNextFrame) {
			int next = tutcanvas1->getFrame() + 1;
			if (next == TUTOR1_LENGTH) next = 0;
			tutcanvas1->setFrame(next);
		}
		if (tutcanvas6->isVisible() && isNextFrame) {
			tutorialcountingvariable++;
			if (tutorialcountingvariable > 8) {
				tutorialcountingvariable = 0;
				int next = tutcanvas6->getFrame() + 1;
				if (next == TUTOR6_LENGTH) next = 0;
				tutcanvas6->setFrame(next);
			}
		}
	}


	//Update the info card
	if (heldButton == nullptr) {
		infoCanvas->setVisible(false);
		if (isTutor && step == 5) advanceTutorial();
	}
	else if (timers[heldButtInd] > 15) {
		infoCanvas->setVisible(true);
		if (isTutor && step == 4) advanceTutorial();
	}

	//Update the opponent health bar
	for (int i = 1; i < 6; i++) {
		std::shared_ptr<Node> child = healthCanvas->getChild(11 - i);
		if (oppGlobe->getHealth() < i) {
			child->setVisible(false);
		}
		else {
			child->setVisible(true);
		}
	}

	//Update the player health bar
	for (int i = 1; i < 6; i++) {
		std::shared_ptr<Node> child = healthCanvas->getChild(6 - i);
		if (playerGlobe->getHealth() < i) {
			child->setVisible(false);
		}
		else {
			child->setVisible(true);
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
			swapPoly->setPosition(screenWidth / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET - (i*ELT_NUM_SPACING));
			swapPoly->setVisible(true);
			std::shared_ptr<Node> oldChild = eltInfoCanvas->getChild(i + 1);
			eltInfoCanvas->swapChild(oldChild, swapPoly, false);
		}
	}

	//update stamps

	for (int i = 0; i < playerGlobe->getStack().getSize(); i++) {
		Chicken chick = playerGlobe->getStackAt(i);
		if (chick.isCycled()) {
			if (chick.getElement() == element::Fire) {
				pStamps[i]->setTexture(fstamp);
				pStamps[i]->setVisible(true);
			}
			else if (chick.getElement() == element::Water) {
				pStamps[i]->setTexture(wstamp);
				pStamps[i]->setVisible(true);
			}
			else if (chick.getElement() == element::Grass) {
				pStamps[i]->setTexture(gstamp);
				pStamps[i]->setVisible(true);
			}
			else {

			}
		}
		else {
			pStamps[i]->setVisible(false);
		}
	}

	for (int i = 0; i < oppGlobe->getStack().getSize(); i++) {

		Chicken chick = oppGlobe->getStackAt(i);
		if (chick.isCycled()) {
			if (chick.getElement() == element::Fire) {
				oStamps[i]->setTexture(fstamp);
				oStamps[i]->setVisible(true);
			}
			else if (chick.getElement() == element::Water) {
				oStamps[i]->setTexture(wstamp);
				oStamps[i]->setVisible(true);
			}
			else if (chick.getElement() == element::Grass) {
				oStamps[i]->setTexture(gstamp);
				oStamps[i]->setVisible(true);
			}
			else {

			}
		}
		else {
			oStamps[i]->setVisible(false);
		}
	}
    
    if (isClashing && !signDone && !isPaused){
        clashSignCanvas->setVisible(true);
        if (signframe >= SIGN_FILMSTRIP_LENGTH - 1){
            signDone = true;
            clashSignCanvas->setVisible(false);
        }
        signCount-= timestep;
        //if (STILL_SIGN_TIME - signCount < (signframe*2.0/(SIGN_FILMSTRIP_LENGTH)) ){ signframe++; }
		if (isNextFrame) {
			signframe++;
		}
        sign->setFrame(signframe);
        return;
    }
    if (!isClashing && signDone){
        signDone = false;
        signCount = STILL_SIGN_TIME;
        signframe = 1;
    }

	if (signCount <= 0 ) {
		signDone = true;
	}

	if (signDone) {
		signCount = STILL_SIGN_TIME;
		clashSignCanvas->setVisible(false);
	}

	//Update win and loss screens
	if (playerGlobe->getHealth() <= 0 && mooseCanvas->getChildByName("player_moose")->getColor() == Color4f::WHITE) {
		loseCanvas->setVisible(true);
		if (!hasLost) {
			//Create the loss home button
			std::shared_ptr<PolygonNode> homePolyL = PolygonNode::allocWithTexture(homeButton);
			homePolyL->setAnchor(Vec2::ANCHOR_CENTER);
			std::shared_ptr<Button> hButtL = Button::alloc(homePolyL);
			hButtL->setAnchor(Vec2::ANCHOR_CENTER);
			hButtL->setScale(WIN_LOSS_B_SCALE);
			hButtL->setAnchor(Vec2::ANCHOR_CENTER);
			hButtL->setPosition(screenWidth / 2 + LOSS_BUTTON_X_SPACING / 2, screenHeight / 2 + WIN_LOSS_Y_OFFSET + WIN_LOSS_B_Y_OFFSET);
			hButtL->setListener([=](const std::string& name, bool down) {
				if (down) {
					goHome = true;
					loseCanvas->setVisible(false);
                    playButtonSound(1);
				}
			});
			loseCanvas->addChild(hButtL);
			//ensure keys are unique
			hButtL->activate(53);
            losebuttons.push_back(hButtL); // 0

			//Create the loss redo button
			std::shared_ptr<PolygonNode> redoPolyL = PolygonNode::allocWithTexture(redo);
			redoPolyL->setAnchor(Vec2::ANCHOR_CENTER);
			std::shared_ptr<Button> rButtL = Button::alloc(redoPolyL);
			rButtL->setAnchor(Vec2::ANCHOR_CENTER);
			rButtL->setScale(WIN_LOSS_B_SCALE);
			rButtL->setAnchor(Vec2::ANCHOR_CENTER);
			rButtL->setPosition(screenWidth / 2 - LOSS_BUTTON_X_SPACING / 2, screenHeight / 2 + WIN_LOSS_Y_OFFSET + WIN_LOSS_B_Y_OFFSET);
			rButtL->setListener([=](const std::string& name, bool down) {
				if (down) {
					retry = true;
					loseCanvas->setVisible(false);
                    playButtonSound(0);
				}
			});
			loseCanvas->addChild(rButtL);
			//ensure keys are unique
			rButtL->activate(54);
            losebuttons.push_back(rButtL); // 1
		}
		hasLost = true;
		deactivateHand();
	}
	else if (oppGlobe->getHealth() <= 0 && mooseCanvas->getChildByName("opp_moose")->getColor() == Color4f::WHITE) {
		winCanvas->setVisible(true);
		if (!hasWon) {
			//Init the win home button
			std::shared_ptr<PolygonNode> homePoly = PolygonNode::allocWithTexture(homeButton);
			homePoly->setAnchor(Vec2::ANCHOR_CENTER);
			std::shared_ptr<Button> hButt = Button::alloc(homePoly);
			hButt->setAnchor(Vec2::ANCHOR_CENTER);
			hButt->setScale(WIN_LOSS_B_SCALE);
			hButt->setAnchor(Vec2::ANCHOR_CENTER);
			hButt->setPosition(screenWidth / 2, screenHeight / 2 + WIN_LOSS_Y_OFFSET + WIN_LOSS_B_Y_OFFSET);
			hButt->setListener([=](const std::string& name, bool down) {
				if (down) {
					goHome = true;
					winCanvas->setVisible(false);
                    playButtonSound(1);
				}
			});
			winCanvas->addChild(hButt);
			//ensure keys are unique
			hButt->activate(50);
            winbuttons.push_back(hButt); // 0

			//Init the win redo button
			std::shared_ptr<PolygonNode> redoPoly = PolygonNode::allocWithTexture(redo);
			redoPoly->setAnchor(Vec2::ANCHOR_CENTER);
			std::shared_ptr<Button> rButt = Button::alloc(redoPoly);
			rButt->setAnchor(Vec2::ANCHOR_CENTER);
			rButt->setScale(WIN_LOSS_B_SCALE);
			rButt->setAnchor(Vec2::ANCHOR_CENTER);
			rButt->setPosition(screenWidth / 2 - WIN_BUTTON_X_SPACING, screenHeight / 2 + WIN_LOSS_Y_OFFSET + WIN_LOSS_B_Y_OFFSET);
			rButt->setListener([=](const std::string& name, bool down) {
				if (down) {
					retry = true;
					winCanvas->setVisible(false);
                    playButtonSound(0);
				}
			});
			winCanvas->addChild(rButt);
			//ensure keys are unique
			rButt->activate(51);
            winbuttons.push_back(rButt); // 1

			//Init the win next level button
			std::shared_ptr<PolygonNode> levelPoly = PolygonNode::allocWithTexture(nextlvl);
			levelPoly->setAnchor(Vec2::ANCHOR_CENTER);
			std::shared_ptr<Button> lButt = Button::alloc(levelPoly);
			lButt->setAnchor(Vec2::ANCHOR_CENTER);
			lButt->setScale(WIN_LOSS_B_SCALE);
			lButt->setAnchor(Vec2::ANCHOR_CENTER);
			lButt->setPosition(screenWidth / 2 + WIN_BUTTON_X_SPACING, screenHeight / 2 + WIN_LOSS_Y_OFFSET + WIN_LOSS_B_Y_OFFSET);
			lButt->setListener([=](const std::string& name, bool down) {
				if (down) {
					nextLevel = true;
					winCanvas->setVisible(false);
                    playButtonSound(0);
				}
			});
			winCanvas->addChild(lButt);
			//ensure keys are unique
			lButt->activate(52);
            winbuttons.push_back(lButt); // 2
		}
		hasWon = true;
		deactivateHand();
	}
	else {

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

void SceneBuilder1::setHome(bool val) {
	goHome = val;
}

void SceneBuilder1::setLevelNum(int levelNum) {
	hasWon = false;
	hasLost = false;
	isPaused = false;
	retry = false;
	goHome = false;
	nextLevel = false;
	isTutor = false;
	sInfoInd = -1;
	step = -1;
	tutbuttonpressed = false;
	tutorialstoredclashcard = -1;

	backCanvas->removeAllChildren();
	frontCanvas->removeAllChildren();
	tutorialCanvas->setVisible(false);
	//Draw background
	std::shared_ptr<Texture> texturebg;
	if (levelNum < 4) {
		texturebg = _assets->get<Texture>("farmbg");
	}
	else if (levelNum < 7) {
		texturebg = _assets->get<Texture>("forestbg");
	}
	else if (levelNum < 10) {
		texturebg = _assets->get<Texture>("nuclearbg");
	}
	else {
		texturebg = _assets->get<Texture>("thronebg");
	}
	std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
	background->setScale(0.65f); // Magic number to rescale asset
	background->setAnchor(Vec2::ANCHOR_CENTER);
	background->setPosition(screenWidth / 2, screenHeight / 2);
	backCanvas->addChild(background);

	//Draw foreground
	std::shared_ptr<Texture> texturefg;
	if (levelNum < 4) {
		texturefg = _assets->get<Texture>("farmfg");
	}
	else if (levelNum < 7) {
		texturefg = _assets->get<Texture>("farmfg");
	}
	else if (levelNum < 10) {
		texturefg = _assets->get<Texture>("nuclearfg");
	}
	else {
		texturefg = _assets->get<Texture>("thronefg");
	}
	std::shared_ptr<PolygonNode> foreground = PolygonNode::allocWithTexture(texturefg);
	foreground->setScale(0.65f); // Magic number to rescale asset
	foreground->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
	foreground->setPosition(screenWidth / 2, FORE_HEIGHT);
	frontCanvas->addChild(foreground);

	//rebind buttons
	for (int i = 0; i < 6; i++) {
		buttons[i]->activate(i + 2);
	}
}

void SceneBuilder1::setTutorial() {
	hasWon = false;
	hasLost = false;
	isPaused = false;
	retry = false;
	goHome = false;
	nextLevel = false;
	isTutor = true;
	sInfoInd = -1;
	step = 0;
	tutbuttonpressed = false;
	tutorialstoredclashcard = -1;

	backCanvas->removeAllChildren();
	frontCanvas->removeAllChildren();
	//Draw background
	std::shared_ptr<Texture> texturebg = _assets->get<Texture>("farmbg");
	std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
	background->setScale(0.65f); // Magic number to rescale asset
	background->setAnchor(Vec2::ANCHOR_CENTER);
	background->setPosition(screenWidth / 2, screenHeight / 2);
	backCanvas->addChild(background);

	//Draw foreground
	std::shared_ptr<Texture> texturefg = _assets->get<Texture>("farmfg");
	std::shared_ptr<PolygonNode> foreground = PolygonNode::allocWithTexture(texturefg);
	foreground->setScale(0.65f); // Magic number to rescale asset
	foreground->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
	foreground->setPosition(screenWidth / 2, FORE_HEIGHT);
	frontCanvas->addChild(foreground);

	playerGlobe->refillHand();
	oppGlobe->refillHand();

	//Disable all but grass chicken for tutorial step 0
	for (int i = 0; i < 6; i++) {
		buttons[i]->setVisible(true);
		if (i != 1) {
			buttons[i]->deactivate();
		}
		else {
			buttons[i]->activate(i + 2);
		}
	}

	tutorialCanvas->setVisible(true);
	grasslightcanvas->setVisible(true);
	tutcanvas1->setVisible(true);
}

void SceneBuilder1::setOppCost(string costume) {
	mooseCanvas->removeChildByName("opp_moose");
	//Draw opponent moose
	std::shared_ptr<PolygonNode> moose2;
	std::shared_ptr<Texture> textureM;

	if (costume == "basic_moose") {
		textureM = _assets->get<Texture>("moose");
	}
	else if (costume == "eldrich_moose") {
		textureM = _assets->get<Texture>("elMoose");
	}
	else if (costume == "branchy") {
		textureM = _assets->get<Texture>("branchy");
	}
	else if (costume == "caffeine_moose") {
		textureM = _assets->get<Texture>("cafMoose");
	}
	else if (costume == "emerald_moose") {
		textureM = _assets->get<Texture>("emMoose");
	}
	else if (costume == "elder_moose") {
		textureM = _assets->get<Texture>("oldMoose");
	}
	else if (costume == "moose_majesty") {
		textureM = _assets->get<Texture>("kingMoose");
	}
	else if (costume == "moosesassin") {
		textureM = _assets->get<Texture>("rogMoose");
	}
	else if (costume == "nebula_moose") {
		textureM = _assets->get<Texture>("cosMoose");
	}
	else if (costume == "tentamoose") {
		textureM = _assets->get<Texture>("tenMoose");
	}
	else if (costume == "moosewave") {
		textureM = _assets->get<Texture>("vapMoose");
	}
	else if (costume == "chrismoose") {
		textureM = _assets->get<Texture>("chrMoose");
	}
	else if (costume == "farmer_moose") {
		textureM = _assets->get<Texture>("farmMoose");
	}
	else {
		textureM = _assets->get<Texture>("moose");
	}

	moose2 = PolygonNode::allocWithTexture(textureM);
	moose2->setScale(MOOSE_SCALE); // Magic number to rescale asset
	moose2->setAnchor(Vec2::ANCHOR_BOTTOM_RIGHT);
	moose2->setPosition(screenWidth + MOOSE_X_OFFSET, MOOSE_HEIGHT);
	moose2->flipHorizontal(true);
	mooseCanvas->addChildWithName(moose2, "opp_moose");
    
	if (retry || goHome || nextLevel || !isPaused) {
        deactivatePause();
        deactivateWin();
        deactivateLose();
    };

}

void SceneBuilder1::playButtonSound(int sound) {
    //Play the button sfx
    // 0 is A, 1 is B
    string sfx = sound ? SOUND_BUTTON_A : SOUND_BUTTON_B;
    auto source = _assets->get<Sound>(sfx);
    if (!AudioChannels::get()->isActiveEffect(SOUND_BUTTON_A) && !AudioChannels::get()->isActiveEffect(SOUND_BUTTON_B)) {
        AudioChannels::get()->playEffect(sfx, source, false, source->getVolume());
    }
}

void SceneBuilder1::deactivateHand() {
	vector <Chicken> hand = playerGlobe->getHand();

	for (int i = 0; i < 6; i++) {
		//buttons[i]->setVisible(false);
		buttons[i]->deactivate();
	}
}

void SceneBuilder1::activateHand() {
	vector <Chicken> hand = playerGlobe->getHand();

	for (int i = 0; i < 6; i++) {
		buttons[i]->setVisible(true);
		buttons[i]->activate(i + 2);
	}
}

void SceneBuilder1::activatePause() {
	deactivateHand();
    pauseMenuCanvas->setVisible(true);
	if (isTutor) tutorialCanvas->setVisible(false);
	for (int i = 0; i < 4; i++) {
		pausebuttons[i]->activate(201 + i);
	}
}

void SceneBuilder1::deactivatePause() {
    pauseMenuCanvas->setVisible(false);
	if (isTutor) tutorialCanvas->setVisible(true);
	for (int i = 0; i < 4; i++) {
		pausebuttons[i]->deactivate();
	}
    isPaused = false;
}

void SceneBuilder1::activateWin() {
    winCanvas->setVisible(true);
    for (int i = 0; i < winbuttons.size(); i++) {
        winbuttons[i]->activate(50 + i);
    }
}

void SceneBuilder1::deactivateWin() {
    winCanvas->setVisible(false);
    for (int i = 0; i < winbuttons.size(); i++) {
        winbuttons[i]->deactivate();
    }
    hasWon = false;
}

void SceneBuilder1::activateLose() {
    loseCanvas->setVisible(true);
    for (int i = 0; i < losebuttons.size(); i++) {
        losebuttons[i]->activate(53 + i);
    }
}

void SceneBuilder1::deactivateLose() {
    loseCanvas->setVisible(false);
    for (int i = 0; i < losebuttons.size(); i++) {
        losebuttons[i]->deactivate();
    }
    hasLost = false;
}

bool SceneBuilder1::getHome() {
	if (isTutor && goHome) exitTutorial();
	return goHome;
}

bool SceneBuilder1::getRedo() {
	if (isTutor && retry) {
		exitTutorial();
	}
	return retry;
}

bool SceneBuilder1::getNextLevel() {
	if (isTutor && nextLevel) exitTutorial();
	return nextLevel;
}

bool SceneBuilder1::getSoundToggle() {
    return soundToggle;
}

bool SceneBuilder1::getPaused() {
	return isPaused;
}

void SceneBuilder1::advanceTutorial() {
	if (!isTutor) {
		CULogError("advanceTutorial called outside Tutorial");
	}

	//CULog("advanceTutorial called at step %i, is now step %i", step, step + 1);
	// 0. Begin Tutorial : show tutor 1 and highlight grass chicken.Disable all chicken buttons but grass.
	if (step == 0) {
		//1. Grass chicken dragged to stack, stop highlighting it.  Stop showing tutorial 1. Show tutorial 2.
		grasslightcanvas->setVisible(false);
		tutcanvas1->setVisible(false);
		tutcanvas2->setVisible(true);
	}
	else if (step == 1) {
		//2. Preview pressed.  Stop showing tutorial 2.
		tutcanvas2->setVisible(false);
	}
	else if (step == 2) {
		//3. Preview let go.  Show tutorial 3.  
		tutbutton3->setVisible(true);
		tutbutton3->activate(403);
	}
	else if (step == 3) {
		//4. Tutorial 3 pressed. Stop showing tutorial 3.  Show tutorial 4.  Highlight spy Chicken.
		tutbutton3->setVisible(false);
		tutcanvas4->setVisible(true);
		spylightcanvas->setVisible(true);
	}
	else if (step == 4) {
		//Bug with hand map and spy rip
		//5. Spy card seen.Stop showing Tutorial 4. 
		tutcanvas4->setVisible(false);
		spylightcanvas->setVisible(false);
	}
	else if (step == 5){
		//6. spy card let go. stop highlighting spy. show tutorial 5
		tutbutton5->setVisible(true);
		tutbutton5->activate(405);
	}
	else if (step == 6) {
		//6. Tutorial 5 tapped.Stop showing tutorial 5. Wait till stack size reaches 5
		tutbutton5->setVisible(false);
	}
	else if (step == 7) {
		//7. Stack size reached 5.  Show tutorial 6.
		heldButton = nullptr;
		heldButtInd = -1;

		tutcanvas6->setFrame(0);
		tutbutton6->setVisible(true);
		tutbutton6->activate(406);
	}
	else if (step == 8) {
		//8. Tutorial 6 tapped. stop showing tutorial 6. start clash. Wait for clash animation to end.
		tutbutton6->setVisible(false);

		special chickType = playerGlobe->getHandAt(handMap[tutorialstoredclashcard]).getSpecial();
		playerGlobe->addToStackFromHand(handMap[tutorialstoredclashcard]);
		/*if (chickType != special::Spy) {
			handMap[tutorialstoredclashcard] = -1;
			for (int j = tutorialstoredclashcard + 1; j < 6; j++) {
				handMap[j]--;
			}
		} */
		tutorialstoredclashcard = -1;

		//Play chicken cluck sfx
		auto source = _assets->get<Sound>(CHICKEN_SCREECH);
		if (!AudioChannels::get()->isActiveEffect(CHICKEN_SCREECH)) {
			AudioChannels::get()->playEffect(CHICKEN_SCREECH, source, false, source->getVolume());
		}
	}
	else if (step == 9) {
		//9. Showing Tutorial 7.
		tutbutton7->setVisible(true);
		tutbutton7->activate(407);
	}
	else if (step == 10) {
		//10. Tutorial 7 tapped.ending tutorial.
		tutbutton7->setVisible(false);
		step = -2;
		//Insert end tutorial stuff here
		goHome = true;
	}
	step++;
}

void SceneBuilder1::exitTutorial() {
	step = -1;

	grasslightcanvas->setVisible(false);
	spylightcanvas->setVisible(false);
	tutcanvas1->setVisible(false);
	tutcanvas2->setVisible(false);
	tutbutton3->setVisible(false);
	tutcanvas4->setVisible(false);
	tutbutton5->setVisible(false);
	tutbutton6->setVisible(false);
	tutbutton7->setVisible(false);

	tutbutton3->deactivate();
	tutbutton5->deactivate();
	tutbutton6->deactivate();
	tutbutton7->deactivate();
}

