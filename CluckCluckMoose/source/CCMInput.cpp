//
//  CCMInput.cpp
//  CCM Demo
//
//  This input controller is primarily designed for mouse control.  On mobile
//  you will notice that we use a two finger pan gesture to emulate this.
//  While the use of the mouse necessitates additional state for tracking movement,
//  our PanInput class does not use such state, and can set _pandelta directly.
//
//  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
//
//  Author: Walker White and Anthony Perello
//  Version: 1/29/17
//
#include "CCMInput.h"

using namespace cugl;

#pragma mark Input Constants

/** The key to use for reseting the game */
#define RESET_KEY KeyCode::R
/** The key for exitting the game */
#define EXIT_KEY  KeyCode::ESCAPE

/** The key for the event handlers */
#define LISTENER_KEY        1

/** The number of fingers to be used for a pan gesture */
#define NUM_PAN_FINGERS 2


#pragma mark -
#pragma mark Input Controller
/**
* Creates a new input controller.
*
* This constructor does NOT do any initialzation.  It simply allocates the
* object. This makes it safe to use this class without a pointer.
*/
CCMInput::CCMInput() :
	_active(false),
	_resetPressed(false),
	_exitPressed(false),
	_keyReset(false),
	_keyExit(false),
	_mousepan(false) {
}

/**
* Deactivates this input controller, releasing all listeners.
*
* This method will not dispose of the input controller. It can be reused
* once it is reinitialized.
*/
void CCMInput::dispose() {
	if (_active) {
#ifndef CU_TOUCH_SCREEN
		Input::deactivate<Keyboard>();
		Mouse* mouse = Input::get<Mouse>();
		mouse->removePressListener(LISTENER_KEY);
		mouse->removeReleaseListener(LISTENER_KEY);
		mouse->removeDragListener(LISTENER_KEY);
#else
		PanInput* pan = Input::get<PanInput>();
		pan->removeMotionListener(LISTENER_KEY);
		pan->removeEndListener(LISTENER_KEY);
#endif
		_active = false;
	}
}

/**
* Initializes the input control for the given drawing scale.
*
* This method works like a proper constructor, initializing the input
* controller and allocating memory.  However, it still does not activate
* the listeners.  You must call start() do that.
*
* @return true if the controller was initialized successfully
*/
bool CCMInput::init() {
	bool success = true;

#ifndef CU_TOUCH_SCREEN
	success = Input::activate<Keyboard>();
	success = success && Input::activate<Mouse>();
	Mouse* mouse = Input::get<Mouse>();
	// Set pointer awareness to always so listening for drags registers
	// See addDragListener for an explanation
	mouse->setPointerAwareness(cugl::Mouse::PointerAwareness::ALWAYS);
	mouse->addPressListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
		this->mouseDownCB(event, clicks, focus);
	});
	mouse->addReleaseListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
		this->mouseUpCB(event, clicks, focus);
	});
	mouse->addMotionListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus) {
		this->mouseMovedCB(event, previous, focus);
});
#else
	PanInput* panInput = Input::get<PanInput>();
	panInput->setFingerSensitive(true);
	panInput->setTouchScreen(true);
	panInput->addEndListener(LISTENER_KEY,[=](const PanEvent& event, bool focus) {
		this->panEndedCB(event,focus);
	});
	panInput->addMotionListener(LISTENER_KEY,[=](const PanEvent& event, bool focus) {
		this->panMovedCB(event,focus);
	});
#endif
	_active = success;
	return success;
}


/**
* Processes the currently cached inputs.
*
* This method is used to to poll the current input state.  This will poll the
* keyboad and accelerometer.
*
* This method also gathers the delta difference in the touches. Depending on
* the OS, we may see multiple updates of the same touch in a single animation
* frame, so we need to accumulate all of the data together.
*/
void CCMInput::update(float dt) {
#ifndef CU_TOUCH_SCREEN
	// DESKTOP CONTROLS
	Keyboard* keys = Input::get<Keyboard>();

	// Map "keyboard" events to the current frame boundary
	_keyReset  = keys->keyPressed(RESET_KEY);
	_keyExit   = keys->keyPressed(EXIT_KEY);

	if (_mousepan) {
		_pandelta = _currentTouch - _previousTouch;
		_pandelta.y *= -1.0f;
	}
	else {
		_pandelta.x = 0.0f;
		_pandelta.y = 0.0f;
	}
#endif
	_previousTouch = _currentTouch;

	_resetPressed = _keyReset;
	_exitPressed  = _keyExit;

	// If it does not support keyboard, we must reset "virtual" keyboard
#ifdef CU_TOUCH_SCREEN
	_keyReset = false;
#endif
}

/**
* Clears any buffered inputs so that we may start fresh.
*/
void CCMInput::clear() {
	_resetPressed = false;
	_exitPressed  = false;

	_currentTouch = Vec2::ZERO;
	_previousTouch = Vec2::ZERO;
  _pandelta = Vec2::ZERO;

	_mousepan = false;
}

#pragma mark -
#pragma mark Touch Callbacks
/**
* Callback for the end of a touch event
*
* @param event	   The associated event
* @param focus     Whether the listener currently has focus
*/
void CCMInput::panEndedCB(const cugl::PanEvent& event, bool focus) {
  _pandelta = Vec2::ZERO;
}

/**
* Callback for a pan movement event
*
* @param event	   The associated event
* @param focus     Whether the listener currently has focus
*/
void CCMInput::panMovedCB(const cugl::PanEvent& event, bool focus) {
  if (event.fingers == NUM_PAN_FINGERS) {
    _pandelta = event.delta;
    _pandelta.y *= -1.0f;
  }
}

#pragma mark -
#pragma mark Mouse Callbacks

/**
* Called when a mouse button is initially pressed
*
* @param  event    The event storing the mouse state
* @param  clicks   The number of recent clicks, including this one
* @parm   focus	   Whether the listener currently has focus
*/
void CCMInput::mouseDownCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
	_currentTouch = event.position;
	_previousTouch = event.position;
	_mousepan = true;
}

/**
* Called when a mouse button is released
*
* @param  event    The event storing the mouse state
* @param  clicks   The number of recent clicks, including this one
* @parm   focus	   Whether the listener currently has focus
*/
void CCMInput::mouseUpCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
	_currentTouch = event.position;
	_previousTouch = event.position;
	_mousepan = false;
}

/**
* Called when the mouse is moved while held down
*
* @param  event    The event storing the mouse state
* @param  previous The previous position of the mouse
* @parm   focus	   Whether the listener currently has focus
*/
void CCMInput::mouseMovedCB(const cugl::MouseEvent& event, const Vec2& previous, bool focus) {
	if (_mousepan) {
		_currentTouch = event.position;
	}
}
