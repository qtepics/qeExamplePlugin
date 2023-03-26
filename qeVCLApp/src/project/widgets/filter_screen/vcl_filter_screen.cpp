/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/filter_screen/vcl_filter_screen.cpp $
 * $Revision: #10 $
 * $DateTime: 2023/01/26 21:48:07 $
 * Last checked in by: $Author: starritt $
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2021-2023 Australian Synchrotron
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

#include "vcl_filter_screen.h"
#include <ui_vcl_filter_screen.h>
#include <QDebug>

#define DEBUG qDebug () << "vcl_filter_screen" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
VCLFilterScreen::VCLFilterScreen (QWidget* parent) :
   QEAbstractWidget (parent),
   ui (new Ui::VCLFilterScreen ())
{
   this->ui->setupUi (this);

   this->ui->statusImage->setSelectPixmap (0);          // invalid/undefined.
   this->setOrientation (Qt::Orientation::Horizontal);  // horizontal as default

   // No variables managed directly by this widget, PV management is left to
   // the embedded QE Widgets.
   //
   this->setNumVariables (0);

   this->setVariableAsToolTip (false);
   this->setAllowDrop (false);
   this->setDisplayAlarmStateOption (standardProperties::DISPLAY_ALARM_STATE_NEVER);

   // Set default property values.
   //
   this->mDeviceName = "";
   this->mDefaultSubstitutions = "";
   this->mOrientation = Qt::Horizontal;
   this->mEnableControl = true;
   this->mShowDecription = true;
   this->mShowStatus = true;

   QObject::connect (this->ui->insertButton,   SIGNAL (dbConnectionChanged            (const bool&)),
                     this,                     SLOT   (insertButtonConnectionChanged  (const bool&)));

   QObject::connect (this->ui->withdrawButton, SIGNAL (dbConnectionChanged             (const bool&)),
                     this,                     SLOT   (withdrawButtonConnectionChanged (const bool&)));

   QObject::connect (this->ui->statusImage,    SIGNAL (dbConnectionChanged          (const bool&)),
                     this,                     SLOT   (statusValueConnectionChanged (const bool&)));

   // Connect status value changes to local slot
   //
   QObject::connect (this->ui->statusImage,    SIGNAL (dbValueChanged     (const int&)),
                     this,                     SLOT   (statusValueChanged (const int&)));


   // Set items to the disconnected state.
   //
   this->openButtonIsConnected = false;
   this->closeButtonIsConnected = false;
   this->statusIsConnected = false;
   this->deviceStatus = stDisconnected;
}

//------------------------------------------------------------------------------
//
VCLFilterScreen::~VCLFilterScreen ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
//
void VCLFilterScreen::setDeviceName (const QString& deviceName)
{
   this->mDeviceName = deviceName;

   // This is all because framework does not issue an initial disconnted signal.
   //
   this->statusIsConnected = false;  // until we know better
   this->setWidgetStatus ();

   // These channel names must be consistant with opcua_pneumatic_actuator.template
   //
   this->ui->descriptionLabel->setVariableNameProperty (deviceName + ":NAME");
   this->ui->statusImage->setVariableNameProperty (deviceName + ":INSERT_WITHDRAW_STATUS");
   this->ui->statusLabel->setVariableNameProperty (deviceName + ":INSERT_WITHDRAW_STATUS");
   this->ui->insertButton->setVariableNameProperty  (deviceName + ":INSERT_WITHDRAW_CMD");
   this->ui->withdrawButton->setVariableNameProperty (deviceName + ":INSERT_WITHDRAW_CMD");
}

//------------------------------------------------------------------------------
//
QString VCLFilterScreen::getDeviceName () const
{
   return this->mDeviceName;
}

//------------------------------------------------------------------------------
//
void VCLFilterScreen::setDefaultSubstitutions (const QString& defSubs)
{
   this->mDefaultSubstitutions = defSubs;

   // This is all because framework does not issue an initial disconnted signal.
   //
   this->statusIsConnected = false;  // until we know better
   this->setWidgetStatus ();

   this->ui->descriptionLabel->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->statusImage->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->statusLabel->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->insertButton->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->withdrawButton->setVariableNameSubstitutionsProperty (defSubs);
}

//------------------------------------------------------------------------------
//
QString VCLFilterScreen::getDefaultSubstitutions () const
{
   return this->mDefaultSubstitutions;
}

//------------------------------------------------------------------------------
//
void VCLFilterScreen::setOrientation (const Qt::Orientation orientation)
{
   // These must be consistant with vcl_filter_screen.qrc
   //
   static const char* pixmapDirectory = ":/vcl/filter_screen";

   // This must be consistant with DeviceStatus enum.
   //
   static const char* pixmapFile [2][NUMBER_DEVICE_STATES]  = {
      { "vcl_filter_screen_r00_unknown.png",
        "vcl_filter_screen_r00_invalid.png",
        "vcl_filter_screen_r00_withdrawn.png",
        "vcl_filter_screen_r00_inserted.png",
        "vcl_filter_screen_r00_moving.png",
        "vcl_filter_screen_r00_disconnected.png" },
      { "vcl_filter_screen_r90_unknown.png",
        "vcl_filter_screen_r90_invalid.png",
        "vcl_filter_screen_r90_withdrawn.png",
        "vcl_filter_screen_r90_inserted.png",
        "vcl_filter_screen_r90_moving.png",
        "vcl_filter_screen_r90_disconnected.png"}
   };

   // First save the value.
   //
   this->mOrientation = orientation;

   // Next select the appropriate pixmaps for the status image widget.
   //
   const int i = (this->mOrientation == Qt::Orientation::Horizontal) ? 0 : 1;
   for (int j = 0; j < NUMBER_DEVICE_STATES; j++) {
      QString filename = QString ("%1/%2").arg (pixmapDirectory).arg(pixmapFile [i][j]);
      QPixmap pixmap (filename);
      this->ui->statusImage->setDataPixmap (pixmap, j);
   }

   // Lastly adjust the grid layout.
   // Remove and re-add the widgets
   //
   this->ui->gridLayout->removeWidget (this->ui->descriptionLabel);
   this->ui->gridLayout->removeWidget (this->ui->insertButton);
   this->ui->gridLayout->removeWidget (this->ui->withdrawButton);
   this->ui->gridLayout->removeWidget (this->ui->statusLabel);
   this->ui->gridLayout->removeWidget (this->ui->statusImage);

   if  (this->mOrientation == Qt::Orientation::Horizontal)  {
      this->ui->gridLayout->addWidget (this->ui->descriptionLabel, 0, 0, 1, 2);
      this->ui->gridLayout->addWidget (this->ui->insertButton,     1, 0, 1, 1);
      this->ui->gridLayout->addWidget (this->ui->withdrawButton,   1, 1, 1, 1);
      this->ui->gridLayout->addWidget (this->ui->statusLabel,      2, 0, 1, 2);
      this->ui->gridLayout->addWidget (this->ui->statusImage,      3, 0, 1, 2);
   } else {
      // Vertical is easy
      this->ui->gridLayout->addWidget (this->ui->descriptionLabel, 0, 0, 1, 1);
      this->ui->gridLayout->addWidget (this->ui->withdrawButton,   1, 0, 1, 1);
      this->ui->gridLayout->addWidget (this->ui->statusLabel,      2, 0, 1, 1);
      this->ui->gridLayout->addWidget (this->ui->statusImage,      3, 0, 1, 1);
      this->ui->gridLayout->addWidget (this->ui->insertButton,     4, 0, 1, 1);

   }
}

//------------------------------------------------------------------------------
//
Qt::Orientation VCLFilterScreen::getOrientation () const
{
   return this->mOrientation;
}

//------------------------------------------------------------------------------
//
void VCLFilterScreen::setEnableControl (const bool enableControl)
{
   this->mEnableControl = enableControl;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
//
bool VCLFilterScreen::getEnableControl () const
{
   return this->mEnableControl;
}

//------------------------------------------------------------------------------
//
void VCLFilterScreen::setShowDescription (const bool showDecription)
{
   this->mShowDecription = showDecription;

   // We use setVisible to keep designer happy, and
   // we use setRunVisible to stop user level login modifying visibility.
   //
   this->ui->descriptionLabel->setVisible (this->mShowDecription);
   this->ui->descriptionLabel->setRunVisible (this->mShowDecription);
}

//------------------------------------------------------------------------------
//
bool VCLFilterScreen::getShowDescription () const
{
    return this->mShowDecription;
}

//------------------------------------------------------------------------------
//
void VCLFilterScreen::setShowStatus (const bool showStatus)
{
   this->mShowStatus = showStatus;

   // We use setVisible to keep designer happy, and
   // we use setRunVisible to stop user level login modifying visibility.
   //
   this->ui->statusLabel->setVisible (this->mShowStatus);
   this->ui->statusLabel->setRunVisible (this->mShowStatus);
}

//------------------------------------------------------------------------------
//
bool VCLFilterScreen::getShowStatus () const
{
   return this->mShowStatus;
}

//------------------------------------------------------------------------------
//
void VCLFilterScreen::setWidgetStatus ()
{
   // Show/hide control buttons depending on the enable control mode.
   // We use setVisible to keep designer happy, and
   // we use setRunVisible to stop user level login modifying visibility.
   //
   this->ui->insertButton->setVisible (this->mEnableControl);
   this->ui->withdrawButton->setVisible (this->mEnableControl);
   this->ui->insertButton->setRunVisible (this->mEnableControl);
   this->ui->withdrawButton->setRunVisible (this->mEnableControl);

   this->ui->insertButton->setEnabled   (this->mEnableControl &&
                                         this->openButtonIsConnected  &&
                                         (this->deviceStatus == stWithdrawn));

   this->ui->withdrawButton->setEnabled (this->mEnableControl &&
                                         this->closeButtonIsConnected &&
                                         (this->deviceStatus == stInserted));

   DeviceStatus pmStatus = this->deviceStatus;

   if (!this->statusIsConnected) {
      // If disconnected, use pseudo state.
      //
      pmStatus = stDisconnected;
   }

  this->ui->statusImage->setSelectPixmap (static_cast<int> (pmStatus));
}

//------------------------------------------------------------------------------
// slot
void VCLFilterScreen::insertButtonConnectionChanged  (const bool& isConnected)
{
   this->openButtonIsConnected = isConnected;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
// slot
void VCLFilterScreen::withdrawButtonConnectionChanged (const bool& isConnected)
{
   this->closeButtonIsConnected = isConnected;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
// slot
void VCLFilterScreen::statusValueConnectionChanged (const bool& isConnected)
{
   this->statusIsConnected = isConnected;
   this->setWidgetStatus ();
}

//------------------------------------------------------------------------------
// slot
void VCLFilterScreen::statusValueChanged (const int& status)
{
   this->deviceStatus = DeviceStatus (status);
   this->setWidgetStatus ();
}

// end
