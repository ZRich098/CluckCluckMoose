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

//Main canvas to draw stuff to
std::shared_ptr<Node> levellayer;

//Canvas for background
std::shared_ptr<Node> levelbackCanvas;

//Canvas for title
//std::shared_ptr<Node> titleCanvas;

//Canvas for buttons
std::shared_ptr<Node> levelbuttonCanvas;

// List for buttons
std::vector<std::shared_ptr<Button>> levelbuttons;


//Level tracking
bool backClicked;
//bool farmClicked;
//bool forestClicked;
//bool plantClicked;

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
            CULog("back");
            backClicked = true;
        }
    });

    // farm button
    std::shared_ptr<Texture> textureFarm = _assets->get<Texture>("levelfarm");
    std::shared_ptr<PolygonNode> idfarm = PolygonNode::allocWithTexture(textureFarm);
    idfarm->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> farmbutt = Button::alloc(idfarm);
    farmbutt->setAnchor(Vec2::ANCHOR_CENTER);
    farmbutt->setScale(0.5, 0.5);

    farmbutt->setAnchor(Vec2::ANCHOR_CENTER);
    farmbutt->setPosition(-SCENE_WIDTH/16, SCENE_HEIGHT/16);
    farmbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
            level = 1;
        }
    });



    // Forest button
    std::shared_ptr<Texture> textureForest = _assets->get<Texture>("levelforest");
    std::shared_ptr<PolygonNode> idforest = PolygonNode::allocWithTexture(textureForest);
    idforest->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> forestbutt = Button::alloc(idforest);
    forestbutt->setAnchor(Vec2::ANCHOR_CENTER);
    forestbutt->setScale(0.3, 0.3);

    forestbutt->setAnchor(Vec2::ANCHOR_CENTER);
    forestbutt->setPosition(SCENE_WIDTH/4, SCENE_HEIGHT/4);
    forestbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
            level = 2;
        }
    });


    // Plant button
    std::shared_ptr<Texture> texturePlant = _assets->get<Texture>("levelplant");
    std::shared_ptr<PolygonNode> idplant = PolygonNode::allocWithTexture(texturePlant);
    idplant->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> plantbutt = Button::alloc(idplant);
    plantbutt->setAnchor(Vec2::ANCHOR_CENTER);
    plantbutt->setScale(0.2, 0.2);

    plantbutt->setAnchor(Vec2::ANCHOR_CENTER);
    plantbutt->setPosition(-SCENE_WIDTH*3/16, SCENE_HEIGHT*7/16);
    plantbutt->setListener([=](const std::string& name, bool down) {
        if (down) {
            level = 3;
        }
    });

    // Throne button
    std::shared_ptr<Texture> textureThrone = _assets->get<Texture>("levelthrone");
    std::shared_ptr<PolygonNode> idthrone = PolygonNode::allocWithTexture(textureThrone);
    idthrone->setAnchor(Vec2::ANCHOR_CENTER);
    std::shared_ptr<Button> thronebutt = Button::alloc(idthrone);
    thronebutt->setAnchor(Vec2::ANCHOR_CENTER);
    thronebutt->setScale(0.4, 0.4);

    thronebutt->setAnchor(Vec2::ANCHOR_CENTER);
    thronebutt->setPosition(SCENE_WIDTH/4, SCENE_HEIGHT*9/16);
    thronebutt->setListener([=](const std::string& name, bool down) {
        if (down) {
            level = 4;
        }
    });

    levelbuttonCanvas->addChild(backbutt);
    levelbuttonCanvas->addChild(farmbutt);
    levelbuttonCanvas->addChild(forestbutt);
    levelbuttonCanvas->addChild(plantbutt);
    levelbuttonCanvas->addChild(thronebutt);

    //ensure keys are unique
    backbutt->activate(103);
    farmbutt->activate(104);
    forestbutt->activate(105);
    plantbutt->activate(106);
    thronebutt->activate(107);

    levelbuttons.push_back(backbutt);
    levelbuttons.push_back(farmbutt);
    levelbuttons.push_back(forestbutt);
    levelbuttons.push_back(plantbutt);
    levelbuttons.push_back(thronebutt);

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

void LevelScene::deactivateButtons() {
    levelbuttons[0]->deactivate();
    levelbuttons[1]->deactivate();
    levelbuttons[2]->deactivate();
    levelbuttons[3]->deactivate();
    levelbuttons[4]->deactivate();
//    for (int i = 0; i < levelbuttons.getSize(); i++) {
//        CULog("deactivating i %d", i);
//        levelbuttons[i]->deactivate();
//    }

}

void LevelScene::activateButtons() {
    levelbuttons[0]->activate(103);
    levelbuttons[1]->activate(104);
    levelbuttons[2]->activate(105);
    levelbuttons[3]->activate(106);
    levelbuttons[4]->activate(107);
}

bool LevelScene::getBack() { return backClicked; }