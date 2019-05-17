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
#include <sstream>

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
	_saveLoad.init();

    // Que up the other assets
    _assets->loadDirectoryAsync("json/assets1.json",nullptr);

    _input.init();

	lastLevel = 1;

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
	_saveLoad.saveGame(4); //@TODO - Replace with call to highest active level
	//save current level state, if applicable
	if (_levelscene.getLevel() > 1) {
		_saveLoad.saveLevel(_gamescene->getPlayer(), _gamescene->getOpp(), _gamescene->getAI(),  _levelscene.getLevel());
	}
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
	std::shared_ptr<JsonReader> stateReader = JsonReader::allocWithAsset("json/saveGame.json");
	if (stateReader == nullptr) {
		CULog("State file not found");
	}
	else {
		std::shared_ptr<JsonValue> json = stateReader->readJson();
		if (json == nullptr) {
			CULog("Failed to load state file");
		}
		else {
			_levelscene.setLevel(_saveLoad.loadSaveGame(json));
		}
	}

	//load last level state, if applicable
	if (_levelscene.getLevel() > 1 && _gamescene == nullptr) {
		stringstream ss;
		ss << "json/level" << _levelscene.getLevel() << ".json";
		string fileName = ss.str();
		std::shared_ptr<JsonReader> gameReader = JsonReader::allocWithAsset(fileName);
		if (gameReader == nullptr) {
			CULog("json/level%d.json file not found", _levelscene.getLevel());
			_gamescene = GameScene::alloc(_assets);
			_gameplay.push_back(_gamescene);
			_gameplay.back()->setActive(false);
		}
		else {
			std::shared_ptr<JsonValue> json = gameReader->readJson();
			if (json == nullptr) {
				CULog("Failed to load level file");
				_gamescene = GameScene::alloc(_assets);
				_gameplay.push_back(_gamescene);
				_gameplay.back()->setActive(false);
			}
			else {
				CULog("File loading");
				std::shared_ptr<Moose> pl = _saveLoad.loadPlayerMoose(json->get("PlayerMoose"));
				std::shared_ptr<Moose> op = _saveLoad.loadOpponentMoose(json->get("OpponentMoose"));
				AIType ai = _saveLoad.loadAI(json->get("AI"));
				_gamescene = GameScene::alloc(_assets, pl, op, ai, _levelscene.getLevel());
				_gameplay.push_back(_gamescene);
				_gameplay.back()->setActive(false);

				_levelscene.setLevel(_saveLoad.loadLevelTag(json->get("Tag")));
			}
		}
	}
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

void CCMApp::update(float timestep) {
    if (!_loaded && _loadingscene.isActive()) {
        _loadingscene.update(timestep);
    } else if (!_loaded) { // if loading scene's PLAY is clicked
        _loadingscene.dispose(); // Disables the input listeners in this mode
        _gameplay.push_back(MenuScene::alloc(_assets));
        _gameplay.back()->setActive(false);
        _gameplay.push_back(LevelScene::alloc(_assets));
        _gameplay.back()->setActive(false);
        //_gameplay.push_back(GameScene::alloc(_assets));
        //_gameplay.back()->setActive(false);
        _current = 0; // go to main menu
        _gameplay[_current]->setActive(true);
        _loaded = true;
        _levelscene.deactivateButtons();
    } else {
		//CULog("updating input");
        _input.update(timestep);
		//CULog("done updating input");
        if (_current == 0) { // if on menu scene
            if (_menuscene.getPlay()) { // play is clicked
                _menuscene.deactivateButtons();
                _gameplay[_current]->setActive(false);
                _current = 1; // to level select
                _gameplay[_current]->setActive(true);
                _menuscene.setPlay(false);
                _levelscene.activateButtons();
            }
        }
        else if (_current == 1) { // on level select
            if (_levelscene.getBack()) {
                _levelscene.deactivateButtons();
                _gameplay[_current]->setActive(false);
                _current = 0; // back to main menu
                _gameplay[_current]->setActive(true);
                _levelscene.setBack(false);
                _menuscene.activateButtons();
            }
            else if (_levelscene.getLevel() != 0) { // level chosen
				_levelscene.deactivateButtons();
				_gameplay[_current]->setActive(false);
				_current = 2;

				if (_levelscene.getLevel() == 1) { //if tutorial
					std::shared_ptr<Moose> pl = Moose::alloc(5, 6, true, true);
					std::shared_ptr<Moose> op = Moose::alloc(5, 6, true, false);
					if (_gameplay.size() < 3) {
						_gamescene = GameScene::tutorialAlloc(_assets, pl, op);
					}
					else {
						_gamescene->setPlayer(pl);
						_gamescene->setOpp(op);
						_gamescene->setAI(op, pl, AIType::Tutorial);
						_gamescene->setTutorial();
						_gameplay.pop_back();
					}
					_gameplay.push_back(_gamescene);
					_gameplay.back()->setActive(false);
					_input.init();
					_gameplay[_current]->setActive(true);
					_gamescene->deactivatePause();

				}
				else {
					//load level, if able
					stringstream ss;
					ss << "json/level" << _levelscene.getLevel() << ".json";
					string fileName = ss.str();
					std::shared_ptr<JsonReader> gameReader = JsonReader::allocWithAsset(fileName);
					if (gameReader == nullptr) {
						CULog("json/level%d.json file not found", _levelscene.getLevel());
						_gamescene = GameScene::alloc(_assets);
						_gameplay.push_back(_gamescene);
						_gameplay.back()->setActive(false);
					}
					else {
						std::shared_ptr<JsonValue> json = gameReader->readJson();
						if (json == nullptr) {
							CULog("Failed to load level file");
							_gamescene = GameScene::alloc(_assets);
							_gameplay.push_back(_gamescene);
							_gameplay.back()->setActive(false);
						}
						else {
							_levelscene.setLevel(_saveLoad.loadLevelTag(json->get("Tag")));
							bool newLevel = false;
							if (lastLevel != _levelscene.getLevel()) {
								CULog("loaded level: %d, last level: %d", _levelscene.getLevel(), lastLevel);
								newLevel = true;
								lastLevel = _levelscene.getLevel();
							}
							if (_gameplay.size() < 3) {
								CULog("Initial asset file loading");
								std::shared_ptr<Moose> pl = _saveLoad.loadPlayerMoose(json->get("PlayerMoose"));
								std::shared_ptr<Moose> op = _saveLoad.loadOpponentMoose(json->get("OpponentMoose"));
								AIType ai = _saveLoad.loadAI(json->get("AI"));
								_gamescene = GameScene::alloc(_assets, pl, op, ai, _levelscene.getLevel());
							}
							else if (newLevel || _gamescene->getPlayer()->getHealth() <= 0 || _gamescene->getOpp()->getHealth() <= 0) {
								CULog("Asset file loading");
								std::shared_ptr<Moose> pl = _saveLoad.loadPlayerMoose(json->get("PlayerMoose"));
								std::shared_ptr<Moose> op = _saveLoad.loadOpponentMoose(json->get("OpponentMoose"));
								AIType ai = _saveLoad.loadAI(json->get("AI"));
								_gamescene->setPlayer(pl);
								_gamescene->setOpp(op);
								_gamescene->setAI(op, pl, ai);
								_gamescene->setLevel(_levelscene.getLevel());
								_gameplay.pop_back();
							}
							else {
								CULog("Save file loading");
								gameReader = JsonReader::alloc("saveLevel.json");
								json = gameReader->readJson();
								std::shared_ptr<Moose> pl = _saveLoad.loadPlayerMoose(json->get("PlayerMoose"));
								std::shared_ptr<Moose> op = _saveLoad.loadOpponentMoose(json->get("OpponentMoose"));
								AIType ai = _saveLoad.loadAI(json->get("AI"));
								_gamescene->setPlayer(pl);
								_gamescene->setOpp(op);
								_gamescene->setAI(op, pl, ai);
								_gamescene->setLevel(_levelscene.getLevel());
								_gameplay.pop_back();
							}
							_gameplay.insert(_gameplay.begin() + _current, _gamescene);
							_gameplay.at(_current)->setActive(false);
							_input.init();
						}
					}
					_gameplay[_current]->setActive(true);
					_gamescene->deactivatePause();
				}
            }
        }
        else if (_current == 2) { // in game scene
            if (_gamescene->getHome()) {
				//CULog("%s", getSaveDirectory().c_str());
				if (_gamescene->getPlayer()->getHealth() > 0 || _gamescene->getOpp()->getHealth() > 0) {
					_saveLoad.saveLevel(_gamescene->getPlayer(), _gamescene->getOpp(), _gamescene->getAI(), _levelscene.getLevel());
				}
				
				lastLevel = _levelscene.getLevel();
                _gamescene->setHome(false);
                _gamescene->deactivatePause();
                _gameplay[_current]->setActive(false);
                //_gameplay[_current]->dispose();
                //_gameplay.erase(_gameplay.begin()+_current);
                _current = 0; // back to main menu
                _gameplay[_current]->setActive(true);
                _menuscene.activateButtons();
                _levelscene.setLevel(0);
				_gamescene->setLevel(_levelscene.getLevel());
            }
			else if (_gamescene->getRedo() || _gamescene->getNextLevel()) {
				CULog("Asset file loading");
				if (_gamescene->getNextLevel()) {
					_levelscene.setLevel(_levelscene.getLevel() + 1);
				}

				if (_levelscene.getLevel() == 1) { //redo tutorial
					std::shared_ptr<Moose> pl = Moose::alloc(5, 6, true, true);
					std::shared_ptr<Moose> op = Moose::alloc(5, 6, true, false);
					_gamescene->setPlayer(pl);
					_gamescene->setOpp(op);
					_gamescene->setAI(op, pl, AIType::Tutorial);
					_gamescene->setTutorial();
					_gameplay.pop_back();
					_gameplay.push_back(_gamescene);
					_gameplay[_current]->setActive(true);
					_gamescene->deactivatePause();
					_input.init();
				}
				else {
					stringstream ss;
					ss << "json/level" << _levelscene.getLevel() << ".json";
					string fileName = ss.str();
					std::shared_ptr<JsonReader> gameReader = JsonReader::allocWithAsset(fileName);
					std::shared_ptr<JsonValue> json = gameReader->readJson();
					std::shared_ptr<Moose> pl = _saveLoad.loadPlayerMoose(json->get("PlayerMoose"));
					std::shared_ptr<Moose> op = _saveLoad.loadOpponentMoose(json->get("OpponentMoose"));
					AIType ai = _saveLoad.loadAI(json->get("AI"));
					_gamescene->setPlayer(pl);
					_gamescene->setOpp(op);
					_gamescene->setAI(op, pl, ai);
					_gamescene->setLevel(_levelscene.getLevel());
					_gameplay.pop_back();
					_gameplay.insert(_gameplay.begin() + _current, _gamescene);
					_gameplay.at(_current)->setActive(false);
					_input.init();
				}
			}
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
