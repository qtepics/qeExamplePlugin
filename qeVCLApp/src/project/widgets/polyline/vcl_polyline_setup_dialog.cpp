/*  vcl_polyline_setup_dialog.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2015-2022 Australian Synchrotron
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
 *  Author:
 *    Andrew Starritt
 *  Contact details:
 *    andrew.starritt@synchrotron.org.au
 */

#include "vcl_polyline_setup_dialog.h"
#include <QtDesigner>
#include <QDebug>
#include <QStringList>
#include <QModelIndex>
#include <QModelIndexList>

#include <vcl_polyline.h>
#include <ui_vcl_polyline_setup_dialog.h>

#define DEBUG  qDebug () << "vcl_polyline_setup_dialog" << __LINE__ << __FUNCTION__ << "  "

// Used to qualify context menu actions.
//
enum Actions {
   ADD_POINT_ABOVE_ACTION = 1,
   ADD_POINT_BELOW_ACTION,
   DELETE_POINT_ACTION,
   CLEAR_ALL_POINTS_ACTION
};

//-----------------------------------------------------------------------------
// Create the dialog
//
VCLPolylineSetupDialog::VCLPolylineSetupDialog (VCLPolyline* polylineIn,
                                                QWidget* parent) :
   QDialog (parent),
   ui (new Ui::VCLPolylineSetupDialog),
   owner (polylineIn)
{
   this->ui->setupUi (this);

   this->selectedRow = -1;
   this->selectedCol = -1;

   this->layout = new QVBoxLayout (this->ui->polylineHolder);

   // Almost an exact fit - leave a bit of wiggle room.
   //
   this->layout->setContentsMargins (1, 1, 1, 1);
   this->layout->setSpacing (0);

   this->preview = new VCLPolyline (true, nullptr);
   this->preview->setObjectName ("preview");
   this->layout->addWidget (this->preview);

   QObject::connect (this->preview, SIGNAL (updateTable ()),
                     this,          SLOT   (updateTable ()));


   // Copy the points. to the preview.
   //
   this->preview->setPoints (this->owner->getPoints());

   // Copy all attributes to the preview widget.
   //
   this->preview->setIsPolygon (this->owner->getIsPolygon ());
   this->preview->setPenStyle (this->owner->getPenStyle ());
   this->preview->setPenWidth (this->owner->getPenWidth ());
   this->preview->setPenColour (this->owner->getPenColour ());
   this->preview->setBrushStyle (this->owner->getBrushStyle ());
   this->preview->setBrushColour (this->owner->getBrushColour ());
   this->preview->setGridSize (this->owner->getGridSize ());

   QTableWidget* table = this->ui->pointsTable;  // alias

   // Set up header
   //
   table->setColumnCount (2);
   for (int col = 0; col < 2; col++) {
      QTableWidgetItem* item = table->horizontalHeaderItem (col);
      if (!item) {
         item = new QTableWidgetItem ();
         table->setHorizontalHeaderItem (col, item);
      }
      item->setText (col == 0 ? "X %" : "Y %");
   }

   // Designer forces the point edit widget to be part of the layout - undo that restriction.
   //
   this->ui->verticalLayout->removeWidget (this->ui->pointEdit);
   this->ui->pointEdit->setParent (this->ui->frame);
   this->ui->pointEdit->setVisible (false);   // hide it until we need it.
   this->ui->pointEdit->installEventFilter (this);

   QObject::connect (this->ui->pointEdit, SIGNAL (valueChanged     (const double)),
                     this,                SLOT   (editValueChanged (const double)));

   this->updateTable ();

   QObject::connect (table, SIGNAL (cellClicked (int, int)),
                     this,  SLOT   (cellClicked (int, int)));

   // Setup table context menu.
   //
   table->setContextMenuPolicy (Qt::CustomContextMenu);

   QObject::connect (table, SIGNAL (customContextMenuRequested (const QPoint &)),
                     this,  SLOT   (tableContextMenuRequested  (const QPoint &)));

   QAction* action = nullptr;

   this->tableContextMenu = new QMenu (this);
   QMenu* menu = this->tableContextMenu;   // alias

   action = new QAction ("Add point above", menu);
   action->setData (QVariant (int (ADD_POINT_ABOVE_ACTION)));
   menu->addAction (action);

   action = new QAction ("Add point below", menu);
   action->setData (QVariant (int (ADD_POINT_BELOW_ACTION)));
   menu->addAction (action);

   action = new QAction ("Delete point", menu);
   action->setData (QVariant (int (DELETE_POINT_ACTION)));
   menu->addAction (action);

   action = new QAction ("Clear all points", menu);
   action->setData (QVariant (int (CLEAR_ALL_POINTS_ACTION)));
   menu->addAction (action);

   QObject::connect (menu, SIGNAL (triggered                 (QAction*)),
                     this, SLOT   (tableContextMenuTriggered (QAction*)));
}

//-----------------------------------------------------------------------------
// Destroy the dialog
//
VCLPolylineSetupDialog::~VCLPolylineSetupDialog()
{
   delete this->ui;
}

//-----------------------------------------------------------------------------
//
bool VCLPolylineSetupDialog::eventFilter (QObject* watched, QEvent* event)
{
   bool eventHasBeenFiltered = false;

   const QEvent::Type type = event->type ();

   switch (type) {

      case QEvent::KeyPress:
         if (watched == this->ui->pointEdit) {
            QKeyEvent* keyEvent = static_cast<QKeyEvent *> (event);
            int key = keyEvent->key();
            if (key == Qt::Key_Escape) {
               eventHasBeenFiltered = true;
               this->ui->pointEdit->setVisible (false);   // hide it until we need it.
               this->selectedRow = -1;
               this->preview->selectedPoint = -1;

               this->preview->update();
               this->updateTable();

               eventHasBeenFiltered = true;
            }
         }

      default:
         // ignore.
         break;
   }

   return eventHasBeenFiltered;
}

//-----------------------------------------------------------------------------
//
void VCLPolylineSetupDialog::updateTable()
{
   QTableWidget* table = this->ui->pointsTable;  // alias

   int n = this->preview->points.count();
   table->setRowCount (n);
   for (int row = 0; row < n; row++) {
      QPointF p = this->preview->points.value (row);

      for (int col = 0; col < 2; col++) {
         QTableWidgetItem* item = table->item (row, col);
         if (!item) {
            // We need to allocate item and insert it into the table.
            //
            item = new QTableWidgetItem ();
            item->setTextAlignment (Qt::AlignRight | Qt::AlignVCenter);
            item->setFlags (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            table->setItem (row, col, item);
         }

         QString image;
         image = QString ("%1 ").arg (col == 0 ?  p.x() : p.y(), 0, 'f', 2);
         item->setText (image);
      }
   }
}

//-----------------------------------------------------------------------------
//
void VCLPolylineSetupDialog::cellClicked (int row, int col)
{
   QTableWidget* table = this->ui->pointsTable;  // alias

   this->selectedRow = row;
   this->selectedCol = col;

   this->preview->selectedPoint = row;
   this->preview->update();

   const QPointF p = this->preview->points.value (row);
   const double v = col == 0 ?  p.x() : p.y();
   this->ui->pointEdit->setValue (v);
   this->ui->pointEdit->setVisible (true);
   this->ui->pointEdit->raise ();
   this->ui->pointEdit->setFocus ();

   QTableWidgetItem* item = table->item (row, col);
   QRect rect = table->visualItemRect (item);

   // Move relative to the grid.
   //
   rect.translate (table->geometry ().topLeft ());

   // And do a bit of extra shuffing and postion tweaking.
   //
   int n = this->preview->points.count();
   int shift = n >= 10 ? 24 : 15;
   rect.translate (QPoint (shift, +rect.height() - 1));
   this->ui->pointEdit->setGeometry (rect);
}

//-----------------------------------------------------------------------------
//
void VCLPolylineSetupDialog::editValueChanged (const double position)
{
   const int n = this->preview->points.count();
   if ((this->selectedRow >= 0) && (this->selectedCol < n)) {
      QPointF percent = this->preview->points.value (this->selectedRow);
      if (this->selectedCol == 0) {
         percent.setX (position);
      } else if (this->selectedCol == 1) {
         percent.setY (position);
      }

      // For snap to grid size.
      //
      this->preview->points.replace (this->selectedRow, percent);
      this->preview->snapToGrid ();
      this->preview->update ();
      this->updateTable ();
   }
}

//-----------------------------------------------------------------------------
//
void VCLPolylineSetupDialog::tableContextMenuRequested (const QPoint& pos)
{
   QPoint golbalPos = this->ui->pointsTable->mapToGlobal (pos);
   this->tableContextMenu->exec (golbalPos, 0);
}

//-----------------------------------------------------------------------------
//
void VCLPolylineSetupDialog::tableContextMenuTriggered (QAction* menuAction)
{
   QTableWidget* table = this->ui->pointsTable;  // alias

   bool okay;
   const Actions action = Actions (menuAction->data ().toInt (&okay));
   if (!okay) {
      DEBUG << "Invalid int action value";
      return;
   }

   // Find the selected row, if any.
   //
   int row = -1;  // no selection.

   QList<QTableWidgetItem*> items = table->selectedItems();
   if (items.count() > 0) {
      row = items.value(0)->row();
   }
   const int n = this->preview->points.count();

   switch (action) {
      case ADD_POINT_ABOVE_ACTION:
         // If any rows exist, then one must be selected.
         //
         if ((n == 0) || (row >= 0)) {
            this->preview->points.insert (row, QPointF (0.0, 0.0));
         }
         break;

      case ADD_POINT_BELOW_ACTION:
         if ((n == 0) || (row >= 0)) {
            this->preview->points.insert (row + 1, QPointF (0.0, 0.0));
         }
         break;

      case DELETE_POINT_ACTION:
         if (row >= 0) {
            this->preview->points.removeAt (row);
         }
         break;

      case CLEAR_ALL_POINTS_ACTION:
         this->preview->points.clear();
         break;

      default:
         DEBUG << "Unexpected action value" << action;
         return;
   }

   this->preview->update();
   this->updateTable ();
}

//-----------------------------------------------------------------------------
// User has pressed OK
//
void VCLPolylineSetupDialog::on_buttonBox_accepted ()
{
   this->owner->setPoints (this->preview->getPoints());
   this->accept ();
}

//-----------------------------------------------------------------------------
// User has pressed cancel
//
void VCLPolylineSetupDialog::on_buttonBox_rejected ()
{
   this->close ();
}

// end
