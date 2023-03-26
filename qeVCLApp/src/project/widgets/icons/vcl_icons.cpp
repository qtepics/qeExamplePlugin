/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/icons/vcl_icons.cpp $
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

#include "vcl_icons.h"
#include <QDebug>
#include <QPixmap>
#include <QTimer>

#define DEBUG qDebug () << "vcl_icons" << __LINE__ << __FUNCTION__ << "  "


//==============================================================================
//
VCLAbstactIcon::VCLAbstactIcon (const QString& imageIn, QWidget* parent) :
   QLabel (parent),
   image (imageIn)
{
   this->setMinimumSize (16, 16);
   this->setScaledContents (true);

   // Due to some Qt weirdness, if we set the image in the constructor, it
   // overrides setting the size and the sizeHint. So we set a timer to do
   // the image setup post construction.
   //
   QTimer::singleShot (0, this, SLOT (postConstruction()));
}

//------------------------------------------------------------------------------
//
void VCLAbstactIcon::postConstruction()
{
   QString imageFilename = QString (":vcl/icons/%1").arg (this->image);
   QPixmap pixMap (imageFilename);
   this->setPixmap (pixMap);
}

//------------------------------------------------------------------------------
//
VCLAbstactIcon::~VCLAbstactIcon () { }

//------------------------------------------------------------------------------
//
QSize VCLAbstactIcon::sizeHint() const
{
   return QSize (40, 40);
}


//==============================================================================
//
VCLIonPumpIcon::VCLIonPumpIcon (QWidget* parent) :
   VCLAbstactIcon ("vcl_ion_pump_icon.png", parent)
{
}

//------------------------------------------------------------------------------
//
VCLIonPumpIcon::~VCLIonPumpIcon () { }


//==============================================================================
//
VCLCameraIcon::VCLCameraIcon (QWidget* parent) :
   VCLAbstactIcon ("vcl_camera_icon.png", parent)
{ }

//------------------------------------------------------------------------------
//
VCLCameraIcon::~VCLCameraIcon () { }

// end
