//
//  CCMApp.cpp
//  CCM Demo
//
//  This is the root class for your game.  The file main.cpp accesses this class
//  to run the application.  While you could put most of your game logic in
//  this class, we prefer to break the game up into player modes and have a
//  class for each mode.
//
//  Author: Walker White and Anthony Perello
//  Version: 1/29/17
//
#include "CCMApp.h"

using namespace cugl;


#pragma mark -
#pragma mark Application State

/**
 * The method called after OpenGL is initialized, but before running the application.
 *
 * This is the method in which all user-defined program intialization should
 * take place.  You should not create a new init() method.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to FOREGROUND,
 * causing the application to run.
 */
void CCMApp::onStartup() {
    _assets = AssetManager::alloc();
    _batch  = SpriteBatch::alloc();
    
    // Start-up basic input
#ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
    Input::activate<PanInput>();
#else
    Input::activate<Mouse>();
#endif
    _assets->attach<Font>(FontLoader::alloc()->getHook());
    _assets->attach<Texture>(TextureLoader::alloc()->getHook());
    _assets->attach<Node>(SceneLoader::alloc()->getHook());

    // Create a "loading" screen
    _loaded = false;
    _loadingscene.init(_assets);

    _playClicked = false;
    _helpClicked = false;
    _settingsClicked = false;

    _levelSelected = false;
    
    // Que up the other assets
    _assets->loadDirectoryAsync("json/assets1.json",nullptr);
//    _assets->loadAsync<LevelModel>(LEVEL_ONE_KEY,LEVEL_ONE_FILE,nullptr);

    _input.init();
    
    Application::onStartup(); // YOU MUST END with call to parent
}

/**
 * The method called when the application is ready to quit.
 *
 * This is the method to dispose of all resources allocated by this
 * application.  As a rule of thumb, everything created in onStartup()
 * should be deleted here.
 *
 * When overriding this method, you should call the parent method as the
 * very last line.  This ensures that the state will transition to NONE,
 * causing the application to be deleted.
 */
void CCMApp::onShutdown() {
    _loadingscene.dispose();
    _gameplay.clear();
    _assets = nullptr;
    _batch = nullptr;
    
    // Shutdown input
#ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
#else
    Input::deactivate<Mouse>();
#endif

    _input.dispose();
    AudioChannels::stop();
    Application::onShutdown();  // YOU MUST END with call to parent
}

/**
 * The method called when the application is suspended and put in the background.
 *
 * When this method is called, you should store any state that you do not
 * want to be lost.  There is no guarantee that an application will return
 * from the background; it may be terminated instead.
 *
 * If you are using audio, it is critical that you pause it on suspension.
 * Otherwise, the audio thread may persist while the application is in
 * the background.
 */
void CCMApp::onSuspend() {
    AudioChannels::get()->pauseAll();
}

/**
 * The method called when the application resumes and put in the foreground.
 *
 * If you saved any state before going into the background, now is the time
 * to restore it. This guarantees that the application looks the same as
 * when it was suspended.
 *
 * If you are using audio, you should use this method to resume any audio
 * paused before app suspension.
 */
void CCMApp::onResume() {
    AudioChannels::get()->resumeAll();
}


#pragma mark -
#pragma mark Application Loop

/**
 * The method called to update the application data.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should contain any code that is not an OpenGL call.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
// */

void CCMApp::update(float timestep) {
    if (!_loaded && _loadingscene.isActive()) {
        _loadingscene.update(timestep);
    } else if (!_loaded) {
        _loadingscene.dispose(); // Disables the input listeners in this mode
        _gameplay.push_back(MenuScene::alloc(_assets));
        _gameplay.back()->setActive(false);
        _gameplay.push_back(LevelScene::alloc(_assets));
        _gameplay.back()->setActive(false);
        _gameplay.push_back(GameScene::alloc(_assets));
        _gameplay.back()->setActive(false);
        _current = 0;
        _gameplay[_current]->setActive(true);
        _loaded = true;
        _levelscene.deactivateButtons();

    } else {
        _input.update(timestep);
        if (_menuscene.getPlay()) { _playClicked = true; }
        else if (_levelscene.getLevel() != 0) { _levelSelected = true; }
        if (_playClicked and (_current == 0)) { // from mainmenu to level select
            _levelscene.activateButtons();
            _gameplay[_current]->setActive(false);
            _current = 1;
            _gameplay[_current]->setActive(true);
        }
//        else if (_current = 1 and _levelscene.getLevel() == 0) {
//            if (_levelscene.getBack()) {
//                _levelscene.deactivateButtons();
//                _gameplay[_current]->setActive(false); // from level select to main
//                _current = 2;
//                _gameplay[_current]->setActive(true);
//            }
//        }
        else if (_levelscene.getLevel() != 0 and (_current == 1)) { // from levelselect
//            std::string levelNum = std::to_string(_levelScene.getLevel());
//            std::string levelFile = "json/assets" + levelNum + ".json";

//             Que up the other assets
//            _assets->loadDirectoryAsync(levelFile,nullptr);

            _levelscene.deactivateButtons();
            _gameplay[_current]->setActive(false);
            _current = 2;
            _gameplay[_current]->setActive(true);
        }
        _gameplay[_current]->update(timestep);
    }
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void CCMApp::draw() {
    if (!_loaded) {
        _loadingscene.render(_batch);
    } else {
        _gameplay[_current]->render(_batch);
    }
}

