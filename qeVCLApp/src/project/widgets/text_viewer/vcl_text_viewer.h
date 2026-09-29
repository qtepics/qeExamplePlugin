/* Repo: tec_gui/qevcl
 * File: qeVCLApp/src/project/widgets/text_viewer/vcl_text_viewer.h
 * DateTime: Tue Sep 29 09:41:53 2026
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

#ifndef VCL_TEXT_VIEWER_H
#define VCL_TEXT_VIEWER_H

#include <QObject>
#include <QPlainTextEdit>
#include <QWidget>
#include <QEAbstractWidget.h>
#include <QEChannel.h>
#include <QEString.h>
#include <QEStringFormatting.h>
#include <QESingleVariableMethods.h>
#include <visual_component_library_global.h>

namespace Ui {
    class VCLTextViewer;  // differed
}

class VISUAL_COMPONENT_LIBRARY_SHARED VCLTextViewer :
      public QEAbstractWidget,
      public QESingleVariableMethods
{
   Q_OBJECT

   /// EPICS variable PV Name.
   ///
   Q_PROPERTY (QString variable
               READ getVariableNameProperty   // defined in QESingleVariableMethods
               WRITE setVariableNameProperty) // defined in QESingleVariableMethods

   /// Default macro substitutions. The default is no substitutions.
   /// The format is NAME1=VALUE1[,] NAME2=VALUE2...
   /// Values may be quoted strings. For example, 'PUMP=PMP3, NAME = "My Pump"'
   /// These substitutions are applied to variable names for all QE widgets.
   /// In some widgets are are also used for other purposes.
   ///
   Q_PROPERTY (QString defaultSubstitutions
               READ  getVariableNameSubstitutionsProperty
               WRITE setVariableNameSubstitutionsProperty)

public:
   explicit VCLTextViewer (QWidget* parent = 0);
   ~VCLTextViewer ();

protected:
   void establishConnection (unsigned int variableIndex);
   QEChannel* createQcaItem (unsigned int variableIndex);

private:
   Ui::VCLTextViewer* ui;
   QEStringFormatting stringFormatting;

private slots:
   void usePvNameProperties (const QEPvNameProperties&);

   void connectionUpdated (const QEConnectionUpdate&);

   void valueUpdated (const QEStringValueUpdate&);
};

#endif  // VCL_TEXT_VIEWER_H
