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
#include "CCMInput.h"

using namespace cugl;

#pragma mark Input Constants

/** The key for the event handlers */
#define LISTENER_KEY        1

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
	_select(false),
	_touchID(-1) {
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
		Touchscreen* touch = Input::get<Touchscreen>();
		touch->removeBeginListener(LISTENER_KEY);
		touch->removeEndListener(LISTENER_KEY);
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
	_timestamp.mark();
	bool success = true;

#ifndef CU_TOUCH_SCREEN
	success = Input::activate<Keyboard>();
	success = success && Input::activate<Mouse>();
	Mouse* mouse = Input::get<Mouse>();
	// Set pointer awareness to always so listening for drags registers
	// See addDragListener for an explanation
	mouse->setPointerAwareness(cugl::Mouse::PointerAwareness::ALWAYS);
	mouse->addPressListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
		this->mousePressBeganCB(event, clicks, focus);
	});
	mouse->addReleaseListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
		this->mouseReleasedCB(event, clicks, focus);
	});
	mouse->addDragListener(LISTENER_KEY, [=](const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus) {
		this->mouseDraggedCB(event, previous, focus);
	});
#else
	Touchscreen* touch = Input::get<Touchscreen>();
	touch->addBeginListener(LISTENER_KEY, [=](const cugl::TouchEvent& event, bool focus) {
		this->touchBeganCB(event, focus);
	});
	touch->addEndListener(LISTENER_KEY, [=](const cugl::TouchEvent& event, bool focus) {
		this->touchEndedCB(event, focus);
	});
	touch->addMotionListener(LISTENER_KEY, [=](const cugl::TouchEvent& event, const cugl::Vec2& previous, bool focus) {
		this->touchesMovedCB(event, previous, focus);
	});
#endif
	_active = success;
	return success;
}

/**
 * Clears any buffered inputs so that we may start fresh.
 */
void CCMInput::clear() {
	_select = false;
	_touchID = -1;

	_dtouch = Vec2::ZERO;
	_timestamp.mark();
}

#pragma mark -
#pragma mark Touch and Mouse Callbacks
/**
 * Callback for the beginning of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void CCMInput::touchBeganCB(const cugl::TouchEvent& event, bool focus) {
	// Time how long it has been since last start touch
	_timestamp = event.timestamp;
	// if there is currently no touch for a selection
	if (_touchID == -1) {
		_touchID = event.touch;
		touchBegan(event.timestamp, event.position);
	}
}

/**
 * Callback for a mouse press event.
 *
 * @param t     The touch information
 * @param event The associated event
 */
void CCMInput::mousePressBeganCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
	touchBegan(event.timestamp, event.position);
}

/**
 * Handles touchBegan and mousePress events using shared logic.
 *
 * Depending on the platform, the appropriate callback (i.e. touch or mouse) will call into this method to handle the Event.
 *
 * @param timestamp	 the timestamp of the event
 * @param pos		 the position of the touch
 */
void CCMInput::touchBegan(const cugl::Timestamp timestamp, const cugl::Vec2& pos) {
	_timestamp = timestamp;

	// Update the touch location for later gestures
	if (!_select) {
		_dtouch = pos;
	}
	_select = true;
}


/**
 * Callback for the end of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void CCMInput::touchEndedCB(const cugl::TouchEvent& event, bool focus) {
	if (event.touch == _touchID) {
		touchEnded(event.timestamp, event.position);
		_touchID = -1;
	}
}

/**
 * Callback for a mouse release event.
 *
 * @param t     The touch information
 * @param event The associated event
 */
void CCMInput::mouseReleasedCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
	touchEnded(event.timestamp, event.position);
}

/**
 * Handles touchEnded and mouseReleased events using shared logic.
 *
 * Depending on the platform, the appropriate callback (i.e. touch or mouse) will call into this method to handle the Event.
 *
 * @param timestamp	 the timestamp of the event
 * @param pos		 the position of the touch
 */
void CCMInput::touchEnded(const cugl::Timestamp timestamp, const cugl::Vec2& pos) {
	_select = false;
}

/**
 * Callback for a touch moved event.
 *
 * @param t     The touch information
 * @param event The associated event
 */
void CCMInput::touchesMovedCB(const cugl::TouchEvent& event, const Vec2& previous, bool focus) {
	if (event.touch == _touchID) {
		touchMoved(event.position);
	}
}

/**
 * Callback for a mouse drag event.
 *
 * @param t     The touch information
 * @param event The associated event
 */
void CCMInput::mouseDraggedCB(const cugl::MouseEvent& event, const Vec2& previous, bool focus) {
	touchMoved(event.position);
}

/**
 * Handles touchMoved and mouseDragged events using shared logic.
 *
 * Depending on the platform, the appropriate callback (i.e. touch or mouse) will call into this method to handle the Event.
 *
 * @param timestamp	 the timestamp of the event
 * @param pos		 the position of the touch
 */
void CCMInput::touchMoved(const cugl::Vec2& pos) {
	_dtouch.set(pos);
}
