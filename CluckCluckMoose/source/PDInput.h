//
//  PDInput.cpp
//  Poly Demo
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
#ifndef __PD_INPUT_H__
#define __PD_INPUT_H__
#include <cugl/cugl.h>

/**
* This class represents player input in the poly demo.
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
class PolyInput {
private:
	/** Whether or not this input is active */
	bool _active;
	/** Whether the reset key is down */
	bool  _keyReset;
	/** Whether the exit key is down */
	bool  _keyExit;

	// TOUCH SUPPORT
	/** The last touch location for the current gesture */
	cugl::Vec2 _previousTouch;
	/** The current touch location for the current gesture */
	cugl::Vec2 _currentTouch;


protected:
	// INPUT RESULTS
	/** Whether the reset action was chosen. */
	bool _resetPressed;
	/** Whether the exit action was chosen. */
	bool _exitPressed;
	/** The panning distance from the player input */
	cugl::Vec2 _pandelta;
  
	/** Whether or not are in an active mouse pan */
	bool _mousepan;

public:
#pragma mark -
#pragma mark Constructors
	/**
	* Creates a new input controller.
	*
	* This constructor does NOT do any initialzation.  It simply allocates the
	* object. This makes it safe to use this class without a pointer.
	*/
	PolyInput(); // Don't initialize.  Allow stack based

  /**
   * Disposes of this input controller, releasing all listeners.
   */
	~PolyInput() { dispose(); }

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
	bool isActive( ) const { return _active; }

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
	void  update(float dt);

	/**
	* Clears any buffered inputs so that we may start fresh.
	*/
	void clear();

#pragma mark -
#pragma mark Input Results
	/**
	* Returns the current pan delta.
	*
	* The delta is the amount that the user has moved a two-finger touch
	* since the last animation frame. This distance is measured according
	* to the first-finger touch.
	*
	* @return The input thrust
	*/
	const cugl::Vec2& getPanDelta() { return _pandelta; }

	/**
	* Returns true if the reset button was pressed.
	*
	* @return true if the reset button was pressed.
	*/
	bool didReset() const { return _resetPressed; }

	/**
	* Returns true if the exit button was pressed.
	*
	* @return true if the exit button was pressed.
	*/
	bool didExit() const { return _exitPressed; }


#pragma mark -
#pragma mark Touch Callbacks
	/**
	* Callback for the end of a pan event
	*
	* @param event The associated event
	* @param focus     Whether the listener currently has focus
	*/
	void panEndedCB(const cugl::PanEvent& event, bool focus);

	/**
	* Callback for a pan movement event
	*
	* @param event The associated event
	* @param focus     Whether the listener currently has focus
	*/
	void panMovedCB(const cugl::PanEvent& event, bool focus);

#pragma mark -
#pragma mark Mouse Callbacks

	/**
	* Called when a mouse button is initially pressed
	*
	* This is called in addition to the touch event, as mouse events are
	* all touch events. The mousepan boolean keeps them both from
	* being processed in future events.
	*
	* @param  event    The event storing the mouse state
	* @param  clicks   The number of recent clicks, including this one
	* @parm   focus	   Whether the listener currently has focus
	*/
	void    mouseDownCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

	/**
	* Called when a mouse button is released
	*
	* This is called in addition to the touch event, as mouse events are
	* all touch events. The mousepan boolean keeps them both from
	* being processed in future events.
	*
	* @param  event    The event storing the mouse state
	* @param  clicks   The number of recent clicks, including this one
	* @parm   focus	   Whether the listener currently has focus
	*/
	void    mouseUpCB(const cugl::MouseEvent& event, Uint8 clicks, bool focus);

	/**
	* Called when the mouse moves
	*
	* @param  event    The event storing the mouse state
	* @param  previous The previous position of the mouse
	* @parm   focus	   Whether the listener currently has focus
	*/
	void    mouseMovedCB(const cugl::MouseEvent& event, const cugl::Vec2& previous, bool focus);
};

#endif /* __PD_INPUT_H__ */
