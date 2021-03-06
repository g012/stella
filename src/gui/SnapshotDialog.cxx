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

#include "bspf.hxx"

#include "BrowserDialog.hxx"
#include "EditTextWidget.hxx"
#include "FSNode.hxx"
#include "LauncherDialog.hxx"
#include "PopUpWidget.hxx"
#include "Settings.hxx"

#include "SnapshotDialog.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
SnapshotDialog::SnapshotDialog(OSystem& osystem, DialogContainer& parent,
                               const GUI::Font& font)
  : Dialog(osystem, parent),
    myFont(font)
{
  const int lineHeight   = font.getLineHeight(),
            fontWidth    = font.getMaxCharWidth(),
            buttonWidth  = font.getStringWidth("Save path") + 20,
            buttonHeight = font.getLineHeight() + 4;
  const int vBorder = 10;
  int xpos, ypos, lwidth, fwidth;
  WidgetArray wid;
  ButtonWidget* b;

  // Set real dimensions
  _w = 53 * fontWidth + 8;
  _h = 10 * (lineHeight + 4) + 10;

  xpos = vBorder;  ypos = vBorder;

  // Snapshot path (save files)
  b = new ButtonWidget(this, font, xpos, ypos, buttonWidth, buttonHeight,
                       "Save path", kChooseSnapSaveDirCmd);
  wid.push_back(b);
  xpos += buttonWidth + 10;
  mySnapSavePath = new EditTextWidget(this, font, xpos, ypos + 2,
                                  _w - xpos - 10, lineHeight, "");
  wid.push_back(mySnapSavePath);

  // Snapshot path (load files)
  xpos = vBorder;  ypos += buttonHeight + 3;
  b = new ButtonWidget(this, font, xpos, ypos, buttonWidth, buttonHeight,
                       "Load path", kChooseSnapLoadDirCmd);
  wid.push_back(b);
  xpos += buttonWidth + 10;
  mySnapLoadPath = new EditTextWidget(this, font, xpos, ypos + 2,
                                  _w - xpos - 10, lineHeight, "");
  wid.push_back(mySnapLoadPath);

  // Snapshot naming
  lwidth = font.getStringWidth("Continuous snapshot interval ");
  fwidth = font.getStringWidth("internal database");
  VariantList items;
  VarList::push_back(items, "actual ROM name", "rom");
  VarList::push_back(items, "internal database", "int");
  xpos = vBorder+10;  ypos += buttonHeight + 8;
  mySnapName =
    new PopUpWidget(this, font, xpos, ypos, fwidth, lineHeight, items,
                    "Save snapshots according to ", lwidth);
  wid.push_back(mySnapName);

  // Snapshot interval (continuous mode)
  items.clear();
  VarList::push_back(items, "1 second", "1");
  VarList::push_back(items, "2 seconds", "2");
  VarList::push_back(items, "3 seconds", "3");
  VarList::push_back(items, "4 seconds", "4");
  VarList::push_back(items, "5 seconds", "5");
  VarList::push_back(items, "6 seconds", "6");
  VarList::push_back(items, "7 seconds", "7");
  VarList::push_back(items, "8 seconds", "8");
  VarList::push_back(items, "9 seconds", "9");
  VarList::push_back(items, "10 seconds", "10");
  ypos += buttonHeight;
  mySnapInterval =
    new PopUpWidget(this, font, xpos, ypos, fwidth, lineHeight, items,
                    "Continuous snapshot interval ", lwidth);
  wid.push_back(mySnapInterval);

  // Booleans for saving snapshots
  fwidth = font.getStringWidth("When saving snapshots:");
  xpos = vBorder;  ypos += buttonHeight + 5;
  new StaticTextWidget(this, font, xpos, ypos, fwidth, lineHeight,
                       "When saving snapshots:", kTextAlignLeft);

  // Snapshot single or multiple saves
  xpos += 30;  ypos += lineHeight + 3;
  mySnapSingle = new CheckboxWidget(this, font, xpos, ypos,
                                    "Overwrite existing files");
  wid.push_back(mySnapSingle);

  // Snapshot in 1x mode (ignore scaling)
  ypos += mySnapSingle->getHeight() + 4;
  mySnap1x = new CheckboxWidget(this, font, xpos, ypos,
                                "Ignore scaling (1x mode)");
  wid.push_back(mySnap1x);

  // Add Defaults, OK and Cancel buttons
  b = new ButtonWidget(this, font, 10, _h - buttonHeight - 10,
                       font.getStringWidth("Defaults") + 20, buttonHeight,
                       "Defaults", kDefaultsCmd);
  wid.push_back(b);
  addOKCancelBGroup(wid, font);

  addToFocusList(wid);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SnapshotDialog::loadConfig()
{
  const Settings& settings = instance().settings();
  mySnapSavePath->setText(settings.getString("snapsavedir"));
  mySnapLoadPath->setText(settings.getString("snaploaddir"));
  mySnapName->setSelected(instance().settings().getString("snapname"), "int");
  mySnapInterval->setSelected(instance().settings().getString("ssinterval"), "2");
  mySnapSingle->setState(settings.getBool("sssingle"));
  mySnap1x->setState(settings.getBool("ss1x"));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SnapshotDialog::saveConfig()
{
  instance().settings().setValue("snapsavedir", mySnapSavePath->getText());
  instance().settings().setValue("snaploaddir", mySnapLoadPath->getText());
  instance().settings().setValue("snapname",
    mySnapName->getSelectedTag().toString());
  instance().settings().setValue("sssingle", mySnapSingle->getState());
  instance().settings().setValue("ss1x", mySnap1x->getState());
  instance().settings().setValue("ssinterval",
    mySnapInterval->getSelectedTag().toString());

  // Flush changes to disk and inform the OSystem
  instance().saveConfig();
  instance().setConfigPaths();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SnapshotDialog::setDefaults()
{
  mySnapSavePath->setText(instance().defaultSaveDir());
  mySnapLoadPath->setText(instance().defaultLoadDir());

  mySnapSingle->setState(false);
  mySnap1x->setState(false);
  mySnapInterval->setSelected("2", "2");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SnapshotDialog::handleCommand(CommandSender* sender, int cmd,
                                   int data, int id)
{
  switch (cmd)
  {
    case kOKCmd:
      saveConfig();
      close();
      break;

    case kDefaultsCmd:
      setDefaults();
      break;

    case kChooseSnapSaveDirCmd:
      // This dialog is resizable under certain conditions, so we need
      // to re-create it as necessary
      createBrowser();
      myBrowser->show("Select snapshot save directory", mySnapSavePath->getText(),
                      BrowserDialog::Directories, kSnapSaveDirChosenCmd);
      break;

    case kChooseSnapLoadDirCmd:
      // This dialog is resizable under certain conditions, so we need
      // to re-create it as necessary
      createBrowser();
      myBrowser->show("Select snapshot load directory", mySnapLoadPath->getText(),
                      BrowserDialog::Directories, kSnapLoadDirChosenCmd);
      break;

    case kSnapSaveDirChosenCmd:
      mySnapSavePath->setText(myBrowser->getResult().getShortPath());
      break;

    case kSnapLoadDirChosenCmd:
      mySnapLoadPath->setText(myBrowser->getResult().getShortPath());
      break;

    default:
      Dialog::handleCommand(sender, cmd, data, 0);
      break;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SnapshotDialog::createBrowser()
{
  uInt32 w = 0, h = 0;
  getResizableBounds(w, h);

  // Create file browser dialog
  if(!myBrowser || uInt32(myBrowser->getWidth()) != w ||
                   uInt32(myBrowser->getHeight()) != h)
    myBrowser = make_unique<BrowserDialog>(this, myFont, w, h);
}
