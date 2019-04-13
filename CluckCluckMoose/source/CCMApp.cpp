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
	AudioChannels::start(24);
	
	// Start-up basic input
#ifdef CU_TOUCH_SCREEN
	Input::activate<Touchscreen>();
	Input::activate<PanInput>();
#else
	Input::activate<Mouse>();
#endif
	_assets->attach<Font>(FontLoader::alloc()->getHook());
	_assets->attach<Texture>(TextureLoader::alloc()->getHook());
	_assets->attach<Sound>(SoundLoader::alloc()->getHook());
	_assets->attach<Node>(SceneLoader::alloc()->getHook());

	// Create a "loading" screen
	_loaded = false;
	_loadingscene.init(_assets);

	// Create the saving and loading controller
	//_saveLoad

	// Que up the other assets
	_assets->loadDirectoryAsync("json/assets1.json",nullptr);

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

	AudioChannels::stop();
	_input.dispose();
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
	//save player's game state
	//save current level state, if applicable
	//needs to have a way of accessing the GameScene
	_saveLoad.saveLevel(_gamescene.getPlayer(),_gamescene.getOpp());
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
	//load player's game state
	//JsonLoader::alloc();
	//JsonLoader::read();
	//load last level state, if applicable
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
*/
//void CCMApp::update(float timestep) {
//    if (!_loaded && _loading.isActive()) {
//        _loading.update(0.01f);
//    } else if (!_loaded) {
//        _loading.dispose(); // Disables the input listeners in this mode
//        _gameplay.init(_assets);
//        _loaded = true;
//    } else {
//        _gameplay.update(timestep);
//    }
//}

void CCMApp::update(float timestep) {
	if (!_loaded && _loadingscene.isActive()) {
		_loadingscene.update(timestep);
	} else if (!_loaded) {
		_loadingscene.dispose(); // Disables the input listeners in this mode
		_gameplay.push_back(MenuScene::alloc(_assets));
		_gameplay.back()->setActive(false);
		_gameplay.push_back(GameScene::alloc(_assets));
		_gameplay.back()->setActive(false);
		_current = 0;
		_gameplay[_current]->setActive(true);
		_loaded = true;
	} else {
		_input.update(timestep);
		_playClicked = false; // loading start and Play might overlap on some devices
		if (_menuscene.getPlay()) {
			_playClicked = true;
		}
		if (_playClicked) {
			_playClicked = false;
			_gameplay[_current]->setActive(false);
			//_current = (_current + 1);
			_current = 1; // this will have to change with Help and Settings
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