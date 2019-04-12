//
//  CCMLoadingScene.cpp
//  CCM Demo
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "CCMLoadingScene.h"

using namespace cugl;

/** The ID for the button listener */
#define LISTENER_ID 1
/** This is adjusted by screen aspect ratio to get the height */
#define SCENE_WIDTH 1024

/** The music to play while loading*/
#define LOADING_MUSIC "trailer"

#pragma mark -
#pragma mark Constructors

/**
 * Initializes the controller contents, making it ready for loading
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool LoadingScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_WIDTH/dimen.width; // Lock the game to a reasonable resolution
    if (assets == nullptr) {
        return false;
    } else if (!Scene::init(dimen)) {
        return false;
    }
    
    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/loading.json");
    auto layer = assets->get<Node>("load");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    
	auto loading_music = _assets->get<Sound>(LOADING_MUSIC);
	AudioChannels::get()->queueMusic(loading_music, true, loading_music->getVolume());
	

    _bar = std::dynamic_pointer_cast<ProgressBar>(assets->get<Node>("load_bar"));
    _button = std::dynamic_pointer_cast<Button>(assets->get<Node>("load_claw_play"));
    _button->setListener([=](const std::string& name, bool down) {
        this->_active = down;
    });
    
    Application::get()->setClearColor(Color4(192,192,192,255));
    addChild(layer);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LoadingScene::dispose() {
    // Deactivate the button (platform dependent)
    if (isPending()) {
        _button->deactivate();
    }
    _button = nullptr;
    _bar = nullptr;
    _assets = nullptr;
    _progress = 0.0f;

	//Stop playing audio

	AudioChannels::get()->stopMusic();
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LoadingScene::update(float progress) {
    if (_progress < 1) {
        _progress = _assets->progress();
        if (_progress >= 1) {
            _progress = 1.0f;
            _button->setVisible(true);
            _button->activate(1);
        }
        _bar->setProgress(_progress);
    }
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool LoadingScene::isPending( ) const {
    return _button != nullptr && _button->isVisible();
}
