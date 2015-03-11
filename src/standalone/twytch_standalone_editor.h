/* Copyright 2013-2015 Matt Tytel
 *
 * twytch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * twytch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with twytch.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TWYTCH_STANDALONE_EDITOR_H
#define TWYTCH_STANDALONE_EDITOR_H

#include "JuceHeader.h"

#include "full_interface.h"
#include "memory.h"
#include "save_load_manager.h"
#include "twytch_engine.h"
#include "twytch_look_and_feel.h"
#include "synth_gui_interface.h"

class TwytchStandaloneEditor : public AudioAppComponent,
                               public KeyListener,
                               public MessageListener,
                               public MidiInputCallback,
                               public SaveLoadManager,
                               public SynthGuiInterface {
  public:
    TwytchStandaloneEditor();
    ~TwytchStandaloneEditor();

    void changeKeyboardOffset(int new_offset);

    // AudioAppComponent
    void prepareToPlay(int buffer_size, double sample_rate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& buffer) override;
    void releaseResources() override;
    void paint(Graphics& g) override;
    void resized() override;

    // KeyListener
    bool keyPressed(const KeyPress &key, Component *origin) override;
    bool keyStateChanged(bool isKeyDown, Component *originatingComponent) override;

    // MessageListener
    void handleMessage(const Message& message) override;

    // MidiInputCallback
    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &midi_message) override;

    // SaveLoadManager
    var stateToVar() override;
    void varToState(var state) override;

    // SynthGuiInterface
    void valueChanged(std::string name, mopo::mopo_float value) override;
    void connectModulation(mopo::ModulationConnection* connection) override;
    void disconnectModulation(mopo::ModulationConnection* connection) override;
    int getNumActiveVoices() override;
    void enterCriticalSection() { critical_section_.enter(); }
    void exitCriticalSection() { critical_section_.exit(); }

  private:
    mopo::TwytchEngine synth_;
    CriticalSection critical_section_;
    TwytchLookAndFeel look_and_feel_;
    mopo::control_map controls_;
    mopo::Memory* output_memory_;

    ScopedPointer<FullInterface> gui_;

    std::set<char> keys_pressed_;
    int computer_keyboard_offset_;
    std::unique_ptr<MidiInput> midi_input_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TwytchStandaloneEditor)
};

#endif  // TWYTCH_STANDALONE_EDITOR_H