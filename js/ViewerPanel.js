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
        }, {
            xtype: 'panel',
            title: 'Files',
            width: 260,
            collapsible: true,
            split: true,
            region: 'east',
            dockedItems: [{
                xtype: 'toolbar',
                dock: 'top',
                items: [{
                    xtype: 'button',
                    text: 'View',
                    cls: 'x-btn-text-icon',
                    iconCls: 'view-datastream-icon',
                    disabled: true,
                    handler : function() {
                    
                    }
                }, {
                    xtype: 'button',
                    text: 'Download',
                    cls: 'x-btn-text-icon',
                    iconCls: 'download-datastream-icon',
                    disabled: true,
                    handler : function() {
                    
                    }
                }]
            },{
                xtype: 'pagingtoolbar',
                store: Ext.data.StoreManager.lookup('files'),   // same store GridPanel is using
                dock: 'bottom'
            }],
            items: [{
                xtype: 'dataview',
                store: Ext.data.StoreManager.lookup('files'),
                itemSelector: 'div.file-item',
                emptyText: 'No Files Availible',
                deferEmptyText: false,
                itemTpl: new Ext.XTemplate(
                    '<tpl for=".">',
                    '   <div class="file-item">',
                    '       <div class="file-item-dsid">{[fm.ellipsis(values.dsid, 30, true)]}</div>',
                    '       <img class="file-item-img file-item-show-view" src="{tn}"></img>',
                    '       <tpl if="this.showLabel(label)">',
                    '           <div class="file-item-label">{[fm.ellipsis(values.label, 30, true)]}</div>',
                    '       </tpl>',
                    '   </div>',
                    '</tpl>',
                    {
                        compiled: true,
                        disableFormats: true,
                        showLabel: function(label) {
                            return jQuery.trim(label) != '';
                        }
                    }),
                listeners: {
                    selectionchanged: function(view, selections, options) {
                        var button, record = selections[0];
                        if(record) {
                            button = Ext.getCmp('view-file');
                            record.get('view') ? button.enable() : button.disable();
                            button = Ext.getCmp('download-file');
                            record.get('download') ? button.enable() : button.disable();
                        }
                    } 
                }    
            }]
        }]
    });
});