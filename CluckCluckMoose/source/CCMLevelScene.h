//
// Created by Amanda Trang on 4/10/19.
//

#ifndef __CCM_LEVEL_SCENE_H__
#define __CCM_LEVEL_SCENE_H__

#include <cugl/cugl.h>
#include <unordered_map>
#include <sstream>

// Sfx for the game
#define SOUND_BUTTON_A        "button_a"
#define SOUND_BUTTON_B        "button_b"

/**
 * A scene for demoing a layout manager
 */
class LevelScene : public cugl::Scene {
protected:
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;

    std::unordered_map<std::string,std::shared_ptr<cugl::Button>> _buttons;

public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new scene with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    LevelScene() {}

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~LevelScene() { dispose(); }

    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    virtual void dispose() override;

    /**
     * Initializes the controller contents, and starts the Menu
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

    static std::shared_ptr<LevelScene> alloc(const std::shared_ptr<cugl::AssetManager>& assets) {
        std::shared_ptr<LevelScene> result = std::make_shared<LevelScene>();
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
    void update(float timestep) override;

    /**
     * Getters and setters for level select buttons
     */
    int getLevel();
	void setLevel(int lev);
    bool getBack();
    void setBack(bool val);
    void deactivateButtons();
    void activateButtons();

    /**
     * Creates locks, flags, and arrow on level select.
     *
     * @param node canvas for this object
     * @param posX x position
     * @param posY y position
     * @param list list of items this belongs to (i.e. background or buttons)
     * @param lev int; level number
     * @param locked bool; if locked
     * @param curr bool; if arrow
     */
    void buildLevelSelect(std::shared_ptr<cugl::Node> node, int posX, int posY,  std::vector<std::shared_ptr<cugl::PolygonNode>> list, int lev, bool locked, bool curr);

    void drawNewBox(int cur);
    void drawLevelNodes(int cur);
    void playButtonSound(int sound);
    cugl::Size computeActiveSize() const;
};

#endif /* __CCM_LEVEL_SCENE_H__ */
