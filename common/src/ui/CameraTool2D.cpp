/*
 Copyright (C) 2010 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CameraTool2D.h"

#include "PreferenceManager.h"
#include "Preferences.h"
#include "render/OrthographicCamera.h"
#include "ui/GestureTracker.h"
#include "ui/InputState.h"

namespace tb::ui
{
namespace
{

bool shouldZoom(const InputState& inputState)
{
  return (
    inputState.mouseButtonsPressed(MouseButtons::None)
    && inputState.modifierKeysPressed(ModifierKeys::None));
}

void zoom(
  render::OrthographicCamera& camera, const vm::vec2f& mousePos, const float factor)
{
  const auto oldWorldPos = camera.unproject(mousePos.x(), mousePos.y(), 0.0f);

  camera.zoom(factor);

  const auto newWorldPos = camera.unproject(mousePos.x(), mousePos.y(), 0.0f);
  const auto delta = newWorldPos - oldWorldPos;
  camera.moveBy(-delta);
}

class PanDragTracker : public GestureTracker
{
private:
  render::OrthographicCamera& m_camera;
  vm::vec2f m_lastMousePos;

public:
  PanDragTracker(render::OrthographicCamera& camera, const vm::vec2f& lastMousePos)
    : m_camera{camera}
    , m_lastMousePos{lastMousePos}
  {
  }

  bool update(const InputState& inputState) override
  {
    const auto currentMousePos = vm::vec2f{inputState.mouseX(), inputState.mouseY()};
    const auto lastWorldPos =
      m_camera.unproject(m_lastMousePos.x(), m_lastMousePos.y(), 0.0f);
    const auto currentWorldPos =
      m_camera.unproject(currentMousePos.x(), currentMousePos.y(), 0.0f);
    const auto delta = currentWorldPos - lastWorldPos;
    m_camera.moveBy(-delta);
    m_lastMousePos = currentMousePos;
    return true;
  }

  void end(const InputState&) override {}
  void cancel() override {}
};

class ZoomDragTracker : public GestureTracker
{
private:
  render::OrthographicCamera& m_camera;
  vm::vec2f m_lastMousePos;

public:
  ZoomDragTracker(render::OrthographicCamera& camera, const vm::vec2f& lastMousePos)
    : m_camera{camera}
    , m_lastMousePos{lastMousePos}
  {
  }

  bool update(const InputState& inputState) override
  {
    const auto speed = pref(Preferences::CameraAltMoveInvert) ? 1.0f : -1.0f;
    const auto factor = 1.0f + static_cast<float>(inputState.mouseDY()) / 100.0f * speed;
    zoom(m_camera, m_lastMousePos, factor);
    return true;
  }

  void end(const InputState&) override {}
  void cancel() override {}
};

bool shouldPan(const InputState& inputState)
{
  return (
    inputState.mouseButtonsPressed(MouseButtons::Right)
    || (inputState.mouseButtonsPressed(MouseButtons::Middle) && !pref(Preferences::CameraEnableAltMove)));
}

bool shouldDragZoom(const InputState& inputState)
{
  return (
    pref(Preferences::CameraEnableAltMove)
    && inputState.mouseButtonsPressed(MouseButtons::Middle)
    && inputState.modifierKeysPressed(ModifierKeys::Alt));
}

} // namespace

CameraTool2D::CameraTool2D(render::OrthographicCamera& camera)
  : ToolController{}
  , Tool{true}
  , m_camera{camera}
{
}

Tool& CameraTool2D::tool()
{
  return *this;
}

const Tool& CameraTool2D::tool() const
{
  return *this;
}

void CameraTool2D::mouseScroll(const InputState& inputState)
{
  if (shouldZoom(inputState))
  {
    if (inputState.scrollY() != 0.0f)
    {
      const float speed = pref(Preferences::CameraMouseWheelInvert) ? -1.0f : 1.0f;
      const float factor = 1.0f + inputState.scrollY() / 50.0f * speed;
      const auto mousePos = vm::vec2f{inputState.mouseX(), inputState.mouseY()};

      if (factor > 0.0f)
      {
        zoom(m_camera, mousePos, factor);
      }
    }
  }
}

std::unique_ptr<GestureTracker> CameraTool2D::acceptMouseDrag(
  const InputState& inputState)
{
  if (shouldPan(inputState))
  {
    const auto lastMousePos = vm::vec2f{inputState.mouseX(), inputState.mouseY()};
    return std::make_unique<PanDragTracker>(m_camera, lastMousePos);
  }

  if (shouldDragZoom(inputState))
  {
    const auto lastMousePos = vm::vec2f{inputState.mouseX(), inputState.mouseY()};
    return std::make_unique<ZoomDragTracker>(m_camera, lastMousePos);
  }

  return nullptr;
}

bool CameraTool2D::cancel()
{
  return false;
}

} // namespace tb::ui
