/* File: qeVCLApp/src/project/widgets/valve/vcl_valve.h
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2020-2021 Australian Synchrotron
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

#ifndef VCL_VALVE_H
#define VCL_VALVE_H

#include <QObject>
#include <QWidget>
#include <QEAbstractWidget.h>

#include <visual_component_library_global.h>

namespace Ui {
    class VCLValve;  // differed
}

// VCLValve and VCLShutter are essentially identical, apart from
// the icons used for each state - keep in sync.
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLValve :
   public QEAbstractWidget
{
   Q_OBJECT

   Q_PROPERTY (QString  deviceName
               READ  getDeviceName
               WRITE setDeviceName)

   /// Default to true.
   /// When false, the control buttons are disabled.
   ///
   Q_PROPERTY (bool  enableControl
               READ  getEnableControl
               WRITE setEnableControl)

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
   explicit VCLValve (QWidget* parent = 0);
   ~VCLValve ();

   void setDeviceName (const QString& deviceName);
   QString getDeviceName () const;

   void setEnableControl (const bool enableControl);
   bool getEnableControl () const;

   void setDefaultSubstitutions (const QString& defSubs);
   QString getDefaultSubstitutions () const;

private:
   enum DeviceStatus {
      stUnknown = 0,
      stInvalid,
      stClosed,
      stOpen,
      stMoving,
      stDisconnected    // a pseudo state
   };

   void setWidgetStatus ();

   Ui::VCLValve* ui;
   QString mDeviceName;
   bool mEnableControl;
   QString mDefaultSubstitutions;

   bool openButtonIsConnected;
   bool closeButtonIsConnected;
   bool statusIsConnected;
   DeviceStatus deviceStatus;

private slots:
   void openButtonConnectionChanged  (const bool&);
   void closeButtonConnectionChanged (const bool&);
   void statusValueConnectionChanged (const bool&);
   void statusValueChanged (const int&);
};

#endif  // VCL_VALVE_H
