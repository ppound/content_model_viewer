Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.OverviewPanel', {
        extend: 'Ext.panel.Panel',
        title: 'Overview',
        layout: {
            type: 'border'
        },
        items: [{
            xtype: 'panel',
            html: '<div>Loading...</div>',
            loader: {
                url: ContentModelViewer.properties.url.object.overview,
                renderer: 'html',
                autoLoad: true
            },
            region: 'center'
        }, Ext.create('ContentModelViewer.widgets.FilesPanel')]
    });
});