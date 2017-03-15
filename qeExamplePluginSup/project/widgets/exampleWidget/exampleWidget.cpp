/*  exampleWidget.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the Australian Synchrotron.
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
 *  Copyright (c) 2015 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

/*
 * This class is a sample widget in a sample custom designer plugin using the QE Framework.
 * This widget is based on a QLabel and takes two EPICS PVs
 * the widget displays the difference of the PV values. It displays the difference as text,
 * plays a sound, and changes the background color according to which is larger.
 *
 * To keep this example simple the following functionality supported by the
 * QEFramework has not been included:
 *
 *    - Emiting signals on value change - see dbValueChanged() signals in QELabel.h for an example
 *    - Supporting cut/paste drag/drop - see dragEnterEvent(), dropEvent(), mousePressEvent(),
 *                                       setDrop(), getDrop(), copyVariable(), copyData(), paste()
 *                                       for an example
 */

#include <exampleWidget.h>
#include <QSound>
#include <QPalette>

/*
    Constructor with no initialisation
*/
exampleWidget::exampleWidget( QWidget *parent ) : QLabel( parent ), QEWidget( this ) {
    setup();
}

/*
    Constructor with known variable
*/
exampleWidget::exampleWidget( const QString &variableNameIn, QWidget *parent ) : QLabel( parent ), QEWidget( this )  {
    setup();
    setVariableName( variableNameIn, 0 );
    activate();
}

/*
    Setup common to all constructors
*/
void exampleWidget::setup() {

    // Initialize
    setText( "----" );
    setAutoFillBackground( true ); // Allow background colour to be changed

    for( int i = 0; i < EXAMPLEWIDGET_NUM_VARIABLES; i++ )
    {
        pvData[i] = 0.0;
        havePvData[i] = false;
    }

    // Set up data
    // This control uses a single data source
    setNumVariables(EXAMPLEWIDGET_NUM_VARIABLES);

    // Set up default properties
    setAllowDrop( false );

    // Use standard context menu
    setupContextMenu();

    // Set up the connections to recieve variable name property changes
    // The variable name property manager class only delivers an updated variable name after the user has stopped typing
    for( int i = 0; i < EXAMPLEWIDGET_NUM_VARIABLES; i++ )
    {
        variableNamePropertyManagers[i].setVariableIndex( i );
        QObject::connect( &variableNamePropertyManagers[i], SIGNAL( newVariableNameProperty( QString, QString, unsigned int ) ), this, SLOT( useNewVariableNameProperty( QString, QString, unsigned int ) ) );
    }
}

/*
    Implementation of QEWidget's virtual funtion to create the specific type of QCaObject required.
    For a label a QCaObject that streams strings is required.
*/
qcaobject::QCaObject* exampleWidget::createQcaItem( unsigned int variableIndex ) {
    // Create all items as QEFloating
   return new QEFloating( getSubstitutedVariableName( variableIndex ), this, &floatingFormatting, variableIndex );
}

/*
    Start updating.
    Implementation of VariableNameManager's virtual funtion to establish a connection to a PV as the variable name has changed.
    This function may also be used to initiate updates when loaded as a plugin.
*/
void exampleWidget::establishConnection( unsigned int variableIndex ) {

    // Create a connection.
    // If successfull, the QCaObject object that will supply data update signals will be returned
    qcaobject::QCaObject* qca = createConnection( variableIndex );

    // If a QCaObject object is now available to supply data update signals, connect it to the appropriate slots
    if(  qca ) {
        QObject::connect( qca,  SIGNAL( floatingChanged( const double&, QCaAlarmInfo&, QCaDateTime&, const unsigned int& ) ),
                          this, SLOT( useData( const double&, QCaAlarmInfo&, QCaDateTime&, const unsigned int& ) ) );

        QObject::connect( qca,  SIGNAL( connectionChanged( QCaConnectionInfo&, const unsigned int& ) ),
                          this, SLOT( connectionChanged( QCaConnectionInfo&, const unsigned int& ) ) );
    }
}

/*
    Act on a connection change.
 */
void exampleWidget::connectionChanged( QCaConnectionInfo& /*connectionInfo*/, const unsigned int& /*variableIndex*/ )
{
    // Currently ignore connection state
}

/*
    Use EPICS data
    -----------Currently ignore alarm and date/time info-----------
 */
void exampleWidget::useData( const double& numberIn, QCaAlarmInfo&, QCaDateTime&, const unsigned int& variableIndex )
{
    // Do nothing if unexpected variable
    if( variableIndex >= EXAMPLEWIDGET_NUM_VARIABLES)
    {
        return;
    }

    // Stash the data
    havePvData[variableIndex] = true;
    pvData[variableIndex] = numberIn;

    // If have all values, use them
    if( havePvData[0] && havePvData[1] )
    {
        // Calculate diference
        double value = pvData[0]-pvData[1];

        // Select a colour and make a sound if greater than zero
        Qt::GlobalColor col;
        if( value > 0.0 )
        {
            QSound::play( "alarm.wav" );
            col = Qt::red;
        }
        else
        {
            col = Qt::green;
        }

        // Set the colour
        QPalette Pal(palette());
        Pal.setColor(QPalette::Background, col);
        setPalette(Pal);

        // Display the result
        setText( QString("%1").arg( value ) );
    }
}
