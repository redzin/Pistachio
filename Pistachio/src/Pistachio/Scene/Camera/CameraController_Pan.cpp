#include "pch.h"
#include "CameraController_Pan.h"

namespace Pistachio
{

  Camera CameraController_Pan::UpdateCamera(Camera camera, std::chrono::duration<float> dt, CameraController_Pan state)
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

    return PerspectiveCamera::NewView(camera, newPos, camera.right, camera.direction);

  }

  CameraController_Pan CameraController_Pan::UpdateState(std::chrono::duration<float> dt, CameraController_Pan state)
  {
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

    return newState;
  }

  CameraController_Pan CameraController_Pan::OnMouseMoveEvent(CameraController_Pan state, MouseMoveEvent& e, uint32_t windowWidth, uint32_t windowHeight)
  {

    if (!state.enabled)
      return state;

    float x = e.x / windowWidth;
    float y = e.y / windowHeight;

    state.rotationalSpeedRemainingAfterOneSecond = 1;

    state.dx = (x - state.lastX);
    state.dy = (state.lastY - y);

    state.lastX = x;
    state.lastY = y;

    return state;
  }

  CameraController_Pan CameraController_Pan::Enable(CameraController_Pan state, Scope<Window>& window)
  {
    CameraController_Pan newState;
    newState.lastX = window->GetMouseX() / window->GetWidth();
    newState.lastY = window->GetMouseY() / window->GetHeight();
    newState.enabled = true;
    return newState;
  }

  CameraController_Pan CameraController_Pan::Disable(CameraController_Pan state)
  {
    state.enabled = false;
    return state;
  }


}
