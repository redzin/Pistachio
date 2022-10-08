#include "pch.h"
#include "CameraController_FPS.h"

namespace Pistachio
{

  Camera CameraController_FPS::UpdateCamera(Camera camera, std::chrono::microseconds dt, CameraController_FPS state)
  {

    //Direction (mouse movement) update
    float yawRad = glm::radians(state.yaw);
    float pitchRad = glm::radians(state.pitch);

    glm::vec3 newDirection = glm::normalize(glm::vec3(
      cos(yawRad) * cos(pitchRad),
      sin(pitchRad),
      sin(yawRad) * cos(pitchRad)
    ));

    glm::vec3 newRight = glm::vec3(
      cos(yawRad + glm::half_pi<double>()),
      0,
      sin(yawRad + glm::half_pi<double>())
    );


    // Position (key press) update
    glm::vec3 moveVector(0.0f);
    float speed = state.forwardSpeed;
  
    if (state.moveForward && !state.moveBackward)
      moveVector += camera.direction;
  
    if (state.moveLeft && !state.moveRight)
      moveVector -= camera.right;
  
    if (state.moveBackward && !state.moveForward) {
      moveVector -= camera.direction;
      speed = state.backwardsSpeed;
    }
  
    if (state.moveRight && !state.moveLeft)
      moveVector += camera.right;
  
    if (state.moveUp && !state.moveDown)
      moveVector += glm::vec3(0.0f, 1.0f, 0.0f);
  
    if (state.moveDown && !state.moveUp)
      moveVector += glm::vec3(0.0f, -1.0f, 0.0f);

    glm::vec3 newPos = camera.position;
    if (glm::length(moveVector) != 0)
    {
      moveVector = glm::normalize(moveVector);
      float time = (float)dt.count() / 1000000;
      newPos += + time * speed * moveVector;
    }


    //Validate
    bool valid_pos = !isnan(newPos.x) && !isnan(newPos.y) && !isnan(newPos.z);
    PSTC_ASSERT(valid_pos, "Invalid camera position!");

    bool valid_dir = !isnan(newDirection.x) && !isnan(newDirection.y) && !isnan(newDirection.z);
    PSTC_ASSERT(valid_dir, "Invalid camera position!");

    bool valid_right = !isnan(newRight.x) && !isnan(newRight.y) && !isnan(newRight.z);
    PSTC_ASSERT(valid_dir, "Invalid camera position!");

    return PerspectiveCamera::NewView(camera, newPos, newRight, newDirection);
    
  }

  CameraController_FPS CameraController_FPS::OnKeyEvent(CameraController_FPS state, KeyEvent& e)
  {
    using Input::KeyCode;
    using Input::ButtonAction;

    if (e.key == KeyCode::W && (e.action == ButtonAction::KeyPressed || e.action == ButtonAction::KeyRepeated)) {
      state.moveForward = true;
    }
    
    if (e.key == KeyCode::W && e.action == ButtonAction::KeyReleased) {
      state.moveForward = false;
    }
    
    if (e.key == KeyCode::A && (e.action == ButtonAction::KeyPressed || e.action == ButtonAction::KeyRepeated)) {
      state.moveLeft = true;
    }
    
    if (e.key == KeyCode::A && e.action == ButtonAction::KeyReleased) {
      state.moveLeft = false;
    }
    
    if (e.key == KeyCode::S && (e.action == ButtonAction::KeyPressed || e.action == ButtonAction::KeyRepeated)) {
      state.moveBackward = true;
    }
    
    if (e.key == KeyCode::S && e.action == ButtonAction::KeyReleased) {
      state.moveBackward = false;
    }
    
    if (e.key == KeyCode::D && (e.action == ButtonAction::KeyPressed || e.action == ButtonAction::KeyRepeated)) {
      state.moveRight = true;
    }
    
    if (e.key == KeyCode::D && e.action == ButtonAction::KeyReleased) {
      state.moveRight = false;
    }
    
    if (e.key == KeyCode::Space && (e.action == ButtonAction::KeyPressed || e.action == ButtonAction::KeyRepeated)) {
      state.moveUp = true;
    }
    
    if (e.key == KeyCode::Space && e.action == ButtonAction::KeyReleased) {
      state.moveUp = false;
    }
    
    if (e.key == KeyCode::C && (e.action == ButtonAction::KeyPressed || e.action == ButtonAction::KeyRepeated)) {
      state.moveDown = true;
    }
    
    if (e.key == KeyCode::C && e.action == ButtonAction::KeyReleased) {
      state.moveDown = false;
    }
    
    if (e.key == KeyCode::LeftControl && (e.action == ButtonAction::KeyPressed || e.action == ButtonAction::KeyRepeated)) {
      state.moveDown = true;
    }
    
    if (e.key == KeyCode::LeftControl && e.action == ButtonAction::KeyReleased) {
      state.moveDown = false;
    }
    return state;
  }

  CameraController_FPS CameraController_FPS::OnMouseMoveEvent(CameraController_FPS state, MouseMoveEvent& e)
  {

    double x = e.x;
    double y = e.y;

    double horzAngle = state.mouseSpeed * (x - state.lastX);
    double vertAngle = state.mouseSpeed * (state.lastY - y);

    state.lastX = x;
    state.lastY = y;

    state.yaw += horzAngle;
    state.pitch += vertAngle;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (state.pitch > 90.0)
      state.pitch = 90.0;
    if (state.pitch < -90.0f)
      state.pitch = -90.0f;

    if (state.yaw > 360.0)
      state.yaw -= 360;
    if (state.yaw < -360.0)
      state.yaw += 360;

    return state;
  }

  CameraController_FPS CameraController_FPS::StopMovement(CameraController_FPS state)
  {
    state.moveForward = false;
    state.moveBackward = false;
    state.moveLeft = false;
    state.moveRight = false;
    state.moveUp = false;
    state.moveDown = false;
    return state;
  }
}
