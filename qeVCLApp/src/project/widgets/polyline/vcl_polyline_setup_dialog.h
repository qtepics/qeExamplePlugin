/*  vcl_polyline_setup_dialog.h
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2015,2017 Australian Synchrotron
 *
 *  Author:
 *    Andrew Starritt
 *  Contact details:
 *    andrew.starritt@synchrotron.org.au
 */

#ifndef VCL_POLYLINE_SETUP_DIALOG_H
#define VCL_POLYLINE_SETUP_DIALOG_H

#include <QAction>
#include <QDialog>
#include <QMenu>
#include <QPoint>
#include <QVBoxLayout>
#include <vcl_polyline.h>
#include <visual_component_library_global.h>

// Differed declaration.
//
namespace Ui {
   class VCLPolylineSetupDialog;
}

class VISUAL_COMPONENT_LIBRARY_SHARED VCLPolylineSetupDialog :
      public QDialog
{
   Q_OBJECT

public:
   explicit VCLPolylineSetupDialog (VCLPolyline* owner, QWidget* parent);
   ~VCLPolylineSetupDialog ();

private:
   bool eventFilter(QObject *, QEvent *) override;

   Ui::VCLPolylineSetupDialog* ui;
   VCLPolyline* owner;
   QVBoxLayout* layout;
   VCLPolyline* preview;
   QMenu* tableContextMenu;

   int selectedRow;
   int selectedCol;

   bool acceptIsInhibited;
   bool returnIsMasked;

private slots:
   void updateTable();
   void cellClicked (int row, int col);
   void editValueChanged (const double position);
   void tableContextMenuRequested (const QPoint& pos);
   void tableContextMenuTriggered (QAction* action);
   void on_buttonBox_accepted ();
   void on_buttonBox_rejected ();
};

#endif // VCL_POLYLINE_SETUP_DIALOG_H
