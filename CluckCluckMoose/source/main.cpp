//
//  main.cpp
//  Cornell University Game Library (CUGL)
//
//  This is the main entry class for your application.  You may need to modify
//  it slightly for your application class or platform.
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 7/1/16

// Include your application class
#include "CCMApp.h"
#include "Networking.h"
#include "android_native_app_glue.h"
#include "JNIHelper.h"

using namespace cugl;

Engine g_engine;


/*
 * Ctor
 */
Engine::Engine()
        : initialized_resources_(false),
          has_focus_(false),
          textViewFPS_(nullptr),
          app_(nullptr),
          dialog_(nullptr),
          button_sign_in_(nullptr),
          button_invite_(nullptr),
          status_text_(nullptr) {
    gl_context_ = ndk_helper::GLContext::GetInstance();
}

/*
 * Dtor
 */
Engine::~Engine() {
    jui_helper::JUIWindow::GetInstance()->Close();
    delete dialog_;
}

/**
 * Called when GPU resources need to be claimed (e.g. Textures, Shaders etc)
 * Initialize all GPU resources here.
 */
void Engine::LoadResources() {
//    renderer_.Init();
//    renderer_.Bind(&tap_camera_);
}

/**
 * Called when GPU resources need to be cleaned up when GL Context has been lost
 * Cleanup all GPU resources here
 */
void Engine::UnloadResources() {
//    renderer_.Unload();
}

/**
 * Initialize an EGL context for the current display.
 */
int Engine::InitDisplay(const int32_t cmd) {
    if (!initialized_resources_) {
        gl_context_->Init(app_->window);
        InitUI();
        LoadResources();
        initialized_resources_ = true;
    } else {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS != gl_context_->Resume(app_->window)) {
            UnloadResources();
            LoadResources();
        }

        jui_helper::JUIWindow::GetInstance()->Resume(app_->activity, cmd);
    }

    // Enable culling OpenGL state
    glEnable(GL_CULL_FACE);

    // Enabled depth test OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Note that screen size might have been changed
    glViewport(0, 0, gl_context_->GetScreenWidth(),
               gl_context_->GetScreenHeight());
//    renderer_.UpdateViewport();

    tap_camera_.SetFlip(1.f, -1.f, -1.f);
    tap_camera_.SetPinchTransformFactor(2.f, 2.f, 8.f);

    return 0;
}

/**
 * Just the current frame in the display.
 */
void Engine::DrawFrame() {
    float fFPS;
    if (monitor_.Update(fFPS)) {
        UpdateFPS(fFPS);
    }
//    renderer_.Update(monitor_.GetCurrentTime());

    // Just fill the screen with a color.
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    renderer_.Render();

    // Swap
    if (EGL_SUCCESS != gl_context_->Swap()) {
        UnloadResources();
        LoadResources();
    }
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void Engine::TermDisplay(const int32_t cmd) {
    gl_context_->Suspend();
    jui_helper::JUIWindow::GetInstance()->Suspend(cmd);
}

void Engine::TrimMemory() {
    LOGI("Trimming memory");
    gl_context_->Invalidate();
}

/**
 * Process the next input event.
 * In the handler, it passes touch events to gesture detector helper class
 * Based on gesture detection, the app moves camera, model view accordingly
 */
int32_t Engine::HandleInput(android_app *app, AInputEvent *event) {
    Engine *eng = (Engine *)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        ndk_helper::GESTURE_STATE doubleTapState =
                eng->doubletap_detector_.Detect(event);
        ndk_helper::GESTURE_STATE dragState = eng->drag_detector_.Detect(event);
        ndk_helper::GESTURE_STATE pinchState = eng->pinch_detector_.Detect(event);

        // Double tap detector has a priority over other detectors
        if (doubleTapState == ndk_helper::GESTURE_STATE_ACTION) {
            // Detect double tap
            eng->tap_camera_.Reset(true);
        } else {
            // Handle drag state
            if (dragState & ndk_helper::GESTURE_STATE_START) {
                // Otherwise, start dragging
                ndk_helper::Vec2 v;
                eng->drag_detector_.GetPointer(v);
                eng->TransformPosition(v);
                eng->tap_camera_.BeginDrag(v);
            } else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
                ndk_helper::Vec2 v;
                eng->drag_detector_.GetPointer(v);
                eng->TransformPosition(v);
                eng->tap_camera_.Drag(v);
            } else if (dragState & ndk_helper::GESTURE_STATE_END) {
                eng->tap_camera_.EndDrag();
            }

            // Handle pinch state
            if (pinchState & ndk_helper::GESTURE_STATE_START) {
                // Start new pinch
                ndk_helper::Vec2 v1;
                ndk_helper::Vec2 v2;
                eng->pinch_detector_.GetPointers(v1, v2);
                eng->TransformPosition(v1);
                eng->TransformPosition(v2);
                eng->tap_camera_.BeginPinch(v1, v2);
            } else if (pinchState & ndk_helper::GESTURE_STATE_MOVE) {
                // Multi touch
                // Start new pinch
                ndk_helper::Vec2 v1;
                ndk_helper::Vec2 v2;
                eng->pinch_detector_.GetPointers(v1, v2);
                eng->TransformPosition(v1);
                eng->TransformPosition(v2);
                eng->tap_camera_.Pinch(v1, v2);
            }
        }
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
void Engine::HandleCmd(struct android_app *app, int32_t cmd) {
    Engine *eng = (Engine *)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            if (app->window != NULL) {
                eng->InitDisplay(APP_CMD_INIT_WINDOW);
                eng->DrawFrame();
            }

            break;
        case APP_CMD_TERM_WINDOW:
            // Note that JUI helper needs to know if a window has been terminated
            eng->TermDisplay(APP_CMD_TERM_WINDOW);

            eng->has_focus_ = false;
            break;
        case APP_CMD_START:
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_RESUME:
            jui_helper::JUIWindow::GetInstance()->Resume(app->activity,
                                                         APP_CMD_RESUME);
            break;
        case APP_CMD_GAINED_FOCUS:
            // Start animation
            eng->has_focus_ = true;
            jui_helper::JUIWindow::GetInstance()->Resume(app->activity,
                                                         APP_CMD_GAINED_FOCUS);
            break;
        case APP_CMD_LOST_FOCUS:
            // Also stop animating.
            eng->has_focus_ = false;
            eng->DrawFrame();
            break;
        case APP_CMD_LOW_MEMORY:
            // Free up GL resources
            eng->TrimMemory();
            break;
        case APP_CMD_CONFIG_CHANGED:
            // Configuration changes
            eng->TermDisplay(APP_CMD_CONFIG_CHANGED);
            eng->InitDisplay(APP_CMD_CONFIG_CHANGED);
            break;
        case APP_CMD_DESTROY:
            ndk_helper::JNIHelper::GetInstance()->DetachCurrentThread();
            break;
    }
}

/*
 * Misc
 */
void Engine::SetState(android_app *state) {
    app_ = state;
    doubletap_detector_.SetConfiguration(app_->config);
    drag_detector_.SetConfiguration(app_->config);
    pinch_detector_.SetConfiguration(app_->config);
}

bool Engine::IsReady() {
    if (has_focus_) return true;

    return false;
}

void Engine::TransformPosition(ndk_helper::Vec2 &vec) {
    vec = ndk_helper::Vec2(2.0f, 2.0f) * vec /
          ndk_helper::Vec2(gl_context_->GetScreenWidth(),
                           gl_context_->GetScreenHeight()) -
          ndk_helper::Vec2(1.f, 1.f);
}

void Engine::UpdateFPS(float fFPS) {
    ndk_helper::JNIHelper::GetInstance()->RunOnUiThread([fFPS, this]() {
        const int32_t count = 64;
        char str[count];
        snprintf(str, count, "%2.2f FPS", fFPS);
        textViewFPS_->SetAttribute("Text", (const char *)str);
    });

    return;
}

/**
 * The main entry point of any CUGL application.
 *
 * This class creates the application and runs it until done.  You may
 * need to tailor this to your application, particularly the application
 * settings.  However, never modify anything below the line marked.
 *
 * @return the exit status of the application
 */
int main(int argc, char * argv[]) {
    // Change this to your application class
    CCMApp app;
    android_app *state;
    ANativeActivity *activity_;
    
    // Set the properties of your application
    app.setName("Cluck Cluck Moose");
    app.setOrganization("GDIAC");
    app.setHighDPI(true);
#if CU_PLATFORM == CU_PLATFORM_MACOS || CU_PLATFORM == CU_PLATFORM_WINDOWS
    app.setMultiSampled(true);
#endif

    //app.setFullscreen(true);
    app.setSize(288, 512);
    app.setFPS(60.0f);

//    ndk_helper::JNIHelper::Init(activity_, "com/sample/helper/NDKHelper");
    ANativeActivity_onCreate(activity_, NULL, state->savedStateSize);
    
    /// DO NOT MODIFY ANYTHING BELOW THIS LINE
    if (!app.init()) {
        return 1;
    }
    
    app.onStartup();
    while (app.step());
    app.onShutdown();

    exit(0);    // Necessary to quit on mobile devices
    return 0;   // This line is never reached
}

void android_main(android_app *state) {
    CULog("ANDROID MAIN PLS\n");
    g_engine.SetState(state);

//    // Init helper functions
//    ndk_helper::JNIHelper::Init(state->activity, HELPER_CLASS_NAME, HELPER_CLASS_SONAME);

    // Init play game services
    g_engine.InitGooglePlayGameServices();

    state->userData = &g_engine;
    state->onAppCmd = Engine::HandleCmd;
    state->onInputEvent = Engine::HandleInput;

    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int id;
        int events;
        android_poll_source *source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(g_engine.IsReady() ? 0 : -1, NULL, &events,
                                     (void **)&source)) >= 0) {
            // Process this event.
            if (source != NULL) source->process(state, source);

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                g_engine.TermDisplay(APP_CMD_TERM_WINDOW);
                return;
            }
        }

        if (g_engine.IsReady()) {
            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            g_engine.DrawFrame();
        }
    }
}