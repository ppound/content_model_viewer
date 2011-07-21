Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.ViewerPanel', {
        extend: 'Ext.panel.Panel',
        title: 'Viewer',
        layout: {
            type: 'border'
        },
        items: [{
            xtype: 'panel',
            html: '<div>viewer</div>',
            region: 'center'
        }, Ext.create('ContentModelViewer.widgets.FilesPanel')]
    });
});