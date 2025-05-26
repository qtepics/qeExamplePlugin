/* File: qeVCLApp/src/project/widgets/stack_lights/vcl_stack_lights.h
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

#ifndef VCL_STACK_LIGHTS_H
#define VCL_STACK_LIGHTS_H

#include <QColor>
#include <QObject>
#include <QWidget>
#include <QCaObject.h>
#include <QCaAlarmInfo.h>
#include <QEAbstractWidget.h>
#include <QEIntegerFormatting.h>
#include <QESingleVariableMethods.h>
#include <visual_component_library_global.h>

/// This widger provides a graphical representation of the standard new/bright
/// beamline statck lights. The associated PV is component_name:STATUS and
/// the status is integer, the least significat 3 bits defining the on/off
/// status of the red (bit 0), the amber (bit 1) and the green (bit 2) lights.
/// Other bits are ignored.
/// //
class VISUAL_COMPONENT_LIBRARY_SHARED VCLStackLights :
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
   explicit VCLStackLights (QWidget* parent = 0);
   ~VCLStackLights ();
   QSize sizeHint () const;

signals:
   // Note, the following signals are common to many QE widgets,
   // if changing the doxygen comments, ensure relevent changes are migrated to all instances
   // These signals are emitted using the QEEmitter::emitDbValueChanged function.
   /// Sent when the widget is updated following a data change
   /// Can be used to pass on EPICS data (as presented in this widget) to other widgets.
   /// For example a QList widget could log updates from this widget.
   void dbValueChanged ();                     // signal event
   void dbValueChanged (const QString& out);   // signal as formatted text
   void dbValueChanged (const int& out);       // signal as int if applicable
   void dbValueChanged (const long& out);      // signal as long if applicable
   void dbValueChanged (const qlonglong& out); // signal as qlonglong if applicable
   void dbValueChanged (const double& out);    // signal as floating if applicable
   void dbValueChanged (const bool& out);      // signal as bool: value != 0 if applicable

   // This signal is emitted using the QEEmitter::emitDbConnectionChanged function.
   /// Sent when the widget state updated following a channel connection change
   /// Applied to provary varible.
   void dbConnectionChanged (const bool& isConnected);

protected:
   void paintEvent (QPaintEvent* event);
   void establishConnection (unsigned int variableIndex);
   qcaobject::QCaObject* createQcaItem (unsigned int variableIndex);

   // Drag and Drop
   void dragEnterEvent (QDragEnterEvent *event) { this->qcaDragEnterEvent (event); }
   void dropEvent (QDropEvent *event)           { this->qcaDropEvent ( event); }
   void mousePressEvent (QMouseEvent *event)    { this->qcaMousePressEvent (event); }
   // Use default getDrop/setDrop

   // Copy paste
   QString copyVariable ();
   QVariant copyData ();
   void paste (QVariant s);

private:
   enum Position {
      spTop = 0,
      spMid,
      spBot,
      NUMBER_OF_LIGHTS  // must be last
   };

   QColor onColour  [NUMBER_OF_LIGHTS];   // on colours
   QColor offColour [NUMBER_OF_LIGHTS];  // off colours

   QEIntegerFormatting integerFormatting;

   bool isConnected;
   bool isValid;      // true when severity is not INVALID
   int currentValue;
   QColor alarmColour;

private slots:
   void connectionChanged (QCaConnectionInfo& connectionInfo,
                           const unsigned int &variableIndex);

   void valueChanged (const long& value,
                      QCaAlarmInfo& alarmInfo,
                      QCaDateTime& dateTime,
                      const unsigned int& variableIndex);

   void useNewPvName (QString variableNameIn,
                      QString variableNameSubstitutionsIn,
                      unsigned int variableIndex);
};

#endif  // VCL_STACK_LIGHTS_H
