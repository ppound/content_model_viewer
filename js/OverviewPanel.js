Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.OverviewPanel', {
        extend: 'Ext.panel.Panel',
        itemId: 'overview',
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
                    id: 'overview-view-file',
                    handler : function() {
                        var view = this.up('panel').down('dataview');
                        var selectionModel = view.getSelectionModel();
                        if(selectionModel.hasSelection()) {
                            var record = selectionModel.selected.first();
                            ContentModelViewer.functions.selectDatastreamRecord(record);
                            ContentModelViewer.functions.viewSelectedDatastreamRecord();
                        }
                    }
                }, {
                    xtype: 'button',
                    text: 'Download',
                    cls: 'x-btn-text-icon',
                    iconCls: 'download-datastream-icon',
                    disabled: true,
                    id: 'overview-download-file',
                    handler : function() {
                        var view = this.up('panel').down('dataview');
                        var selectionModel = view.getSelectionModel();
                        if(selectionModel.hasSelection()) {
                            var record = selectionModel.selected.first();
                            var dsid = record.get('dsid');
                            var url = ContentModelViewer.properties.url.datastream.download(dsid);
                            var form = Ext.get("datastream-download-form");
                            form.set({
                                action: url
                            });
                            document.forms["datastream-download-form"].submit();
                        }
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
                    selectionchange: function(view, selections, options) {
                        var button, record = selections[0];
                        if(record) {
                            button = Ext.getCmp('overview-view-file');
                            record.get('view') ? button.enable() : button.disable();
                            button = Ext.getCmp('overview-download-file');
                            record.get('download') ? button.enable() : button.disable();
                        }
                    } 
                }    
            }]
        }]
    });
});