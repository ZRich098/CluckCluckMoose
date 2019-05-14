#include "TutorialSB.h"

using namespace cugl;

bool TutorialSB::init(const std::shared_ptr<cugl::AssetManager>& assets, const Size dimen, std::shared_ptr<cugl::Node> root, std::shared_ptr<Moose> player, std::shared_ptr<Moose> opp, string costume, int levelNum) {
	root->removeAllChildren();

	//Set screen size
	screenHeight = dimen.height;
	screenWidth = dimen.width;

	playerGlobe = player;
	oppGlobe = opp;

	step = 0;

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

	retry = false;
	goHome = false;
	soundToggle = false;

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


	layer = assets->get<Node>("game");
	layer->setContentSize(dimen);
	layer->doLayout(); // This rearranges the children to fit the screen
	root->addChild(layer);

	//Create background node
	backCanvas = Node::alloc();
	layer->addChild(backCanvas);

	//init flapping booleans to 0
	for (int i = 0; i < 6; i++) {
		int f = 0;
		flappingFrame.push_back(f);
	}

	for (int i = 0; i < 5; i++) {
		int f = -1;
		pSmokeFrame.push_back(f);
		eSmokeFrame.push_back(f);
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
		stackButton->activate(i + 8);
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
					sInfoInd = i + 5;
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


	//reset drawing between frames
	backCanvas->removeAllChildren();
	mooseCanvas->removeAllChildren();
	frontCanvas->removeAllChildren();
	buttonCanvas->removeAllChildren();

	//Draw background
	std::shared_ptr<Texture> texturebg;
	if (levelNum == 1) {
		texturebg = _assets->get<Texture>("farmbg");
	}
	else if (levelNum == 2) {
		texturebg = _assets->get<Texture>("forestbg");
	}
	else if (levelNum == 3) {
		texturebg = _assets->get<Texture>("nuclearbg");
	}
	else {
		texturebg = _assets->get<Texture>("farmbg");
	}
	std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
	background->setScale(0.7f); // Magic number to rescale asset
	background->setAnchor(Vec2::ANCHOR_CENTER);
	background->setPosition(screenWidth / 2, screenHeight / 2);
	backCanvas->addChild(background);



	//Draw player moose

	std::shared_ptr<Texture> textureP = _assets->get<Texture>("moose");
	std::shared_ptr<PolygonNode> moose1 = PolygonNode::allocWithTexture(textureP);
	moose1->setScale(0.2f); // Magic number to rescale asset
	moose1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	moose1->setPosition(-MOOSE_X_OFFSET, MOOSE_HEIGHT);
	moose1->flipHorizontal(false);
	mooseCanvas->addChild(moose1);

	//Draw opponent moose
	std::shared_ptr<PolygonNode> moose2;
	std::shared_ptr<Texture> textureM;

	if (costume == "moose") {
		textureM = _assets->get<Texture>("moose");
	}
	else if (costume == "eldritch_moose") {
		textureM = _assets->get<Texture>("elMoose");
	}
	else if (costume == "christmoose") {
		textureM = _assets->get<Texture>("chrMoose");
	}
	else if (costume == "farmer_moose") {
		textureM = _assets->get<Texture>("farmMoose");
	}
	else {
		textureM = _assets->get<Texture>("moose");
	}

	moose2 = PolygonNode::allocWithTexture(textureM);
	moose2->setScale(0.2f); // Magic number to rescale asset
	moose2->setAnchor(Vec2::ANCHOR_BOTTOM_RIGHT);
	moose2->setPosition(screenWidth + MOOSE_X_OFFSET, MOOSE_HEIGHT);
	moose2->flipHorizontal(true);
	mooseCanvas->addChild(moose2);

	//Draw foreground
	std::shared_ptr<Texture> texturefg = _assets->get<Texture>("farmfg");
	std::shared_ptr<PolygonNode> foreground = PolygonNode::allocWithTexture(texturefg);
	foreground->setScale(0.7f); // Magic number to rescale asset
	foreground->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
	foreground->setPosition(screenWidth / 2, FORE_HEIGHT);
	frontCanvas->addChild(foreground);

	//Draw stack info
	std::shared_ptr<PolygonNode> sInfo = PolygonNode::allocWithTexture(infoF);
	sInfo->setScale(INFO_SCALE);
	sInfo->setAnchor(Vec2::ANCHOR_CENTER);
	sInfo->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
	stackInfoCanvas->addChild(sInfo);
	stackInfoCanvas->setVisible(false);

	//Draw info
	std::shared_ptr<PolygonNode> info = PolygonNode::allocWithTexture(infoF);
	info->setScale(INFO_SCALE);
	info->setAnchor(Vec2::ANCHOR_CENTER);
	info->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
	infoCanvas->addChild(info);
	infoCanvas->setVisible(false);


	//Init appropriately sized buttons
	for (int i = 0; i < 6; i++) {
		std::shared_ptr<Button> button;
		std::shared_ptr<Texture> text;
		text = textureW;

		std::shared_ptr<AnimationNode> id = AnimationNode::alloc(text, 1, CHICKEN_FILMSTRIP_LENGTH, CHICKEN_FILMSTRIP_LENGTH);
		id->setAnchor(Vec2::ANCHOR_CENTER);
		id->flipHorizontal(true);
		std::shared_ptr<Button> butt = Button::alloc(id);
		butt->setAnchor(Vec2::ANCHOR_CENTER);
		butt->setScale(HAND_SCALE, HAND_SCALE);

		butt->setAnchor(Vec2::ANCHOR_CENTER);

		//Draw shadows
		std::shared_ptr<PolygonNode> shadowNode = PolygonNode::allocWithTexture(shadow);
		shadowNode->setAnchor(Vec2::ANCHOR_CENTER);
		shadowNode->setScale(SHADOW_SCALE);
		shadowNode->setVisible(false);

		if (i < 3) {
			butt->setPosition((i - 1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET);
			shadowNode->setPosition((i - 1)*BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET - SHADOW_OFFSET);
		}
		else {
			butt->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING);
			shadowNode->setPosition((i - 4) * BUTTON_X_SPACING + BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING - SHADOW_OFFSET);
		}
		if (_input.isActive()) {
			//CULog("active");
		}

		if (i == 1 || i == 4) { //grass or spy chicken for tutorial
			butt->setListener([=](const std::string& name, bool down) {
				if (down) {
					heldButton = butt;
					if (timers[i] > 15) {
						infoCanvas->setVisible(true);
						if (step == 4) {
							tutbuttonpressed = true;
							advanceTutorial();
						}
					}
				}
				if (!down) {
					if (step == 5) {
						if (tutbuttonpressed) {
							tutbuttonpressed = false;
							advanceTutorial();
						}
						else {
							return;
						}
					}
					if (timers[i] < 15 && timers[i] > 1) {
						if (step == 0) advanceTutorial();
						playerGlobe->addToStackFromHand(handMap[i]);
						handMap[i] = -1;
						for (int j = i + 1; j < 6; j++) {
							handMap[j]--;
						}

						if (playerGlobe->getStack().getSize() == 5 && step == 6) {
							advanceTutorial();
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
			});
		}
		else {
			butt->setListener([=](const std::string& name, bool down) {
				if (down) {
					heldButton = butt;
					if (timers[i] > 15) {
						infoCanvas->setVisible(true);
					}

				}
				if (!down) {
					if (timers[i] < 15 && timers[i] > 1) {


						playerGlobe->addToStackFromHand(handMap[i]);
						handMap[i] = -1;
						for (int j = i + 1; j < 6; j++) {
							handMap[j]--;
						}

						if (playerGlobe->getStack().getSize() == 5 && step == 6) {
							advanceTutorial();
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
			});
		}

		buttonCanvas->addChild(shadowNode);
		buttonCanvas->addChild(butt);
		//i+2 to ensure keys are unique
		//butt->activate(i + 2);
		//CULog("Button %d made", i);
		buttons.push_back(butt);
		buttonTextures.push_back(id);
		texturesHand.push_back(text);

	}

	//Init Tutorial glows
	grasslightcanvas = AnimationNode::alloc(grasslight, 1, GRASS_FILMSTRIP_LENGTH, GRASS_FILMSTRIP_LENGTH);
	grasslightcanvas->setAnchor(Vec2::ANCHOR_CENTER);
	grasslightcanvas->setPosition(BUTTON_X_OFFSET, BUTTON_Y_OFFSET);
	grasslightcanvas->setScale(HAND_SCALE, HAND_SCALE);
	buttonCanvas->addChild(grasslightcanvas); //child 12

	spylightcanvas = AnimationNode::alloc(spylight, 1, SPY_FILMSTRIP_LENGTH, SPY_FILMSTRIP_LENGTH);
	spylightcanvas->setAnchor(Vec2::ANCHOR_CENTER);
	spylightcanvas->setPosition(BUTTON_X_OFFSET, BUTTON_Y_OFFSET + BUTTON_Y_SPACING);
	spylightcanvas->setScale(HAND_SCALE, HAND_SCALE);
	buttonCanvas->addChild(spylightcanvas); //child 13
	spylightcanvas->setVisible(false);

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

	//Init the clash preview button

	std::shared_ptr<Texture> textureFight = _assets->get<Texture>("preview");

	std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(textureFight);
	id->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> butt = Button::alloc(id);
	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setScale(0.4, 0.4);

	butt->setAnchor(Vec2::ANCHOR_CENTER);
	butt->setPosition(0, screenHeight / 3);
	butt->setListener([=](const std::string& name, bool down) {
		if (down) {
			//previewSet = true;
			prevTint = true;
			if (step == 1) advanceTutorial();
		}
		if (!down) {
			prevTint = false;
			if (step == 2) advanceTutorial();
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
		playerB->setPosition(screenWidth / 2 - BAR_DISTANCE / 2 - (i*HEALTH_BLOCK_SPACING), healthYScale);
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
	grassNum->setPosition(screenWidth / 2 + ELT_NUM_X_OFFSET, healthYScale - ELT_Y_OFFSET - ELT_NUM_Y_OFFSET - (2 * ELT_NUM_SPACING));
	eltInfoCanvas->addChild(grassNum);

	//Init the tutorial buttons
	tutcanvas1 = AnimationNode::alloc(tutor1, 1, TUTOR1_LENGTH, TUTOR1_LENGTH);
	tutcanvas1->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
	tutcanvas1->setScale(0.65f,0.65f);
	layer->addChild(tutcanvas1);


	tutcanvas2 = PolygonNode::allocWithTexture(tutor2);
	tutcanvas2->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
	tutcanvas2->setScale(screenWidth/tutcanvas2->getWidth(), screenWidth / tutcanvas2->getWidth());
	layer->addChild(tutcanvas2);
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
	layer->addChild(tutbutton3);
	tutbutton3->setVisible(false);

	tutcanvas4 = PolygonNode::allocWithTexture(tutor4);
	tutcanvas4->setAnchor(Vec2::ANCHOR_TOP_LEFT);
	tutcanvas4->setScale(screenWidth / tutcanvas4->getWidth(), screenWidth / tutcanvas4->getWidth());
	layer->addChild(tutcanvas4);
	tutcanvas4->setVisible(false);


	tutcanvas5 = PolygonNode::allocWithTexture(tutor5);
	tutbutton5 = Button::alloc(tutcanvas5);
	tutbutton5->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	tutbutton5->setScale(screenWidth / tutbutton5->getWidth(), screenWidth / tutbutton5->getWidth());
	tutbutton5->setListener([=](const std::string& name, bool down) {
		if (step == 5) {
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
	layer->addChild(tutbutton5);
	tutbutton5->setVisible(false);
	
	tutcanvas6 = AnimationNode::alloc(tutor6, 1, TUTOR6_LENGTH, TUTOR6_LENGTH);
	tutbutton6 = Button::alloc(tutcanvas6);
	tutbutton6->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	tutbutton6->setScale(screenWidth / tutbutton6->getWidth(), screenWidth / tutbutton6->getWidth());
	tutbutton6->setListener([=](const std::string& name, bool down) {
		if (step == 7) {
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
	layer->addChild(tutbutton6);
	tutbutton6->setVisible(false);
	
	tutcanvas7 = PolygonNode::allocWithTexture(tutor7);
	tutbutton7 = Button::alloc(tutcanvas7);
	tutbutton7->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
	tutbutton7->setScale(screenWidth / tutbutton7->getWidth(), screenWidth / tutbutton7->getWidth());
	tutbutton7->setListener([=](const std::string& name, bool down) {
		if (step == 9) {
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
	layer->addChild(tutbutton7);
	tutbutton7->setVisible(false);

	//Init the pause button
	std::shared_ptr<Texture> texturePause = _assets->get<Texture>("pausebutton");
	std::shared_ptr<PolygonNode> pauseid = PolygonNode::allocWithTexture(texturePause);
	pauseid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pausebutt = Button::alloc(pauseid);
	pausebutt->setScale(0.25, 0.25);
	pausebutt->setAnchor(Vec2::ANCHOR_CENTER);
	pausebutt->setPosition(screenWidth / 2, healthYScale);
	pausebutt->setListener([=](const std::string& name, bool down) {
		if (down) {
			pauseMenuCanvas->setVisible(true);
			deactivateHand(); //@TODO: freeze game state??
			activatePause();
		}
	});
	pauseButtonCanvas->addChild(pausebutt);
	pausebutt->activate(200); //ensure keys are unique

	//Draw pause box
	std::shared_ptr<Texture> texturePauseOverlay = _assets->get<Texture>("pauseoverlay");
	std::shared_ptr<PolygonNode> pauseOverlay = PolygonNode::allocWithTexture(texturePauseOverlay);
	pauseOverlay->setAnchor(Vec2::ANCHOR_CENTER);
	pauseOverlay->setPosition(screenWidth / 2, screenHeight / 2);
	pauseMenuCanvas->addChild(pauseOverlay);

	std::shared_ptr<Texture> texturePauseBox = _assets->get<Texture>("pausebox");
	std::shared_ptr<PolygonNode> pauseBox = PolygonNode::allocWithTexture(texturePauseBox);
	pauseBox->setScale(0.7, 0.7);
	pauseBox->setAnchor(Vec2::ANCHOR_CENTER);
	pauseBox->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2);
	pauseMenuCanvas->addChild(pauseBox);

	std::shared_ptr<Texture> texturePauseRestart = _assets->get<Texture>("pauserestart");
	std::shared_ptr<PolygonNode> pauserestartid = PolygonNode::allocWithTexture(texturePauseRestart);
	pauserestartid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pauseRestart = Button::alloc(pauserestartid);
	pauseRestart->setScale(0.65, 0.65);
	pauseRestart->setAnchor(Vec2::ANCHOR_CENTER);
	pauseRestart->setPosition(screenWidth / 4, screenHeight / 2 + 50);
	pauseRestart->setListener([=](const std::string& name, bool down) { if (down) { retry = true; }});
	pauseMenuCanvas->addChild(pauseRestart);
	pauseRestart->activate(201); //ensure keys are unique
	pausebuttons.push_back(pauseRestart);

	std::shared_ptr<Texture> texturePauseHome = _assets->get<Texture>("pausehome");
	std::shared_ptr<PolygonNode> pausehomeid = PolygonNode::allocWithTexture(texturePauseHome);
	pausehomeid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pauseHome = Button::alloc(pausehomeid);
	pauseHome->setScale(0.65, 0.65);
	pauseHome->setAnchor(Vec2::ANCHOR_CENTER);
	pauseHome->setPosition(screenWidth / 2, screenHeight / 2 + 50);
	pauseHome->setListener([=](const std::string& name, bool down) { if (down) { goHome = true; }});
	pauseMenuCanvas->addChild(pauseHome);
	pauseHome->activate(202); //ensure keys are unique
	pausebuttons.push_back(pauseHome);

	std::shared_ptr<Texture> texturePauseSettings = _assets->get<Texture>("pausesoundoff");
	std::shared_ptr<PolygonNode> pausesettingsid = PolygonNode::allocWithTexture(texturePauseSettings);
	pausesettingsid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pauseSettings = Button::alloc(pausesettingsid);
	pauseSettings->setScale(0.65, 0.65);
	pauseSettings->setAnchor(Vec2::ANCHOR_CENTER);
	pauseSettings->setPosition(screenWidth * 3 / 4, screenHeight / 2 + 50);
	pauseSettings->setListener([=](const std::string& name, bool down) { if (down) { soundToggle = soundToggle ? false : true; }});
	pauseMenuCanvas->addChild(pauseSettings);
	pauseSettings->activate(203); //ensure keys are unique
	pausebuttons.push_back(pauseSettings);

	std::shared_ptr<Texture> texturePauseResume = _assets->get<Texture>("pauseresume");
	std::shared_ptr<PolygonNode> pauseresumeid = PolygonNode::allocWithTexture(texturePauseResume);
	pauseresumeid->setAnchor(Vec2::ANCHOR_CENTER);
	std::shared_ptr<Button> pauseResume = Button::alloc(pauseresumeid);
	pauseResume->setScale(0.65, 0.65);
	pauseResume->setAnchor(Vec2::ANCHOR_CENTER);
	pauseResume->setPosition(screenWidth / 2, screenHeight / 2 - INFO_Y_OFFSET);
	pauseResume->setListener([=](const std::string& name, bool down) { if (down) {
		pauseMenuCanvas->setVisible(false);
		activateHand(); //@TODO: freeze game state
		deactivatePause();
	}});
	pauseMenuCanvas->addChild(pauseResume);
	pauseResume->activate(204); //ensure keys are unique
	pausebuttons.push_back(pauseResume);

	pauseMenuCanvas->setVisible(false);

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

	return true;
}

void TutorialSB::updateGameScene(float timestep) {
	timeAmount += timestep;
	bool isNextFrame = (timeAmount > timeBtnFrames);
	if (timeAmount > timeBtnFrames)
	{
		isNextFrame = true;
		timeAmount = 0;
		if (thisFrame >= CHICKEN_FILMSTRIP_LENGTH - 1)
			thisFrame = 0;
		thisFrame++;
	}

	vector <Chicken> hand = playerGlobe->getHand();

	if (playerGlobe->getHand().size() == 6) {
		handMap.clear();
		for (int i = 0; i < 6; i++) {
			handMap.push_back(i);
		}
	}

	if (step == 0) { //only grass chicken
		for (int i = 0; i < 6; i++) {
			buttons[i]->setVisible(true);
			buttonCanvas->getChild(2 * i)->setVisible(true);
			if (i == 1) buttons[i]->activate(i + 2);
			if (buttons[i] == heldButton) {
				heldButtInd = i;
				std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoG);
				newPoly->setVisible(false);
				infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
			}
		}
	}
	else if (step == 4) {
		for (int i = 0; i < 6; i++) {
			if (handMap[i] >= 0) {
				buttons[i]->setVisible(true);
				buttonCanvas->getChild(2 * i)->setVisible(true);
				if (i == 4) buttons[i]->activate(i + 2);
				if (buttons[i] == heldButton) {
					heldButtInd = i;
					if (i == 4) {
						std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoSpy);
						newPoly->setScale(INFO_SCALE);
						newPoly->setAnchor(Vec2::ANCHOR_CENTER);
						newPoly->setPosition(screenWidth / 2 + INFO_X_OFFSET, screenHeight / 2 + INFO_Y_OFFSET);
						infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
					}
					else {
						std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoG);
						newPoly->setVisible(false);
						infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
					}
				}
			}
			else {
				buttons[i]->setVisible(false);
				buttonCanvas->getChild(2 * i)->setVisible(false);
				buttons[i]->deactivate();
			}

		}
	}
	else {
		for (int i = 0; i < 6; i++) {
			if (handMap[i] >= 0) {
				buttons[i]->setVisible(true);
				buttonCanvas->getChild(2 * i)->setVisible(true);
				if (buttons[i] == heldButton) {
					heldButtInd = i;
					std::shared_ptr<PolygonNode> newPoly = PolygonNode::allocWithTexture(infoG);
					newPoly->setVisible(false);
					infoCanvas->swapChild(infoCanvas->getChild(0), newPoly, false);
				}
			}
			else {
				buttons[i]->setVisible(false);
				buttonCanvas->getChild(2 * i)->setVisible(false);
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

	if (sInfoInd != -1) {
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
		for (int j = 0; j < 6; j++) {
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

		if (isChange) {
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

		if (isChange) {
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
	}

	//Tutorial Animations
	if (tutcanvas1->isVisible() && isNextFrame) {
		int next = tutcanvas1->getFrame() + 1;
		if (next == TUTOR1_LENGTH) next = 0;
		tutcanvas1->setFrame(next);
	}
	if (tutcanvas6->isVisible() && isNextFrame) {
		int next = tutcanvas6->getFrame() + 1;
		if (next == TUTOR6_LENGTH) next = 0;
		tutcanvas6->setFrame(next);
	}
	

	//Update the info card
	if (heldButton == nullptr) {
		infoCanvas->setVisible(false);
	}
	else {
		if (timers[heldButtInd] > 15) {
			infoCanvas->setVisible(true);
		}
	}

	//Update the opponent health bar
	for (int i = 1; i < 6; i++) {
		if (oppGlobe->getHealth() < i) {
			std::shared_ptr<Node> child = healthCanvas->getChild(11 - i);
			child->setVisible(false);
		}
	}

	//Update the player health bar
	for (int i = 1; i < 6; i++) {
		if (playerGlobe->getHealth() < i) {
			std::shared_ptr<Node> child = healthCanvas->getChild(6 - i);
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

	//Update win and loss screens
	if (playerGlobe->getHealth() < 1) {
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
				}
			});
			loseCanvas->addChild(hButtL);
			//ensure keys are unique
			hButtL->activate(53);

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
				}
			});
			loseCanvas->addChild(rButtL);
			//ensure keys are unique
			rButtL->activate(54);
		}
		hasLost = true;
	}
	else if (oppGlobe->getHealth() < 1) {
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
				}
			});
			winCanvas->addChild(hButt);
			//ensure keys are unique
			hButt->activate(50);

			//Init the win redo button
			std::shared_ptr<PolygonNode> redoPoly = PolygonNode::allocWithTexture(redo);
			redoPoly->setAnchor(Vec2::ANCHOR_CENTER);
			std::shared_ptr<Button> rButt = Button::alloc(redoPoly);
			rButt->setAnchor(Vec2::ANCHOR_CENTER);
			rButt->setScale(WIN_LOSS_B_SCALE);
			rButt->setAnchor(Vec2::ANCHOR_CENTER);
			rButt->setPosition(screenWidth / 2 - WIN_BUTTON_X_SPACING, SCENE_HEIGHT / 2 + WIN_LOSS_Y_OFFSET + WIN_LOSS_B_Y_OFFSET);
			rButt->setListener([=](const std::string& name, bool down) {
				if (down) {
					retry = true;
				}
			});
			winCanvas->addChild(rButt);
			//ensure keys are unique
			rButt->activate(51);

			//Init the win next level button
			std::shared_ptr<PolygonNode> levelPoly = PolygonNode::allocWithTexture(nextlvl);
			levelPoly->setAnchor(Vec2::ANCHOR_CENTER);
			std::shared_ptr<Button> lButt = Button::alloc(levelPoly);
			lButt->setAnchor(Vec2::ANCHOR_CENTER);
			lButt->setScale(WIN_LOSS_B_SCALE);
			lButt->setAnchor(Vec2::ANCHOR_CENTER);
			lButt->setPosition(screenWidth / 2 + WIN_BUTTON_X_SPACING, SCENE_HEIGHT / 2 + WIN_LOSS_Y_OFFSET + WIN_LOSS_B_Y_OFFSET);
			lButt->setListener([=](const std::string& name, bool down) {
				if (down) {
					nextLevel = true;
				}
			});
			winCanvas->addChild(lButt);
			//ensure keys are unique
			lButt->activate(52);
		}
		hasWon = true;
	}
	else {

	}
}

void TutorialSB::advanceTutorial() {
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
		buttons[4]->activate(6);
	}
	else if (step == 4) {
		//Bug with hand map and spy rip
		//5. Spy card seen.Stop showing Tutorial 4.  stop highlighting spy.
		tutcanvas4->setVisible(false);
		spylightcanvas->setVisible(false);
	}
	else if (step == 5) {
		//6. Spy card let go.Show tutorial 5.
		tutbutton5->setVisible(true);
		tutbutton5->activate(405);
	}
	else if (step == 6) {
		//7. Tutorial 5 tapped.Stop showing tutorial 5. Wait till stack size reaches 5
		tutbutton5->deactivate();
	}
	else if (step == 7) {
		//8. Stack size reached 5.  Show tutorial 6.
		tutbutton6->setVisible(true);
		tutbutton6->activate(406);
	}
	else if (step == 8) {
		//9. Tutorial 6 tapped.stop showing tutorial 6.  Wait for clash animation to end.
		//Need enlist clashers for step 7-9? make clash not immediately occur.
		tutbutton6->setVisible(false);
	}
	else if (step == 9) {
		//10. Showing Tutorial 7.
		tutbutton7->setVisible(true);
		tutbutton7->activate(407);

	}
	else if (step == 10) {
		//11. Tutorial 7 tapped.ending tutorial.
		tutbutton7->setVisible(false);
		step = -2;
		//Insert end tutorial stuff here
	}
	step++;
}
