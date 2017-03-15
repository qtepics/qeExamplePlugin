#include <exampleWidgetManager.h>
// ADD NEXT WIDGET HERE...

#include <exampleDesignerPlugin.h>

/*
    Add all plugins that will be provided in QT Designer.
*/
exampleWidgets::exampleWidgets(QObject *parent) : QObject(parent) {
    widgets.append(new exampleWidgetManager(this));
    // ADD NEXT WIDGET HERE...
}

/*
    Multiplugin interface.
*/
QList<QDesignerCustomWidgetInterface*> exampleWidgets::customWidgets() const {
    return widgets;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
// Publish plugins through to QT designer.
Q_EXPORT_PLUGIN2(exampleWidgetsPlugin, exampleWidgets)
#endif

// end
