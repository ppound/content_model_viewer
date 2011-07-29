/**
 * Display the viewer.
 */
Ext.onReady(function(){
    Ext.QuickTips.init();
    var tabs = [];
    // Create Tabs for each of the Panels if they are defined 
    if(ContentModelViewer.widgets.OverviewPanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.OverviewPanel'));
    }
    if(ContentModelViewer.widgets.CollectionPanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.CollectionPanel'));
    }
    if(ContentModelViewer.widgets.ViewerPanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.ViewerPanel'));
    }
    if(ContentModelViewer.widgets.ManagePanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.ManagePanel'));
    }
    var viewer = Ext.create('Ext.tab.Panel', {
        width: 740,
        height: 800,
        renderTo: 'content-model-viewer',
        items: tabs
    });
    viewer.show();
});