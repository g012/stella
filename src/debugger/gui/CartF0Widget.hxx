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

#ifndef CARTRIDGEF0_WIDGET_HXX
#define CARTRIDGEF0_WIDGET_HXX

class CartridgeF0;
class PopUpWidget;

#include "CartDebugWidget.hxx"

class CartridgeF0Widget : public CartDebugWidget
{
  public:
    CartridgeF0Widget(GuiObject* boss, const GUI::Font& lfont,
                      const GUI::Font& nfont,
                      int x, int y, int w, int h,
                      CartridgeF0& cart);
    virtual ~CartridgeF0Widget() = default;

  private:
    CartridgeF0& myCart;
    PopUpWidget* myBank;

    enum { kBankChanged = 'bkCH' };

  private:
    void loadConfig() override;
    void handleCommand(CommandSender* sender, int cmd, int data, int id) override;

    string bankState() override;

    // Following constructors and assignment operators not supported
    CartridgeF0Widget() = delete;
    CartridgeF0Widget(const CartridgeF0Widget&) = delete;
    CartridgeF0Widget(CartridgeF0Widget&&) = delete;
    CartridgeF0Widget& operator=(const CartridgeF0Widget&) = delete;
    CartridgeF0Widget& operator=(CartridgeF0Widget&&) = delete;
};

#endif
