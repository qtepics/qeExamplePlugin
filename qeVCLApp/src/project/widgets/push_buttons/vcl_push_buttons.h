/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/push_buttons/vcl_push_buttons.h $
 * $Revision: #1 $
 * $DateTime: 2021/05/01 20:37:04 $
 * Last checked in by: $Author: starritt $
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

#ifndef VCL_PUSH_BUTTONS_H
#define VCL_PUSH_BUTTONS_H

#include <QWidget>
#include <QEPushButton.h>
#include <visual_component_library_global.h>

/// This module provides a number of push button widgets which are just
/// QEPushButtons, but with different default property values, specifically
/// the default style, i.e. the colour, and the default text.

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLApplyPushButton : public QEPushButton {
   Q_OBJECT
public:
   VCLApplyPushButton (QWidget* parent = 0);
   ~VCLApplyPushButton ();
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLNavigatePushButton : public QEPushButton {
   Q_OBJECT
public:
   VCLNavigatePushButton (QWidget* parent = 0);
   ~VCLNavigatePushButton ();
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLResetPushButton : public QEPushButton {
   Q_OBJECT
public:
   VCLResetPushButton (QWidget* parent = 0);
   ~VCLResetPushButton ();
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLStartPushButton : public QEPushButton {
   Q_OBJECT
public:
   VCLStartPushButton (QWidget* parent = 0);
   ~VCLStartPushButton ();
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLStopPushButton : public QEPushButton {
   Q_OBJECT
public:
   VCLStopPushButton (QWidget* parent = 0);
   ~VCLStopPushButton ();
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLValidatePushButton : public QEPushButton {
   Q_OBJECT
public:
   VCLValidatePushButton (QWidget* parent = 0);
   ~VCLValidatePushButton ();
};

#endif  // VCL_PUSH_BUTTONS_H
