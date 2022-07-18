#include "pch.h"
#include "CameraController_Pan.h"

namespace Pistacio
{

  std::tuple<Camera, CameraController_Pan> CameraController_Pan::OnUpdate(Camera camera, std::chrono::duration<float> dt, CameraController_Pan state)
  {
    #ifdef PSTC_VERBOSE_DEBUG
        PSTC_CORE_TRACE("Last frame time: {0}s ({1}ms)", dt.count(), std::chrono::duration<float, std::milli>(dt).count());
    #endif

    // New camera
    float temp = state.rotationalSpeedRemainingAfterOneSecond == 1.0 ? 1.0f : -std::log(state.rotationalSpeedRemainingAfterOneSecond);

    float dt_factor = (state.rotationalSpeedRemainingAfterOneSecond == 1 ? 1 : dt.count() * 1000);

    glm::vec3 moveVector = (float)state.mode * (camera.right * state.dx + camera.up * state.dy) / temp;
    float speed = state.mouseSpeed;
    glm::vec3 newPos = camera.position + dt_factor * speed * moveVector;

    bool valid_pos = !isnan(newPos.x) && !isnan(newPos.y) && !isnan(newPos.z);
    PSTC_ASSERT(valid_pos, "Invalid camera position!");
    
    Camera newCamera = PerspectiveCamera::NewView(camera, newPos, camera.right, camera.direction);


    CameraController_Pan newState;
    if (state.enabled)
    {
      newState = state;
      newState.dx = 0;
      newState.dy = 0;
    }
    else
    {

      // framerate independent drag
      // see: https://stackoverflow.com/questions/43960217/framerate-independent-acceleration-decceleration

      newState = state;
      double temp = std::pow(0.0001, dt.count());
      newState.dx = state.dx * temp;
      newState.dy = state.dy * temp;
      newState.rotationalSpeedRemainingAfterOneSecond = 0.0001;

    }

    return std::tuple(newCamera, newState);
    
  }

  CameraController_Pan CameraController_Pan::OnMouseMoveEvent(CameraController_Pan state, MouseMoveEvent& e)
  {

    if (!state.enabled)
      return state;

    float x = e.x / Application::Get()->GetWindow().GetWidth();
    float y = e.y / Application::Get()->GetWindow().GetHeight();

    state.rotationalSpeedRemainingAfterOneSecond = 1;

    state.dx = (x - state.lastX);
    state.dy = (state.lastY - y);

    state.lastX = x;
    state.lastY = y;

    return state;
  }

  CameraController_Pan CameraController_Pan::Enable(CameraController_Pan state)
  {
    Window& window = Application::Get()->GetWindow();
    CameraController_Pan newState;
    newState.lastX = window.GetMouseX() / window.GetWidth();
    newState.lastY = window.GetMouseY() / window.GetHeight();
    newState.enabled = true;
    return newState;
  }

  CameraController_Pan CameraController_Pan::Disable(CameraController_Pan state)
  {
    state.enabled = false;
    return state;
  }


}
