Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.ViewerPanel', {
        extend: 'Ext.panel.Panel',
        title: 'Viewer',
        layout: {
            type: 'border'
        },
        items: [{
            xtype: 'panel',
            region: 'center',
            html: '<div>Loading...</div>',
            loader: {
                url: ContentModelViewer.properties.url.datastream.view(ContentModelViewer.properties.dsid),
                renderer: 'html',
                loadMask: true,
                autoLoad: true,
                success: function() {
                    var pid = ContentModelViewer.properties.pid;
                    var dsid = ContentModelViewer.properties.dsid;
                    loadFlexPlayer(pid, dsid);
                }
            }
        }, Ext.create('ContentModelViewer.widgets.FilesPanel')]
    });
});