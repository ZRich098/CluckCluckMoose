//
//  CCMInput.cpp
//  CCM Demo
//
//  This input controller is primarily designed for keyboard control.  On mobile
//  you will notice that we use gestures to emulate keyboard commands. They even
//  use the same variables (though we need other variables for internal keyboard
//  emulation).  This simplifies our design quite a bit.
//
//	This file is based on the ragdoll demo, with some features removed
//  The ragdoll demo is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author of Ragdoll Demo: Walker White and Anthony Perello
//  Version: 1/26/17
//
#ifndef __CCM_INPUT_H__
#define __CCM_INPUT_H__
#include <cugl/cugl.h>

/**
 * This class represents player input in the Ragdollk demo.
 *
 * This input handler uses the CUGL input API.  It uses the polling API for
 * keyboard, but the callback API for touch.  This demonstrates a mix of ways
 * to handle input, and the reason for hiding it behind an abstraction like
 * this class.
 *
 * Unlike CUGL input devices, this class is not a singleton.  It must be
 * allocated before use.  However, you will notice that we do not do any
 * input initialization in the constructor.  This allows us to allocate this
 * controller as a field without using pointers. We simply add the class to the
 * header file of its owner, and delay initialization (via the method init())
 * until later. This is one of the main reasons we like to avoid initialization
 * in the constructor.
 */
class CCMInput {
private:
	/** Whether or not this input is active */
	bool _active;

	// TOUCH SUPPORT
	/** The initial touch location for the current gesture, IN SCREEN COORDINATES */
	cugl::Vec2 _dtouch;
	/** The timestamp for the beginning of the current gesture */
	cugl::Timestamp _timestamp;


	/**
	 * Handles touchBegan and mousePress events using shared logic.
	 *
	 * Depending on the platform, the appropriate callback (i.e. touch or mouse)
	 * will call into this method to handle the Event.
	 *
	 * @param timestamp	 the timestamp of the event
	 * @param pos		 the position of the touch
	 */
	void touchBegan(const cugl::Timestamp timestamp, const cugl::Vec2& pos);


	/**
	 * Handles touchEnded and mouseReleased events using shared logic.
	 *
	 * Depending on the platform, the appropriate callback (i.e. touch or mouse)
	 * will call into this method to handle the Event.
	 *
	 * @param timestamp	 the timestamp of the event
	 * @param pos		 the position of the touch
	 */
	void touchEnded(const cugl::Timestamp timestamp, const cugl::Vec2& pos);

	/**
	 * Handles touchMoved and mouseDragged events using shared logic.
	 *
	 * Depending on the platform, the appropriate callback (i.e. touch or mouse)
	 * will call into this method to handle the Event.
	 *
	 * @param timestamp	 the timestamp of the event
	 * @param pos		 the position of the touch
	 */
	void touchMoved(const cugl::Vec2& pos);


protected:
	// INPUT RESULTS
	/** Are we registering an object selection? */
	bool _select;
	/** The id of the current selection touch */
	long long _touchID;

public:
#pragma mark -
#pragma mark Constructors
	/**
	 * Creates a new input controller.
	 *
	 * This constructor does NOT do any initialzation.  It simply allocates the
	 * object. This makes it safe to use this class without a pointer.
	 */
	CCMInput(); // Don't initialize.  Allow stack based

	/**
	 * Disposes of this input controller, releasing all listeners.
	 */
	~CCMInput() { dispose(); }

	/**
	 * Deactivates this input controller, releasing all listeners.
	 *
	 * This method will not dispose of the input controller. It can be reused
	 * once it is reinitialized.
	 */
	void dispose();

	/**
	 * Deactivates this input controller, releasing all listeners.
	 *
	 * This method will not dispose of the input controller. It can be reused
	 * once it is reinitialized.
	 */
	bool init();

#pragma mark -
#pragma mark Input Detection
	/**
	 * Returns true if the input handler is currently active
	 *
	 * @return true if the input handler is currently active
	 */
	bool isActive() const { return _active; }

	/**
	 * Clears any buffered inputs so that we may start fresh.
	 */
	void clear();

#pragma mark -
#pragma mark Input Results
	/**
	 * Returns a bool based on whether a touch or mouse-click is happening
	 */
	bool didSelect() const { return _select; }

	/**
	 * Returns the location (in world space) of the selection.
	 *
	 * The origin of the coordinate space is the top left corner of the
	 * screen.  This will need to be transformed to world coordinates
	 * (via the scene graph) to be useful
	 *
	 * @return the location (in world space) of the selection.
	 */
	const cugl::Vec2& getSelection() const { return _dtouch; }


#pragma mark -
#pragma mark Touch and Mouse Callbacks
	/**
	 * Callback for the beginning of a touch event
	 *
	 * @param t     The touch information
	 * @param event The associated event
	 */
	void touchBeganCB(const cugl::TouchEvent& event, bool focus);

	/**
	 * Callback for a mouse press event.
	 *
	 * @param t     The touch information
	 * @param event The associated event
	 */
	void mousePressBeganCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

	/**
	 * Callback for the end of a touch event
	 *
	 * @param t     The touch information
	 * @param event The associated event
	 */
	void touchEndedCB(const cugl::TouchEvent& event, bool focus);

	/**
	 * Callback for a mouse release event.
	 *
	 * @param t     The touch information
	 * @param event The associated event
	 */
	void mouseReleasedCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

	/**
	 * Callback for a mouse release event.
	 *
	 * @param t     The touch information
	 * @param event The associated event
	 */
	void touchesMovedCB(const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus);

	/**
	 * Callback for a mouse drag event.
	 *
	 * @param t     The touch information
	 * @param event The associated event
	 */
	void mouseDraggedCB(const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus);

};

#endif /* __CCM_INPUT_H__ */

