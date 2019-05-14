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
#include "CCMMenuScene.h"

using namespace cugl;

/** The ID for the button listener */
#define LISTENER_ID 3
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 576
#define SCENE_HEIGHT 1024

//#define TITLE_HEIGHT 600 //

// BGM for the game
#define MUSIC_THEME			"theme"

// Sfx for the game
#define SOUND_BUTTON_A		"button_a"
#define SOUND_BUTTON_B		"button_b"

//Main canvas to draw stuff to
std::shared_ptr<Node> menulayer;

//Canvas for background
std::shared_ptr<Node> menubackCanvas;

//Canvas for title
std::shared_ptr<Node> titleCanvas;

//Canvas for credits
std::shared_ptr<Node> creditsCanvas;

//Canvas for credits
std::shared_ptr<Node> helpCanvas;

//Canvas for buttons
std::shared_ptr<Node> menubuttonCanvas;

// List for credits buttons
std::vector<std::shared_ptr<Button>> creditsbuttons;

// List for help buttons
std::vector<std::shared_ptr<Button>> helpbuttons;

// List for non-credits buttons
std::vector<std::shared_ptr<Button>> menubuttons;


//Preview tracking
bool playClicked;
bool helpClicked;
bool helpRightClicked;
bool helpLeftClicked;
bool helpBackClicked;
bool creditsClicked;
bool creditsBackClicked;
int page; // help page #

//Screen dimensions
float menuscreenHeight;
float menuscreenWidth;

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
bool MenuScene::init(const std::shared_ptr<AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = computeActiveSize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }
    
    //Set screen size
    menuscreenHeight = dimen.height;
    menuscreenWidth = dimen.width;

    //Root node for scene builder
    std::shared_ptr<Node> root;

    root = Node::alloc();
    addChild(root);

    root->removeAllChildren();

    _assets = assets;

	//auto game_music = _assets->get<Sound>(MUSIC_THEME);
	//AudioChannels::get()->queueMusic(game_music, true, game_music->getVolume());

    menulayer = assets->get<Node>("menu");
    menulayer->setContentSize(dimen);
    menulayer->doLayout(); // This rearranges the children to fit the screen
    root->addChild(menulayer);
    
    playClicked = false;
    helpClicked = false;
    helpRightClicked = false;
    helpLeftClicked = false;
    helpBackClicked = false;
    creditsClicked = false;
    creditsBackClicked = false;
    page = 1;

    //Create background node
    menubackCanvas = Node::alloc();
    menulayer->addChild(menubackCanvas);

    //Create title node
    titleCanvas = Node::alloc();
    menulayer->addChild(titleCanvas);

    //Add button canvas
    menubuttonCanvas = Node::alloc();
    menulayer->addChild(menubuttonCanvas);
    menubuttonCanvas->setPosition(menuscreenWidth / 2, 150);
    
    //Create credits node
    creditsCanvas = Node::alloc();
    menulayer->addChild(creditsCanvas);
    
    //Create help node
    helpCanvas = Node::alloc();
    menulayer->addChild(helpCanvas);

    //reset drawing between frames
    menubackCanvas->removeAllChildren();
    titleCanvas->removeAllChildren();
    menubuttonCanvas->removeAllChildren();
    creditsCanvas->removeAllChildren();
    helpCanvas->removeAllChildren();

    //Draw background
    std::shared_ptr<Texture> texturebg = _assets->get<Texture>("menubg");
    std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
    background->setScale(0.65f); // Magic number to rescale asset
    background->setAnchor(Vec2::ANCHOR_CENTER);
    background->setPosition(menuscreenWidth/2, menuscreenHeight/2);
    menubackCanvas->addChild(background);

    //Draw title
    std::shared_ptr<Texture> texturetitle = _assets->get<Texture>("menutitle");
    std::shared_ptr<PolygonNode> title = PolygonNode::allocWithTexture(texturetitle);
    title->setScale(0.55f); // Magic number to rescale asset
    title->setAnchor(Vec2::ANCHOR_BOTTOM_CENTER);
    title->setPosition(menuscreenWidth/2, menuscreenHeight*5/8);
    titleCanvas->addChild(title);

    // Play button
    std::shared_ptr<Texture> texturePlay = _assets->get<Texture>("menuplay");
    std::shared_ptr<PolygonNode> idplay = PolygonNode::allocWithTexture(texturePlay);
    idplay->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> playbutt = Button::alloc(idplay);
    playbutt->setAnchor(Vec2::ANCHOR_CENTER);
    playbutt->setScale(0.4, 0.4);

    playbutt->setAnchor(Vec2::ANCHOR_CENTER);
    playbutt->setPosition(0, menuscreenHeight/3);
    playbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
			playButtonSound();
            playClicked = true;
        }
    });

    // Help button
    std::shared_ptr<Texture> textureHelp = _assets->get<Texture>("menuhelp");
    std::shared_ptr<PolygonNode> idhelp = PolygonNode::allocWithTexture(textureHelp);
    idhelp->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> helpbutt = Button::alloc(idhelp);
    helpbutt->setAnchor(Vec2::ANCHOR_CENTER);
    helpbutt->setScale(0.4, 0.4);

    helpbutt->setAnchor(Vec2::ANCHOR_CENTER);
    helpbutt->setPosition(0, menuscreenHeight/3 - 100);
    helpbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
			playButtonSound();
            helpClicked = true;
        }
    });

    // Credits button
    std::shared_ptr<Texture> textureCredits = _assets->get<Texture>("menucredits");
    std::shared_ptr<PolygonNode> idcredits = PolygonNode::allocWithTexture(textureCredits);
    idcredits->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> creditsbutt = Button::alloc(idcredits);
    creditsbutt->setAnchor(Vec2::ANCHOR_CENTER);
    creditsbutt->setScale(0.21, 0.21);

    creditsbutt->setAnchor(Vec2::ANCHOR_CENTER);
    creditsbutt->setPosition(0, menuscreenHeight/3 - 200);
    creditsbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
			playButtonSound();
            creditsClicked = true;
        }
    });

    menubuttonCanvas->addChild(playbutt);
    menubuttonCanvas->addChild(helpbutt);
    menubuttonCanvas->addChild(creditsbutt);

    //ensure keys are unique
    playbutt->activate(100);
    menubuttons.push_back(playbutt); // 0
    helpbutt->activate(101);
    menubuttons.push_back(helpbutt); // 1
    creditsbutt->activate(102);
    menubuttons.push_back(creditsbutt); // 2
    
    std::shared_ptr<Texture> creditsbg = _assets->get<Texture>("levelbg");
    std::shared_ptr<PolygonNode> creditbackground = PolygonNode::allocWithTexture(creditsbg);
    creditbackground->setScale(0.65f); // Magic number to rescale asset
    creditbackground->setAnchor(Vec2::ANCHOR_CENTER);
    creditbackground->setPosition(menuscreenWidth/2, menuscreenHeight/2);
    creditsCanvas->addChild(creditbackground);
    
    //Draw credits box
    std::shared_ptr<Texture> texturebox = _assets->get<Texture>("creditsbox");
    std::shared_ptr<PolygonNode> box = PolygonNode::allocWithTexture(texturebox);
    box->setScale(0.5f); // Magic number to rescale asset
    box->setAnchor(Vec2::ANCHOR_CENTER);
    box->setPosition(menuscreenWidth/2, menuscreenHeight/2);
    creditsCanvas->addChild(box);
    
    // credits back button
    std::shared_ptr<Texture> textureBack = _assets->get<Texture>("levelback");
    std::shared_ptr<PolygonNode> idback = PolygonNode::allocWithTexture(textureBack);
    idback->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> backbutt = Button::alloc(idback);
    backbutt->setAnchor(Vec2::ANCHOR_CENTER);
    backbutt->setScale(0.5, 0.5);
    backbutt->setAnchor(Vec2::ANCHOR_CENTER);
    backbutt->setPosition(menuscreenWidth/8, menuscreenHeight*7/8);
    backbutt->setListener([=](const std::string& name, bool down) {
        if (down) {creditsBackClicked = true;}});
    creditsCanvas->addChild(backbutt);
    backbutt->activate(103);
    creditsbuttons.push_back(backbutt);
    
    creditsCanvas->setVisible(false);
    creditsbuttons[0]->deactivate();
    
    // help bg
    std::shared_ptr<Texture> helpbg = _assets->get<Texture>("helpbg");
    std::shared_ptr<PolygonNode> helpbackground = PolygonNode::allocWithTexture(helpbg);
    helpbackground->setScale(0.65f); // Magic number to rescale asset
    helpbackground->setAnchor(Vec2::ANCHOR_CENTER);
    helpbackground->setPosition(menuscreenWidth/2, menuscreenHeight/2);
    helpCanvas->addChild(helpbackground);
    
    //Draw help 1
    std::shared_ptr<Texture> texturehelp = _assets->get<Texture>("help1");
    std::shared_ptr<PolygonNode> help = PolygonNode::allocWithTexture(texturehelp);
    help->setScale(0.5f); // Magic number to rescale asset
    help->setAnchor(Vec2::ANCHOR_CENTER);
    help->setPosition(menuscreenWidth/2, menuscreenHeight/2);
    helpCanvas->addChild(help);
    
    // next button
    std::shared_ptr<Texture> texthelpright = _assets->get<Texture>("helpright");
    std::shared_ptr<PolygonNode> idhelpright = PolygonNode::allocWithTexture(texthelpright);
    idhelpright->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> helpright = Button::alloc(idhelpright);
    helpright->setAnchor(Vec2::ANCHOR_CENTER);
    helpright->setScale(0.5, 0.5);
    helpright->setAnchor(Vec2::ANCHOR_CENTER);
    helpright->setPosition(menuscreenWidth/2 + 75, menuscreenHeight/2 - 375);
    helpright->setListener([=](const std::string& name, bool down) {
        if (down) { helpRightClicked = true; }});
    helpCanvas->addChild(helpright);
    helpright->activate(120);
    helpbuttons.push_back(helpright); // 0
    
    // prev button
    std::shared_ptr<Texture> texthelpleft = _assets->get<Texture>("helpleft");
    std::shared_ptr<PolygonNode> idhelpleft = PolygonNode::allocWithTexture(texthelpleft);
    idhelpleft->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> helpleft = Button::alloc(idhelpleft);
    helpleft->setAnchor(Vec2::ANCHOR_CENTER);
    helpleft->setScale(0.5, 0.5);
    helpleft->setAnchor(Vec2::ANCHOR_CENTER);
    helpleft->setPosition(menuscreenWidth/2 - 75, menuscreenHeight/2 - 375);
    helpleft->setListener([=](const std::string& name, bool down) {
        if (down) { helpLeftClicked = true; }});
    helpCanvas->addChild(helpleft);
    helpleft->activate(121);
    helpbuttons.push_back(helpleft); // 1
    
    // help back button
    std::shared_ptr<Texture> texthelpback = _assets->get<Texture>("levelback");
    std::shared_ptr<PolygonNode> idhelpback = PolygonNode::allocWithTexture(texthelpback);
    idhelpback->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> helpback = Button::alloc(idhelpback);
    helpback->setAnchor(Vec2::ANCHOR_CENTER);
    helpback->setScale(0.5, 0.5);
    helpback->setAnchor(Vec2::ANCHOR_CENTER);
    helpback->setPosition(menuscreenWidth/8, menuscreenHeight*7/8);
    helpback->setListener([=](const std::string& name, bool down) {
        if (down) {helpBackClicked = true;}});
    helpCanvas->addChild(helpback);
    helpback->activate(122);
    helpbuttons.push_back(helpback); // 2
    
    helpCanvas->setVisible(false);
    helpbuttons[0]->deactivate();
    helpbuttons[1]->deactivate();
    helpbuttons[2]->deactivate();

    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MenuScene::dispose() {
    _assets = nullptr;
    _buttons.clear();
    Scene::dispose();

	//Stop playing audio
	AudioChannels::get()->stopMusic();
}

void MenuScene::update(float timestep) {
    if (playClicked) { // this doesn't seem to do anything
        playClicked = false;
    }
    if (creditsClicked){
        creditsClicked = false;
        deactivateButtons();
        creditsCanvas->setVisible(true);
        creditsbuttons[0]->activate(103);
    }
    else if (creditsBackClicked){
        creditsBackClicked = false;
        activateButtons();
        creditsCanvas->setVisible(false);
        creditsbuttons[0]->deactivate();
    }
    if (helpClicked){
        helpClicked = false;
        activateHelp();
    }
    if (helpBackClicked){
        helpBackClicked = false;
        deactivateHelp();
    }
    if (helpRightClicked || helpLeftClicked){
        if (page < 5 && helpRightClicked) { page++; }
        if (page > 1 && helpLeftClicked) { page--; }
        if (helpRightClicked) { helpRightClicked = false; }
        if (helpLeftClicked) { helpLeftClicked = false; }
        
        stringstream ssnum;
        ssnum << "help" << page;
        string helppage = ssnum.str();
        
        CULog("help page %d", page);
        
        std::shared_ptr<Texture> texturehelp = _assets->get<Texture>(helppage);
        std::shared_ptr<PolygonNode> help = PolygonNode::allocWithTexture(texturehelp);
        help->setScale(0.5f); // Magic number to rescale asset
        help->setAnchor(Vec2::ANCHOR_CENTER);
        help->setPosition(menuscreenWidth/2, menuscreenHeight/2);
        helpCanvas->addChild(help);
    }
}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void MenuScene::setActive(bool value) {
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

void MenuScene::playButtonSound() {
	//Play the button sfx
	string sfx = rand() % 2 ? SOUND_BUTTON_A : SOUND_BUTTON_B;
	auto source = _assets->get<Sound>(sfx);
	if (!AudioChannels::get()->isActiveEffect(SOUND_BUTTON_A) && !AudioChannels::get()->isActiveEffect(SOUND_BUTTON_B)) {
		AudioChannels::get()->playEffect(sfx, source, false, source->getVolume());
	}
}

void MenuScene::activateButtons() {
//    pauseMenuCanvas->setVisible(true);
    for (int i = 0; i < menubuttons.size(); i++) {
        menubuttons[i]->activate(100 + i);
    }
}

void MenuScene::deactivateButtons() {
//    pauseMenuCanvas->setVisible(false);
    for (int i = 0; i < menubuttons.size(); i++) {
        menubuttons[i]->deactivate();
    }
}

bool MenuScene::getPlay() { return playClicked; }
void MenuScene::setPlay(bool val) { playClicked = val; }

void MenuScene::activateHelp() {
    helpCanvas->setVisible(true);
    for (int i = 0; i < helpbuttons.size(); i++) {
        helpbuttons[i]->activate(120 + i);
    }
}

void MenuScene::deactivateHelp() {
    helpCanvas->setVisible(false);
    for (int i = 0; i < helpbuttons.size(); i++) {
        helpbuttons[i]->deactivate();
    }
}


Size MenuScene::computeActiveSize() const {
    Size dimen = Application::get()->getDisplaySize();
    //float ratio1 = dimen.width / dimen.height;
    //float ratio2 = ((float)SCENE_WIDTH) / ((float)SCENE_HEIGHT);
    
    //dimen *= SCENE_WIDTH / dimen.width;
    
    dimen *= SCENE_HEIGHT / dimen.height;
    return dimen;
}
