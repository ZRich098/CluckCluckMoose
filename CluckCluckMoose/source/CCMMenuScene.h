//
// Created by Amanda Trang on 4/10/19.
//

#ifndef __CCM_MENU_SCENE_H__
#define __CCM_MENU_SCENE_H__

#include <cugl/cugl.h>
#include <unordered_map>

/**
 * A scene for demoing a layout manager
 */
class MenuScene : public cugl::Scene {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;

    std::unordered_map<std::string,std::shared_ptr<cugl::Button>> _buttons;

//    bool playClicked;
//    bool helpClicked;
//    bool settingsClicked;

public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new scene with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    MenuScene() {}

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~MenuScene() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    virtual void dispose() override;

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
    bool init(const std::shared_ptr<cugl::AssetManager>& assets);

    static std::shared_ptr<MenuScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<MenuScene> result = std::make_shared<MenuScene>();
        return (result->init(assets) ? result : nullptr);
    }

    /**
     * Sets whether the scene is currently active
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

    /**
    * The method called to update the game mode.
     *
     * This method contains any gameplay code that is not an OpenGL call.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep);

    bool getPlay();
    bool setPlay(bool val);
    bool getHelp();
    bool getSettings();
};

#endif /* __CCM_MENU_SCENE_H__ */
