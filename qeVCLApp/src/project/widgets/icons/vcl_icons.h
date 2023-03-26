/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/icons/vcl_icons.h $
 * $Revision: #1 $
 * $DateTime: 2021/08/07 12:27:57 $
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

#ifndef VCL_ICONS_H
#define VCL_ICONS_H

#include <QWidget>
#include <QLabel>
#include <QSize>
#include <QString>
#include <visual_component_library_global.h>

/// This module provides a number of icon widgets. They are passive and
/// provide no active functionality. The inherit directly from VCLAbstactIcon
/// which inherits directly from QLabel.
///
//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLAbstactIcon : public QLabel {
   Q_OBJECT

   // Make text property inaccessable in designer.
   //
   Q_PROPERTY (QString text READ text WRITE setText DESIGNABLE false)

public:
   explicit VCLAbstactIcon (const QString& image, QWidget* parent = 0);
   ~VCLAbstactIcon ();
   QSize sizeHint () const;

private:
   const QString image;

private slots:
   void postConstruction ();
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLIonPumpIcon : public VCLAbstactIcon {
   Q_OBJECT
public:
   explicit VCLIonPumpIcon (QWidget* parent = 0);
   ~VCLIonPumpIcon ();
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLCameraIcon : public VCLAbstactIcon {
   Q_OBJECT
public:
   explicit VCLCameraIcon (QWidget* parent = 0);
   ~VCLCameraIcon ();
};

#endif  // VCL_ICONS_H
