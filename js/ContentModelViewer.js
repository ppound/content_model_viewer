/**
 * Display the viewer.
 */

Ext.onReady(function(){

    Ext.state.Manager.setProvider(new Ext.state.CookieProvider({
      expires: new Date(new Date().getTime()+(1000*60*60*24*7)), //7 days from now
    }));

    Ext.QuickTips.init();
    var parent = $('#tabs-tabset').parent();
    if(parent.length) {
        var content_model_viewer = $('#content-model-viewer');
        var content = content_model_viewer.remove();
        parent.empty();
        parent.append(content);    
    }
    var tabs = [];
    // Create Tabs for each of the Panels if they are defined 
    if(ContentModelViewer.widgets.CollectionPanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.CollectionPanel'));
    }
    if(ContentModelViewer.widgets.OverviewPanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.OverviewPanel'));
    }
    if(ContentModelViewer.widgets.ViewerPanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.ViewerPanel'));
    }
    if(ContentModelViewer.widgets.ManagePanel !== undefined) {
        tabs.push(Ext.create('ContentModelViewer.widgets.ManagePanel'));
    }

    //Create layout for project and add tree menu and tab panel.
    Ext.create('Ext.container.Container', {
        renderTo: 'content-model-viewer',
        frame: true,
        height: 800,
        width: 960,
        layout: {
            type: 'border',
        },
        defaults: {
            split: true,
        },
        items: [
          Ext.create('ContentModelViewer.widgets.TreePanel'), 
          {
            xtype: 'tabpanel',
            region: 'center',
            width: 760,
            height: 800,
            activeTab: 0,
            items: tabs 
        }]
    });
});
