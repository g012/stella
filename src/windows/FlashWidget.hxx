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

#ifndef FLASH_WIDGET_HXX
#define FLASH_WIDGET_HXX

class ButtonWidget;

#include "Control.hxx"
#include "ControllerWidget.hxx"

class FlashWidget : public ControllerWidget
{
public:
  FlashWidget(GuiObject* boss, const GUI::Font& font, int x, int y,
                 Controller& controller);
  virtual ~FlashWidget() = default;

protected:
  void init(GuiObject* boss, const GUI::Font& font, int x, int y);
  void drawWidget(bool hilite) override;

private:
  ButtonWidget* myEEPROMEraseAll;
  ButtonWidget* myEEPROMEraseCurrent;
  enum
  {
    kEEPROMEraseAll = 'eeER',
    kEEPROMEraseCurrent = 'eeEC'
  };

private:  
  void loadConfig() override {}
  void handleCommand(CommandSender* sender, int cmd, int data, int id) override;
  void updateButtonState();
  
  virtual string getName() = 0;  
  virtual void eraseCurrent() = 0;
  virtual void eraseAll() = 0;
  virtual bool isPageDetected() = 0;

  // Following constructors and assignment operators not supported
  FlashWidget() = delete;
  FlashWidget(const FlashWidget&) = delete;
  FlashWidget(FlashWidget&&) = delete;
  FlashWidget& operator=(const FlashWidget&) = delete;
  FlashWidget& operator=(FlashWidget&&) = delete;
};

#endif
