//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2017 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifndef ATARIVOX_WIDGET_HXX
#define ATARIVOX_WIDGET_HXX

#include "Control.hxx"
#include "FlashWidget.hxx"

class AtariVoxWidget : public FlashWidget
{
  public:
    AtariVoxWidget(GuiObject* boss, const GUI::Font& font, int x, int y,
                   Controller& controller);
    virtual ~AtariVoxWidget() = default;

 private:
    void loadConfig() override { }

    string getName()
    {
        return "AtariVox";
    }
    void eraseCurrent();
    void eraseAll();
    bool isPageDetected();

    // Following constructors and assignment operators not supported
    AtariVoxWidget() = delete;
    AtariVoxWidget(const AtariVoxWidget&) = delete;
    AtariVoxWidget(AtariVoxWidget&&) = delete;
    AtariVoxWidget& operator=(const AtariVoxWidget&) = delete;
    AtariVoxWidget& operator=(AtariVoxWidget&&) = delete;
};

#endif
