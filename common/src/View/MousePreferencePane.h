/*
 Copyright (C) 2010-2017 Kristian Duske
 
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
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_MousePreferencePane
#define TrenchBroom_MousePreferencePane

#include "Preferences.h"
#include "View/PreferencePane.h"

class wxButton;
class wxCheckBox;
class wxChoice;
class wxSlider;
class wxStaticText;

namespace TrenchBroom {
    namespace View {
        class KeyboardShortcut;
        class KeyboardShortcutEditor;
        class KeyboardShortcutEvent;
        
        class MousePreferencePane : public PreferencePane {
        private:
            wxSlider* m_lookSpeedSlider;
            wxCheckBox* m_invertLookHAxisCheckBox;
            wxCheckBox* m_invertLookVAxisCheckBox;
            wxSlider* m_panSpeedSlider;
            wxCheckBox* m_invertPanHAxisCheckBox;
            wxCheckBox* m_invertPanVAxisCheckBox;
            wxSlider* m_moveSpeedSlider;
            wxCheckBox* m_invertMouseWheelCheckBox;
            wxCheckBox* m_enableAltMoveCheckBox;
            wxCheckBox* m_invertAltMoveAxisCheckBox;
            wxCheckBox* m_moveInCursorDirCheckBox;
            wxSlider* m_flyLookSpeedSlider;
            wxCheckBox* m_invertFlyVAxisCheckBox;
            
            KeyboardShortcutEditor* m_forwardKeyEditor;
            KeyboardShortcutEditor* m_backwardKeyEditor;
            KeyboardShortcutEditor* m_leftKeyEditor;
            KeyboardShortcutEditor* m_rightKeyEditor;
            KeyboardShortcutEditor* m_upKeyEditor;
            KeyboardShortcutEditor* m_downKeyEditor;
            wxSlider* m_flyMoveSpeedSlider;
        public:
            MousePreferencePane(wxWindow* parent);
        private:
            void OnLookSpeedChanged(wxScrollEvent& event);
            void OnInvertLookHAxisChanged(wxCommandEvent& event);
            void OnInvertLookVAxisChanged(wxCommandEvent& event);
            
            void OnPanSpeedChanged(wxScrollEvent& event);
            void OnInvertPanHAxisChanged(wxCommandEvent& event);
            void OnInvertPanVAxisChanged(wxCommandEvent& event);
            
            void OnMoveSpeedChanged(wxScrollEvent& event);
            
            void OnInvertMouseWheelChanged(wxCommandEvent& event);
            void OnEnableAltMoveChanged(wxCommandEvent& event);
            void OnInvertAltMoveAxisChanged(wxCommandEvent& event);
            void OnMoveCameraInCursorDirChanged(wxCommandEvent& event);

            void OnFlyLookSpeedChanged(wxScrollEvent& event);
            void OnInvertFlyVAxisChanged(wxCommandEvent& event);
            
            void OnForwardKeyChanged(KeyboardShortcutEvent& event);
            void OnBackwardKeyChanged(KeyboardShortcutEvent& event);
            void OnLeftKeyChanged(KeyboardShortcutEvent& event);
            void OnRightKeyChanged(KeyboardShortcutEvent& event);
            void OnUpKeyChanged(KeyboardShortcutEvent& event);
            void OnDownKeyChanged(KeyboardShortcutEvent& event);

            bool setShortcut(const KeyboardShortcut& shortcut, Preference<KeyboardShortcut>& preference);
            bool hasConflict(const KeyboardShortcut& shortcut, const Preference<KeyboardShortcut>& preference) const;

            void OnFlyMoveSpeedChanged(wxScrollEvent& event);
        private:
            void createGui();
            wxWindow* createCameraPreferences();

            void bindEvents();
            
            bool doCanResetToDefaults() override;
            void doResetToDefaults() override;
            void doUpdateControls() override;
            bool doValidate() override;
        };
    }
}

#endif /* defined(TrenchBroom_MousePreferencePane) */
