/* File: qeVCLApp/src/project/widgets/filter_screen/vcl_filter_screen.h
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
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

#ifndef VCL_FILTER_SCREEN_H
#define VCL_FILTER_SCREEN_H

#include <QObject>
#include <QWidget>
#include <QEAbstractWidget.h>

#include <visual_component_library_global.h>

namespace Ui {
    class VCLFilterScreen;  // differed
}

// VCLFilterScreen is very simmilar to VCLShutter and VCLValve, therefore
// keep in sync as far as resonably possible.
// Designed to be used in conjunction with opcua_pneumatic_actuator.template
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLFilterScreen :
   public QEAbstractWidget
{
   Q_OBJECT

   Q_PROPERTY (QString  deviceName
               READ  getDeviceName
               WRITE setDeviceName)

   /// Determines if the variable values are displayed in orientation or vertical form. The default is vertical.
   ///
   Q_PROPERTY (Qt::Orientation orientation READ getOrientation WRITE setOrientation)

   /// When false, the control buttons are non-visible.
   /// Default to true.
   ///
   Q_PROPERTY (bool  enableControl
               READ  getEnableControl
               WRITE setEnableControl)

   /// Show/hide description label.
   /// By default this is true.
   ///
   Q_PROPERTY (bool showDecription READ getShowDescription WRITE setShowDescription)

   /// Show/hide status label.
   /// By default this is true.
   ///
   Q_PROPERTY (bool showStatus     READ getShowStatus      WRITE setShowStatus)

   /// Default macro substitutions. The default is no substitutions.
   /// The format is NAME1=VALUE1[,] NAME2=VALUE2...
   /// Values may be quoted strings. For example, 'PUMP=PMP3, NAME = "My Pump"'
   /// These substitutions are applied to variable names for all QE widgets.
   /// In some widgets are are also used for other purposes.
   ///
   Q_PROPERTY (QString  defaultSubstitutions
               READ  getDefaultSubstitutions
               WRITE setDefaultSubstitutions)

public:
   explicit VCLFilterScreen (QWidget* parent = 0);
   ~VCLFilterScreen ();

   void setDeviceName (const QString& deviceName);
   QString getDeviceName () const;

   void setDefaultSubstitutions (const QString& defSubs);
   QString getDefaultSubstitutions () const;

   void setOrientation (const Qt::Orientation option);
   Qt::Orientation getOrientation () const;

   void setEnableControl (const bool enableControl);
   bool getEnableControl () const;

   void setShowDescription (const bool showDecription);
   bool getShowDescription () const;

   void setShowStatus (const bool showStatus);
   bool getShowStatus () const;

private:
   // Must be consistant with the device status record out of
   // the filter_screen.template
   //
   enum DeviceStatus {
      stUnknown = 0,
      stInvalid,
      stWithdrawn,
      stInserted,
      stMoving,
      stDisconnected,        // a pseudo state
      NUMBER_DEVICE_STATES   // must be last
   };

   void setWidgetStatus ();

   Ui::VCLFilterScreen* ui;

   QString mDeviceName;
   QString mDefaultSubstitutions;
   Qt::Orientation mOrientation;
   bool mEnableControl;
   bool mShowDecription;
   bool mShowStatus;

   bool openButtonIsConnected;
   bool closeButtonIsConnected;
   bool statusIsConnected;
   DeviceStatus deviceStatus;

private slots:
   void insertButtonConnectionChanged (const bool&);
   void withdrawButtonConnectionChanged (const bool&);
   void statusValueConnectionChanged (const bool&);
   void statusValueChanged (const int&);
};

#endif  // VCL_FILTER_SCREEN_H
