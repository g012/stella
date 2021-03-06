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

#include "AtariVox.hxx"
#include "MT24LC256.hxx"
#include "AtariVoxWidget.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AtariVoxWidget::AtariVoxWidget(GuiObject* boss, const GUI::Font& font,
                               int x, int y, Controller& controller)
  : FlashWidget(boss, font, x, y, controller)
{
  init(boss, font, x, y);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AtariVoxWidget::eraseCurrent()
{
  AtariVox& avox = static_cast<AtariVox&>(myController);
  
  avox.myEEPROM->eraseCurrent();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AtariVoxWidget::eraseAll()
{
  AtariVox& avox = static_cast<AtariVox&>(myController);
  
  avox.myEEPROM->eraseAll();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool AtariVoxWidget::isPageDetected()
{
  AtariVox& avox = static_cast<AtariVox&>(myController);

  return avox.myEEPROM->isPageDetected();
}