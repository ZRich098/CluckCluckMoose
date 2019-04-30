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
int curmap;
bool upClicked;
bool downClicked;
bool drawNew;
bool nodesMade;

//Screen dimensions
float levelscreenHeight;
float levelscreenWidth;

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
    
    //Set screen size
    levelscreenHeight = dimen.height;
    levelscreenWidth = dimen.width;

    //Root node for scene builder
    std::shared_ptr<Node> root;
    root = Node::alloc();
    addChild(root);
    root->removeAllChildren();

    level = 0;
    curmap = 0;
    backClicked = false;
    upClicked = false;
    downClicked = false;
    drawNew = false;
    nodesMade = false;
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
    levelbuttonCanvas->setPosition(levelscreenWidth / 2, 150);

    //reset drawing between frames
    levelbackCanvas->removeAllChildren();
    levelbuttonCanvas->removeAllChildren();

    //Draw background
    std::shared_ptr<Texture> texturebg = _assets->get<Texture>("levelbg");
    std::shared_ptr<PolygonNode> background = PolygonNode::allocWithTexture(texturebg);
    background->setScale(0.6f); // Magic number to rescale asset
    background->setAnchor(Vec2::ANCHOR_CENTER);
    background->setPosition(levelscreenWidth/2, levelscreenHeight/2);
    levelbackCanvas->addChild(background);

    //Draw level box
    std::shared_ptr<Texture> texturebox = _assets->get<Texture>("levelfarm");
    std::shared_ptr<PolygonNode> box = PolygonNode::allocWithTexture(texturebox);
    box->setScale(0.5f); // Magic number to rescale asset
    box->setAnchor(Vec2::ANCHOR_CENTER);
    box->setPosition(levelscreenWidth/2, levelscreenHeight/2);
    levelbackCanvas->addChild(box);

    // back button
    std::shared_ptr<Texture> textureBack = _assets->get<Texture>("levelback");
    std::shared_ptr<PolygonNode> idback = PolygonNode::allocWithTexture(textureBack);
    idback->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> backbutt = Button::alloc(idback);
    backbutt->setAnchor(Vec2::ANCHOR_CENTER);
    backbutt->setScale(0.5, 0.5);
    backbutt->setAnchor(Vec2::ANCHOR_CENTER);
    backbutt->setPosition(-levelscreenWidth*3/8, levelscreenHeight*3/4);
    backbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
            backClicked = true;
            level = 0; }});
    levelbuttonCanvas->addChild(backbutt);
    backbutt->activate(104);
    levelbuttons.push_back(backbutt);

    // up button
    std::shared_ptr<Texture> textureUp = _assets->get<Texture>("levelup");
    std::shared_ptr<PolygonNode> idup = PolygonNode::allocWithTexture(textureUp);
    idup->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> upbutt = Button::alloc(idup);
    upbutt->setAnchor(Vec2::ANCHOR_CENTER);
    upbutt->setScale(0.5, 0.5);
    upbutt->setPosition(levelscreenWidth*5/16, levelscreenHeight*10/16);
    upbutt->setListener([=](const std::string& name, bool down) {
        if (down) { upClicked = true; }});
    levelbuttonCanvas->addChild(upbutt);
    upbutt->activate(105);
    levelbuttons.push_back(upbutt);

    // down button
    std::shared_ptr<Texture> textureDown = _assets->get<Texture>("leveldown");
    std::shared_ptr<PolygonNode> iddown = PolygonNode::allocWithTexture(textureDown);
    iddown->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> downbutt = Button::alloc(iddown);
    downbutt->setAnchor(Vec2::ANCHOR_CENTER);
    downbutt->setScale(0.5, 0.5);
    downbutt->setPosition(levelscreenWidth*5/16, levelscreenHeight/8);
    downbutt->setListener([=](const std::string& name, bool down) {
        if (down) { downClicked = true; }});
    levelbuttonCanvas->addChild(downbutt);
    downbutt->activate(106);
    levelbuttons.push_back(downbutt);

    // Draw base circles
    buildLevelSelect(levelbuttonCanvas, -levelscreenWidth*4/32, levelscreenHeight*39/64, levelNodes, 0, false, false);
    buildLevelSelect(levelbuttonCanvas, 0, levelscreenHeight*25/64, levelNodes, 0, false, false);
    buildLevelSelect(levelbuttonCanvas, levelscreenWidth*5/32, levelscreenHeight*6/64, levelNodes, 0, false, false);
    
    // Draw farm nodes
    drawLevelNodes(0);

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

// This can be cleaned up lol
void LevelScene::update(float timestep) {
    if (curmap == 0) { // farm
        if (upClicked){
            curmap++;
            drawNew = true;
            upClicked = false;
        }
        else if (downClicked){ downClicked = false; }
    }
    else if (curmap == 1) { // forest
        if (upClicked){
            curmap++;
            drawNew = true;
            upClicked = false;
        }
        else if (downClicked){
            curmap--;
            drawNew = true;
            downClicked = false;
        }
    }
    else if (curmap == 2){ // plant
        if (upClicked){
            curmap++;
            drawNew = true;
            upClicked = false;
        }
        else if (downClicked){
            curmap--;
            drawNew = true;
            downClicked = false;
        }
    }
    else if (curmap == 3){ //throne
        if (upClicked){
            upClicked = false;
        }
        else if (downClicked){
            curmap--;
            drawNew = true;
            downClicked = false;
        }
    }
    if (drawNew){
        drawNew = false;
        drawNewBox(curmap);
    }

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
    for (int i = 0; i < levelbuttons.size(); i++) {
        levelbuttons[i]->deactivate();
    }
}

/**
 * Activates buttons to allow for seamless scene changes
 */
void LevelScene::activateButtons() {
    //note: zero is the back button
//    for (int i = 0; i <= levelbuttons.size(); i++) {
//        levelbuttons[i]->activate(104 + i);
//    }

    levelbuttons[0]->activate(104);
    levelbuttons[1]->activate(105);
    levelbuttons[2]->activate(106);
    levelbuttons[3]->activate(107); // lev 0% (3x)
    levelbuttons[4]->activate(108); // lev 2%
    levelbuttons[5]->activate(109); // lev 1%
}

void LevelScene::setLevel(int lev) { level = lev; }
bool LevelScene::getBack() { return backClicked; }
void LevelScene::setBack(bool val) { backClicked = val; }

/**
 * Builds the button for each level, i.e. the white flag, , or lock arrow
 * 4/16: No current support for actual unlocking of levels, this is for playtesting 4/17
 */
void LevelScene::buildLevelSelect(std::shared_ptr<cugl::Node> node, int posX, int posY,  std::vector<std::shared_ptr<cugl::PolygonNode>> list, int lev, bool locked, bool curr){
    if (lev == 0){ // circles beneath icons
        std::shared_ptr<Texture> textureCircle = _assets->get<Texture>("levelcircle");
        std::shared_ptr<PolygonNode> circleBase = PolygonNode::allocWithTexture(textureCircle);
        circleBase->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<Button> circle = Button::alloc(circleBase);
        circle->setAnchor(Vec2::ANCHOR_CENTER);
        circle->setScale(0.45, 0.45);
        circle->setPosition(posX, posY);
        levelbuttonCanvas->addChild(circle);
    }
    else if (!locked && !curr) { // flags
        std::shared_ptr<Texture> texture = _assets->get<Texture>("levelflag");
        std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(texture);
        id->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<Button> butt = Button::alloc(id);
        butt->setAnchor(Vec2::ANCHOR_CENTER);
        butt->setScale(0.45, 0.45);
        butt->setPosition(posX + 15, posY + 15);
        butt->setListener([=](const std::string& name, bool down) { if (down) { level = lev; } });
        levelbuttonCanvas->addChild(butt);
        butt->activate(107 + (lev % 3));
        levelbuttons.push_back(butt);
    }
    else if (curr){ // arrow
        std::shared_ptr<Texture> texture = _assets->get<Texture>("levelarrow");
        std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(texture);
        id->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<Button> butt = Button::alloc(id);
        butt->setAnchor(Vec2::ANCHOR_CENTER);
        butt->setScale(0.45, 0.45);
        butt->setPosition(posX, posY + 10);
        butt->setListener([=](const std::string& name, bool down) { if (down) { level = lev; } });
        levelbuttonCanvas->addChild(butt);
        butt->activate(107 + (lev % 3));
        levelbuttons.push_back(butt);
    }
    else { // locks
        std::shared_ptr<Texture> texture = _assets->get<Texture>("levellock");
        std::shared_ptr<PolygonNode> id = PolygonNode::allocWithTexture(texture);
        id->setAnchor(Vec2::ANCHOR_CENTER);
        std::shared_ptr<Button> butt = Button::alloc(id);
        butt->setAnchor(Vec2::ANCHOR_CENTER);
        butt->setScale(0.45, 0.45);
        butt->setPosition(posX, posY);
        butt->setListener([=](const std::string& name, bool down) { if (down) { level = lev; } });
        levelbuttonCanvas->addChild(butt);
        butt->activate(107 + (lev % 3));
        levelbuttons.push_back(butt);
    }
}

// draw new background level scene
void LevelScene::drawNewBox(int cur){
    std::shared_ptr<Texture> texturebox;
    if (cur == 0){ texturebox = _assets->get<Texture>("levelfarm"); }
    else if (cur == 1){ texturebox = _assets->get<Texture>("levelforest"); }
    else if (cur == 2){ texturebox = _assets->get<Texture>("levelplant"); }
    else if (cur == 3){ texturebox = _assets->get<Texture>("levelthrone"); }
    else { texturebox = _assets->get<Texture>("levelfarm"); }
    //Draw level box
    std::shared_ptr<PolygonNode> box = PolygonNode::allocWithTexture(texturebox);
    box->setScale(0.5f); // Magic number to rescale asset
    box->setAnchor(Vec2::ANCHOR_CENTER);
    box->setPosition(levelscreenWidth/2, levelscreenHeight/2);
    levelbackCanvas->addChild(box);

    drawLevelNodes(cur);
}

// Draws new flags/locks/arrows
void LevelScene::drawLevelNodes(int cur){
    if (nodesMade){
        levelbuttons[3]->setVisible(false);
        levelbuttons[4]->setVisible(false);
        levelbuttons[5]->setVisible(false);
        
        levelbuttons[3]->deactivate();
        levelbuttons[4]->deactivate();
        levelbuttons[5]->deactivate();
        
        levelbuttons.pop_back();
        levelbuttons.pop_back();
        levelbuttons.pop_back();
    }
    
    if (curmap == 0){
        buildLevelSelect(levelbuttonCanvas, -levelscreenWidth*4/32, levelscreenHeight*39/64, levelNodes, 3, false, false);
        buildLevelSelect(levelbuttonCanvas, 0, levelscreenHeight*25/64, levelNodes, 2, false, false);
        buildLevelSelect(levelbuttonCanvas, levelscreenWidth*5/32, levelscreenHeight*6/64, levelNodes, 1, false, false);
    }
    else if (curmap == 1){
        buildLevelSelect(levelbuttonCanvas, -levelscreenWidth*4/32, levelscreenHeight*39/64, levelNodes, 6, false, false);
        buildLevelSelect(levelbuttonCanvas, 0, levelscreenHeight*25/64, levelNodes, 5, false, false);
        buildLevelSelect(levelbuttonCanvas, levelscreenWidth*5/32, levelscreenHeight*6/64, levelNodes, 4, false, true);
    }
    else if (curmap == 2){
        buildLevelSelect(levelbuttonCanvas, -levelscreenWidth*4/32, levelscreenHeight*39/64, levelNodes, 9, true, false);
        buildLevelSelect(levelbuttonCanvas, 0, levelscreenHeight*25/64, levelNodes, 8, false, false);
        buildLevelSelect(levelbuttonCanvas, levelscreenWidth*5/32, levelscreenHeight*6/64, levelNodes, 7, false, false);
    }
    else {
        buildLevelSelect(levelbuttonCanvas, -levelscreenWidth*4/32, levelscreenHeight*39/64, levelNodes, 12, true, false);
        buildLevelSelect(levelbuttonCanvas, 0, levelscreenHeight*25/64, levelNodes, 11, true, false);
        buildLevelSelect(levelbuttonCanvas, levelscreenWidth*5/32, levelscreenHeight*6/64, levelNodes, 10, true, false);
    }
    nodesMade = true;
}

Size LevelScene::computeActiveSize() const {
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT / dimen.height;
    return dimen;
}
