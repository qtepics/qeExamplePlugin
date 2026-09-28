/* Repo: tec_gui/qevcl
 * File: qeVCLApp/src/project/widgets/heartbeat/vcl_heartbeat.h
 * DateTime: Thu Sep 24 16:00:58 2026
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2026 Australian Synchrotron
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

#ifndef VCL_HEARTBEAT_H
#define VCL_HEARTBEAT_H

#include <QObject>
#include <QWidget>
#include <QEAbstractWidget.h>

#include <visual_component_library_global.h>

namespace Ui {
    class VCLHeartBeat;  // differed
}

// VCLHeartBeat and VCLValve are essentially identical, apart from
// the icons used for each state - keep in sync.
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLHeartBeat :
   public QEAbstractWidget
{
   Q_OBJECT

   Q_PROPERTY (QString  iocName
               READ  getIocName
               WRITE setIocName)

   Q_PROPERTY (int   edgeWidth
               READ  getEdgeWidth
               WRITE setEdgeWidth)

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
   explicit VCLHeartBeat (QWidget* parent = 0);
   ~VCLHeartBeat ();

   void setIocName (const QString& iocName);
   QString getIocName () const;

   void setEdgeWidth (const int edgeWidth);
   int getEdgeWidth () const;

   void setDefaultSubstitutions (const QString& defSubs);
   QString getDefaultSubstitutions () const;

private:
   Ui::VCLHeartBeat* ui;
   QString mIocName;
   QString mDefaultSubstitutions;
   int mEdgeWidth;
};

#endif  // VCL_HEARTBEAT_H
