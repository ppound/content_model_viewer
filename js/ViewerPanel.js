Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.ViewerPanel', {
        extend: 'Ext.panel.Panel',
        itemId: 'viewer',
        title: 'Viewer',
        layout: {
            type: 'border'
        },
        items: [{
                xtype: 'panel',
                region: 'center',
                id: 'datastream-viewer',
                loader: {
                    url: ContentModelViewer.properties.url.datastream.view(ContentModelViewer.properties.dsid),
                    renderer: 'html',
                    loadMask: true,
                    autoLoad: true,
                    success: function() {
                        ContentModelViewer.functions.callDatastreamViewFunction();
                    }
                }
            }, Ext.create('ContentModelViewer.widgets.FilesPanel')]
    });
});