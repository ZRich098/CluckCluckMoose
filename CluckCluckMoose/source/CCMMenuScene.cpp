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

//Canvas for buttons
std::shared_ptr<Node> menubuttonCanvas;


//Preview tracking
bool playClicked;
bool helpClicked;
bool settingsClicked;

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

    //reset drawing between frames
    menubackCanvas->removeAllChildren();
    titleCanvas->removeAllChildren();
    menubuttonCanvas->removeAllChildren();

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

    // Settings button
    std::shared_ptr<Texture> textureSettings = _assets->get<Texture>("menusettings");
    std::shared_ptr<PolygonNode> idsettings = PolygonNode::allocWithTexture(textureSettings);
    idsettings->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> settingsbutt = Button::alloc(idsettings);
    settingsbutt->setAnchor(Vec2::ANCHOR_CENTER);
    settingsbutt->setScale(0.4, 0.4);

    settingsbutt->setAnchor(Vec2::ANCHOR_CENTER);
    settingsbutt->setPosition(0, menuscreenHeight/3 - 200);
    settingsbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
			playButtonSound();
            settingsClicked = true;
        }
    });

    menubuttonCanvas->addChild(playbutt);
    menubuttonCanvas->addChild(helpbutt);
    menubuttonCanvas->addChild(settingsbutt);

    //ensure keys are unique
    playbutt->activate(100);
    helpbutt->activate(101);
    settingsbutt->activate(102);

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

bool MenuScene::getPlay() { return playClicked; }
void MenuScene::setPlay(bool val) { playClicked = val; }
bool MenuScene::getHelp() { return helpClicked; }
bool MenuScene::getSettings() { return settingsClicked; }

Size MenuScene::computeActiveSize() const {
    Size dimen = Application::get()->getDisplaySize();
    //float ratio1 = dimen.width / dimen.height;
    //float ratio2 = ((float)SCENE_WIDTH) / ((float)SCENE_HEIGHT);
    
    //dimen *= SCENE_WIDTH / dimen.width;
    
    dimen *= SCENE_HEIGHT / dimen.height;
    return dimen;
}
