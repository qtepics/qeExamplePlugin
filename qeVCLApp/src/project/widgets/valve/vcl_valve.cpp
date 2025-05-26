/* File: qeVCLApp/src/project/widgets/valve/vcl_valve.cpp
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2020-2024 Australian Synchrotron
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

#include "vcl_valve.h"
#include <ui_vcl_valve.h>
#include <QDebug>
#include <QEEnums.h>

#define DEBUG qDebug () << "vcl_valve" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
VCLValve::VCLValve (QWidget* parent) :
   QEAbstractWidget (parent),
   ui (new Ui::VCLValve ())
{
   this->ui->setupUi (this);

   this->mEnableControl = true;
   this->ui->statusImage->setSelectPixmap (0);  // invalid/undefined.

   this->setMinimumSize (this->ui->mainFrame->minimumSize ());
   this->setMaximumSize (this->ui->mainFrame->maximumSize ());

   // No variables managed directly by this widget, PV management is left to
   // the embedded QE Widgets.
   //
   this->setNumVariables (0);

   this->setVariableAsToolTip (false);
   this->setAllowDrop (false);
   this->setDisplayAlarmStateOption (QE::Never);

   this->mDeviceName = "";
   this->mDefaultSubstitutions = "";

   QObject::connect (this->ui->openButton,  SIGNAL (dbConnectionChanged          (const bool&)),
                     this,                  SLOT   (openButtonConnectionChanged  (const bool&)));

   QObject::connect (this->ui->closeButton, SIGNAL (dbConnectionChanged          (const bool&)),
                     this,                  SLOT   (closeButtonConnectionChanged (const bool&)));

   QObject::connect (this->ui->statusImage, SIGNAL (dbConnectionChanged          (const bool&)),
                     this,                  SLOT   (statusValueConnectionChanged (const bool&)));

   // Connect status value changes to local slot
   //
   QObject::connect (this->ui->statusImage, SIGNAL (dbValueChanged     (const int&)),
                     this,                  SLOT   (statusValueChanged (const int&)));


   // Set items to the disconnected state.
   //
   this->openButtonIsConnected = false;
   this->closeButtonIsConnected = false;
   this->statusIsConnected = false;
   this->deviceStatus = stDisconnected;
}

//------------------------------------------------------------------------------
//
VCLValve::~VCLValve ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
//
void VCLValve::setDeviceName (const QString& deviceName)
{
   this->mDeviceName = deviceName;
   this->ui->statusImage->setVariableNameProperty (deviceName + ":OPEN_CLOSE_STATUS");
   this->ui->openButton->setVariableNameProperty  (deviceName + ":OPEN_CLOSE_CMD");
   this->ui->closeButton->setVariableNameProperty (deviceName + ":OPEN_CLOSE_CMD");
}

//------------------------------------------------------------------------------
//
QString VCLValve::getDeviceName () const
{
   return this->mDeviceName;
}

//------------------------------------------------------------------------------
//
void VCLValve::setEnableControl (const bool enableControl)
{
   this->mEnableControl = enableControl;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
//
bool VCLValve::getEnableControl () const
{
   return this->mEnableControl;
}

//------------------------------------------------------------------------------
//
void VCLValve::setDefaultSubstitutions (const QString& defSubs)
{
   this->mDefaultSubstitutions = defSubs;

   this->ui->statusImage->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->openButton->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->closeButton->setVariableNameSubstitutionsProperty (defSubs);
}

//------------------------------------------------------------------------------
//
QString VCLValve::getDefaultSubstitutions () const
{
   return this->mDefaultSubstitutions;
}

//------------------------------------------------------------------------------
//
void VCLValve::setWidgetStatus ()
{
   // Show/hide control buttons depending on the enable control mode.
   // We use setVisible to keep designer happy, and
   // we use setRunVisible to stop user level login modifying visibility.
   //
   this->ui->openButton->setVisible (this->mEnableControl);
   this->ui->closeButton->setVisible (this->mEnableControl);
   this->ui->openButton->setRunVisible (this->mEnableControl);
   this->ui->closeButton->setRunVisible (this->mEnableControl);

   // Enable/disable control buttons depending on the state.
   //
   this->ui->openButton->setEnabled  (this->mEnableControl &&
                                      this->openButtonIsConnected &&
                                      (this->deviceStatus == stClosed));

   this->ui->closeButton->setEnabled (this->mEnableControl &&
                                      this->closeButtonIsConnected &&
                                      (this->deviceStatus == stOpen));

   DeviceStatus pmStatus = this->deviceStatus;

   if (this->statusIsConnected) {
      // If not in a known status, then select the unknown status pix map.
      //
      if ((pmStatus != stInvalid) && (pmStatus != stOpen) &&
          (pmStatus != stClosed) && (pmStatus != stMoving)) {
         pmStatus = stUnknown;
      }
   } else {
      pmStatus = stDisconnected;
   }

  this->ui->statusImage->setSelectPixmap (static_cast<int> (pmStatus));
}

//------------------------------------------------------------------------------
// slot
void VCLValve::openButtonConnectionChanged  (const bool& isConnected)
{
   this->openButtonIsConnected = isConnected;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
// slot
void VCLValve::closeButtonConnectionChanged (const bool& isConnected)
{
   this->closeButtonIsConnected = isConnected;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
// slot
void VCLValve::statusValueConnectionChanged (const bool& isConnected)
{
   this->statusIsConnected = isConnected;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
// slot
void VCLValve::statusValueChanged (const int& status)
{
   this->deviceStatus = DeviceStatus (status);
   this->setWidgetStatus ();
}

// end
