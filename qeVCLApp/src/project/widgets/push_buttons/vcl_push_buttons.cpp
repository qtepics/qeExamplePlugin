/* File: qeVCLApp/src/project/widgets/push_buttons/vcl_push_buttons.cpp
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2021 Australian Synchrotron
 *
 * The QE VCL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The QE VCL Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the QE VCL Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Original author: Andrew Starritt
 * Maintained by:   Andrew Starritt
 * Contact details: andrews@ansto.gov.au
 */

#include "vcl_push_buttons.h"
#include <QDebug>
#include <QECommon.h>
#include <QColor>

#define DEBUG qDebug () << "vcl_push_buttons" << __LINE__ << __FUNCTION__ << "  "

// Inspired bu GUI-251 and the Delphi
// //ASP/Dev/SBS/4_Controls/4_8_GUI_Frameworks/4_8_1_Delphi/sw/framework/prim/coloured_speed_buttons.pas
//
static const QColor defaultApplyColour    = QColor ("#80E880");  // Green
static const QColor defaultNavigateColour = QColor ("#B0D0FF");  // Pale Blue
static const QColor defaultResetColour    = QColor ("#FFA0FF");  // Pale Purple
static const QColor defaultStartColour    = QColor ("#A0FFA0");  // Pale Green
static const QColor defaultStopColour     = QColor ("#FFA0A0");  // Pale Red
static const QColor defaultValidateColour = QColor ("#FFFF80");  // Pale Yellow


// All the VCL buttons are essentially identical, so we just use a macro
// to implement them. Set both the actual style (for use in designer) and
// the default style (for run time use in qegui/kubili etc.)
//
#define IMPLEMENT_BUTTON_CLASS(Name,Text)                                      \
VCL##Name##PushButton::VCL##Name##PushButton (QWidget* parent) :               \
   QEPushButton (parent)                                                       \
{                                                                              \
   QString style = QEUtilities::colourToStyle (default##Name##Colour);         \
   this->setStyleSheet (style);                                                \
   this->setDefaultStyle (style);                                              \
   this->setText (Text);                                                       \
}                                                                              \
/* */                                                                          \
VCL##Name##PushButton::~VCL##Name##PushButton() { }


IMPLEMENT_BUTTON_CLASS (Apply,    "Apply")
IMPLEMENT_BUTTON_CLASS (Navigate, "Navigate")
IMPLEMENT_BUTTON_CLASS (Reset,    "Reset")
IMPLEMENT_BUTTON_CLASS (Start,    "Start")
IMPLEMENT_BUTTON_CLASS (Stop,     "Stop")
IMPLEMENT_BUTTON_CLASS (Validate, "Validate")

// end
