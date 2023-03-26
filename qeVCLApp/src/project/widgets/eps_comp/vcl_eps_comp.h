/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/eps_comp/vcl_eps_comp.h $
 * $Revision: #3 $
 * $DateTime: 2023/03/26 13:41:39 $
 * Last checked in by: $Author: starritt $
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2023 Australian Synchrotron
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

#ifndef VCL_EPS_COMP_H
#define VCL_EPS_COMP_H

#include <QObject>
#include <QSize>
#include <QWidget>
#include <QEFrame.h>

#include <visual_component_library_global.h>

namespace Ui {
    class VCLEpsComp;  // differed
}

class VISUAL_COMPONENT_LIBRARY_SHARED VCLEpsComp :
   public QEFrame
{
   Q_OBJECT

   // Some QEFrame properties are really not applicable to this widget.
   // These are re-declared as DESIGNABLE false.
   // Consider spliting QEFrame into QEFrame (basic) and QEPixmapFrame - keep that for 4.1
   //
   Q_PROPERTY (bool scaledContents READ getScaledContents  WRITE setScaledContents  DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap0     READ getPixmap0Property WRITE setPixmap0Property DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap1     READ getPixmap1Property WRITE setPixmap1Property DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap2     READ getPixmap2Property WRITE setPixmap2Property DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap3     READ getPixmap3Property WRITE setPixmap3Property DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap4     READ getPixmap4Property WRITE setPixmap4Property DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap5     READ getPixmap5Property WRITE setPixmap5Property DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap6     READ getPixmap6Property WRITE setPixmap6Property DESIGNABLE false)
   Q_PROPERTY (QPixmap pixmap7     READ getPixmap7Property WRITE setPixmap7Property DESIGNABLE false)

   Q_PROPERTY (QString  deviceName
               READ  getDeviceName
               WRITE setDeviceName)

   Q_PROPERTY (QString  signalName
               READ  getSignalName
               WRITE setSignalName)

   /// Default is false
   ///
   Q_PROPERTY (bool  isStatus
               READ  getIsStatus
               WRITE setIsStatus)

   /// Default is true
   ///
   Q_PROPERTY (bool  hasEpsOosStatus
               READ  getHasEpsOosStatus
               WRITE setHasEpsOosStatus)

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
   explicit VCLEpsComp (QWidget* parent = 0);
   ~VCLEpsComp ();

   QSize sizeHint () const;

   void setDeviceName (const QString& deviceName);
   QString getDeviceName () const;

   void setSignalName (const QString& signalName);
   QString getSignalName () const;

   void setIsStatus (const bool isStatus);
   bool getIsStatus () const;

   void setHasEpsOosStatus (const bool hasEpsOos);
   bool getHasEpsOosStatus () const;

   void setDefaultSubstitutions (const QString& defSubs);
   QString getDefaultSubstitutions () const;

private:
   void setPvNames ();

   Ui::VCLEpsComp* ui;
   QString mDeviceName;
   QString mSignalName;
   bool mIsStatus;
   bool mHasEpsOosStatus;
   QString mDefaultSubstitutions;

private slots:
   void epsOosStatusChanged (const int& state);
};

#endif  // VCL_EPS_COMP_H
