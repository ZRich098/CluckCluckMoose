//
// Created by Amanda Trang on 4/12/19.
//

#include "CCMLevelScene.h"

using namespace cugl;

/** The ID for the button listener */
#define LISTENER_ID 4
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 576
#define SCENE_HEIGHT 1024

/** The number of levels in game--used for activation and deactivation */
#define LEVELS 4

//Main canvas to draw stuff to
std::shared_ptr<Node> levellayer;

//Canvas for background
std::shared_ptr<Node> levelbackCanvas;

//Canvas for buttons
std::shared_ptr<Node> levelbuttonCanvas;

// List for buttons
std::vector<std::shared_ptr<Button>> levelbuttons;

//List of level nodes
std::vector<std::shared_ptr<cugl::PolygonNode>> levelNodes;

//Level tracking
bool backClicked;
int level;

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
bool LevelScene::init(const std::shared_ptr<AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }

    //Root node for scene builder
    std::shared_ptr<Node> root;
    root = Node::alloc();
    addChild(root);
    root->removeAllChildren();

    level = 0;
    backClicked = false;
    _assets = assets;

    levellayer = assets->get<Node>("levelselect");
    levellayer->setContentSize(dimen);
    levellayer->doLayout(); // This rearranges the children to fit the screen
    root->addChild(levellayer);

    //Create background node
    levelbackCanvas = Node::alloc();
    levellayer->addChild(levelbackCanvas);

    //Add button canvas
    levelbuttonCanvas = Node::alloc();
    levellayer->addChild(levelbuttonCanvas);
    levelbuttonCanvas->setPosition(SCENE_WIDTH / 2, 150);

    //reset drawing between frames
    levelbackCanvas->removeAllChildren();
    levelbuttonCanvas->removeAllChildren();

    //Draw background
    std::shared_ptr<Texture> texturebg = _assets->get<Texture>("levelbg");
    std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
    background->setScale(0.55f); // Magic number to rescale asset
    background->setAnchor(Vec2::ANCHOR_CENTER);
    background->setPosition(SCENE_WIDTH/2, SCENE_HEIGHT/2);
    levelbackCanvas->addChild(background);

    //Draw level box
    std::shared_ptr<Texture> texturebox = _assets->get<Texture>("levelbox");
    std::shared_ptr<PolygonNode> box = PolygonNode::allocWithTexture(texturebox);
    box->setScale(0.5f); // Magic number to rescale asset
    box->setAnchor(Vec2::ANCHOR_CENTER);
    box->setPosition(SCENE_WIDTH/2, SCENE_HEIGHT/2);
    levelbackCanvas->addChild(box);

    // back button
    std::shared_ptr<Texture> textureBack = _assets->get<Texture>("levelback");
    std::shared_ptr<PolygonNode> idback = PolygonNode::allocWithTexture(textureBack);
    idback->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> backbutt = Button::alloc(idback);
    backbutt->setAnchor(Vec2::ANCHOR_CENTER);
    backbutt->setScale(0.5, 0.5);
    backbutt->setAnchor(Vec2::ANCHOR_CENTER);
    backbutt->setPosition(-SCENE_WIDTH*3/8, SCENE_HEIGHT*3/4);
    backbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
            backClicked = true;
            level = 0; }});
    levelbuttonCanvas->addChild(backbutt);
    backbutt->activate(103);
    levelbuttons.push_back(backbutt);

    // farm button
    std::shared_ptr<Texture> textureFarm = _assets->get<Texture>("levelfarm");
    std::shared_ptr<PolygonNode> idfarm = PolygonNode::allocWithTexture(textureFarm);
    idfarm->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> farmbutt = Button::alloc(idfarm);
    farmbutt->setAnchor(Vec2::ANCHOR_CENTER);
    farmbutt->setScale(0.45, 0.45);
    farmbutt->setPosition(SCENE_WIDTH*3/8, SCENE_HEIGHT*3/16);
    levelbackCanvas->addChild(farmbutt);

    // Forest button
    std::shared_ptr<Texture> textureForest = _assets->get<Texture>("levelforest");
    std::shared_ptr<PolygonNode> idforest = PolygonNode::allocWithTexture(textureForest);
    idforest->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> forestbutt = Button::alloc(idforest);
    forestbutt->setAnchor(Vec2::ANCHOR_CENTER);
    forestbutt->setScale(0.45, 0.45);
    forestbutt->setPosition(SCENE_WIDTH*3/4, SCENE_HEIGHT*3/8);
    levelbackCanvas->addChild(forestbutt);

    // Plant button
    std::shared_ptr<Texture> texturePlant = _assets->get<Texture>("levelplant");
    std::shared_ptr<PolygonNode> idplant = PolygonNode::allocWithTexture(texturePlant);
    idplant->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> plantbutt = Button::alloc(idplant);
    plantbutt->setAnchor(Vec2::ANCHOR_CENTER);
    plantbutt->setScale(0.45, 0.45);
    plantbutt->setPosition(SCENE_WIDTH/4, SCENE_HEIGHT*9/16);
    levelbackCanvas->addChild(plantbutt);

    // Throne button
    std::shared_ptr<Texture> textureThrone = _assets->get<Texture>("levelthrone");
    std::shared_ptr<PolygonNode> idthrone = PolygonNode::allocWithTexture(textureThrone);
    idthrone->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> thronebutt = Button::alloc(idthrone);
    thronebutt->setAnchor(Vec2::ANCHOR_CENTER);
    thronebutt->setScale(0.45, 0.45);
    thronebutt->setPosition(SCENE_WIDTH*3/4, SCENE_HEIGHT*3/4);
    levelbackCanvas->addChild(thronebutt);

    // Initializing locks
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 - 45, SCENE_HEIGHT*51/64, levelNodes, 12, true, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 - 75, SCENE_HEIGHT*47/64, levelNodes, 11, true, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 - 20, SCENE_HEIGHT*44/64, levelNodes, 10, true, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 + 20, SCENE_HEIGHT*40/64, levelNodes, 9, true, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 + 10, SCENE_HEIGHT*36/64, levelNodes, 8, true, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 - 30, SCENE_HEIGHT*32/64, levelNodes, 7, true, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 - 70, SCENE_HEIGHT*28/64, levelNodes, 6, true, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 - 80, SCENE_HEIGHT*24/64, levelNodes, 5, true, false);

    // Initializing arrow
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 - 45, SCENE_HEIGHT*21/64, levelNodes, 4, true, true);

    // Initializing flags
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 + 70, SCENE_HEIGHT*17/64, levelNodes, 3, false, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 + 115, SCENE_HEIGHT*13/64, levelNodes, 2, false, false);
    buildLevelSelect(levelbackCanvas, SCENE_WIDTH/2 + 125, SCENE_HEIGHT*9/64, levelNodes, 1, false, false);

    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LevelScene::dispose() {
    _assets = nullptr;
    _buttons.clear();
    level = 0;
    Scene::dispose();
}

void LevelScene::update(float timestep) {

}

/**
 * Sets whether the scene is currently active
 *
 * @param value whether the scene is currently active
 */
void LevelScene::setActive(bool value) {
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

int LevelScene::getLevel() { return level; }

/**
 * Deactivates buttons to allow for seamless scene changes
 */
void LevelScene::deactivateButtons() {
    //note: zero is the back button
    for (int i = 0; i <= LEVELS; i++) {
        levelbuttons[i]->deactivate();
    }
}

/**
 * Activates buttons to allow for seamless scene changes
 */
void LevelScene::activateButtons() {
    //note: zero is the back button
    for (int i = 0; i <= LEVELS; i++) {
        levelbuttons[i]->activate(103 + i);
    }
}

void LevelScene::setLevel(int lev) { level = lev; }
bool LevelScene::getBack() { return backClicked; }
void LevelScene::setBack(bool val) { backClicked = val; }

/**
 * Builds the button for each level, i.e. the white flag, , or lock arrow
 * 4/16: No current support for actual unlocking of levels, this is for playtesting 4/17
 */
void LevelScene::buildLevelSelect(std::shared_ptr<cugl::Node> node, int posX, int posY,  std::vector<std::shared_ptr<cugl::PolygonNode>> list, int lev, bool locked, bool curr){
    if (!locked && !curr) { // flags
        std::shared_ptr<Texture> texture = _assets->get<Texture>("levelflag");
        std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(texture);
        id->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<Button> butt = Button::alloc(id);
        butt->setAnchor(Vec2::ANCHOR_CENTER);
        butt->setScale(0.45f);
        butt->setPosition(posX, posY);
        butt->setListener([=](const std::string& name, bool down) { if (down) { level = lev; } });
        node->addChild(butt);
        butt->activate(103 + lev);
        levelbuttons.push_back(butt);
    }
    else if (curr){ // arrow
        std::shared_ptr<Texture> texture = _assets->get<Texture>("levelarrow");
        std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(texture);
        id->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<Button> butt = Button::alloc(id);
        butt->setAnchor(Vec2::ANCHOR_CENTER);
        butt->setScale(0.45f);
        butt->setPosition(posX, posY);
        butt->setListener([=](const std::string& name, bool down) { if (down) { level = lev; } });
        node->addChild(butt);
        butt->activate(103 + lev);
        levelbuttons.push_back(butt);
    }
    else { // locks
        std::shared_ptr<Texture> texture = _assets->get<Texture>("levellock");
        std::shared_ptr<PolygonNode> levelptr = PolygonNode::allocWithTexture(texture);
        levelptr->setScale(0.45f); // Magic number to rescale asset
        levelptr->setAnchor(Vec2::ANCHOR_CENTER);
        levelptr->setPosition(posX, posY);
        node->addChild(levelptr);
        list.push_back(levelptr);
    }
}