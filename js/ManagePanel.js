Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.ManagePanel', {
        extend: 'Ext.panel.Panel',
        title: 'Manage',
        layout: {
            type: 'border'
        },
        items: [{
            xtype: 'panel',
            title: 'Object Properties',
            height: 200,
            region: 'north'
        }, {
            xtype: 'panel',
            title: 'Datastreams',
            region: 'center'
        }, {
            xtype: 'panel',
            title: 'Detailed Information',
            region: 'east',
            width: 200
        }]
    });
});