Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.ManagePanel', {
        extend: 'Ext.panel.Panel',
        title: 'Manage',
        layout: {
            type: 'border'
        },
        items: [{
            xtype: 'panel',
            region: 'north',
            height: 255,
            layout: {
                type: 'border'
            },
            items: [{
                xtype: 'form',
                title: 'Object Properties',
                height: 245,
                region: 'center',
                bodyPadding: 10,
                items: [{
                    xtype: 'textfield',
                    fieldLabel: 'Label',
                    name: 'label',
                    width: 400
                }, {
                    xtype: 'textfield',
                    fieldLabel: 'Owner',
                    name: 'owner',
                    width: 400
                }, {
                    xtype: 'combobox',
                    fieldLabel: 'State',
                    name: 'state',
                    queryMode: 'local',
                    displayField: 'name',
                    valueField: 'value',
                    value: 'I',
                    store: Ext.create('Ext.data.Store', {
                        fields: ['value', 'name'],
                        data : [{
                            value:'A', 
                            name:'Active'
                        }, {
                            value:'I', 
                            name:'Inactive'
                        }, {
                            value:'D', 
                            name:'Deleted'
                        }]
                    })
                }, {
                    xtype: 'displayfield',
                    fieldLabel: 'Date Created',
                    name: 'created',
                    width: 400
                }, {
                    xtype: 'displayfield',
                    fieldLabel: 'Last Modified',
                    name: 'modified',
                    width: 400
                }],
                buttons: [{
                    text: 'Save Changes',
                    formBind: true, // Only enabled once the form is valid
                    handler: function(button) {
                        button.up('form').getForm().submit({
                            url: ContentModelViewer.properties.url.object.properties,
                            waitMsg: 'Saving Data...',
                            success: function(form, action) {
                                var store = Ext.data.StoreManager.lookup('objectProperties');
                                var record = store.first();
                                record.set(action.result.data);
                                form.loadRecord(record);
                            }
                        });
                    }
                }],
                dockedItems: [{
                    xtype: 'toolbar',
                    dock: 'top',
                    items: [{
                        xtype: 'button',
                        text: 'Edit Permissions',
                        cls: 'x-btn-text-icon',
                        iconCls: 'edit-datastream-icon',
                        id: 'edit-xacml'
                    }, {
                        xtype: 'button',
                        text: 'Purge Object',
                        cls: 'x-btn-text-icon',
                        iconCls: 'remove-datastream-icon',
                        id: 'purge-object'
                    }]
                }],
                listeners: {
                    added: function(form) {
                        var store = Ext.data.StoreManager.lookup('objectProperties');
                        var record = store.first();
                        if(record) {
                            form.getForm().loadRecord(record);
                        }
                        else {
                            store.addListener('load', function (store) {
                                var record = store.first();
                                form.getForm().loadRecord(record);
                            });
                        }
                    }
                }
            }, {
                xtype: 'panel',
                title: 'Datastream Preview',
                region: 'east',
                width: 350
            }]
        }, {
            xtype: 'gridpanel',
            title: 'Datastreams',
            id: 'manage-panel-datastreams',
            region: 'center',
            height: '100%',
            selType: 'rowmodel',
            plugins: [Ext.create('Ext.grid.plugin.RowEditing', {
                clicksToEdit: 2
            })],
            columns: [{
                header: 'ID',  
                dataIndex: 'dsid'
            }, {
                header: 'Label', 
                dataIndex: 'label', 
                field:{
                    xtype:'textfield',
                    allowBlank:false
                },
                flex:1
            }, {
                header: 'State', 
                dataIndex: 'state',
                field:{
                    xtype:'combobox',
                    store: Ext.create('Ext.data.Store', {
                        fields: ['value', 'name'],
                        data : [{
                            "value":"A", 
                            "name":"Active"
                        }, {
                            "value":"I", 
                            "name":"Inactive"
                        }]
                    }),
                    queryMode: 'local',
                    displayField: 'name',
                    valueField: 'value',
                    value: "I"
                }
            }, {
                header: 'Mime Type', 
                dataIndex: 'mime'
            }, {
                header: 'Date Created', 
                dataIndex: 'created'
            }],
            listeners: {
                selectionchange: function(view, selections, options) {
                    var button, record = selections[0];
                    button = Ext.getCmp('remove-datastream');
                    button.enable();
                    button = Ext.getCmp('edit-datastream');
                    record.get('edit') ? button.enable() : button.disable();
                    button = Ext.getCmp('view-datastream');
                    record.get('view') ? button.enable() : button.disable();
                    button = Ext.getCmp('download-datastream');
                    record.get('download') ? button.enable() : button.disable();
                }      
            },
            store: Ext.data.StoreManager.lookup('datastreams'),
            dockedItems: [{
                xtype: 'toolbar',
                dock: 'top',
                items: [{
                    xtype: 'button',
                    text: 'Add',
                    cls: 'x-btn-text-icon',
                    iconCls: 'add-datastream-icon',
                    id: 'add-datastream',
                    handler : function() {
                        // Launch form then post a form to the given url.
                        Ext.create('Ext.window.Window', {
                            title: 'Add Datastream',
                            height: 200,
                            width: 400,
                            layout: 'fit',
                            items: {  // Let's put an empty grid in just to illustrate fit layout
                                xtype: 'form',
                                bodyPadding: 10,
                                items: [{
                                    xtype: 'textfield',
                                    fieldLabel: 'Datastream ID'
                                }, {
                                    xtype: 'textfield',
                                    fieldLabel: 'Label'
                                }, {
                                    xtype:'combobox',
                                    fieldLabel: 'State',
                                    store: Ext.create('Ext.data.Store', {
                                        fields: ['value', 'name'],
                                        data : [{
                                            "value":"A", 
                                            "name":"Active"
                                        }, {
                                            "value":"I", 
                                            "name":"Inactive"
                                        }]
                                    }),
                                    queryMode: 'local',
                                    displayField: 'name',
                                    valueField: 'value',
                                    value: "I"
                                }]
                            },
                            buttons: [{
                                text: 'Add',
                                formBind: true, // Only enabled once the form is valid
                                handler: function(button) {
                                    button.up('form').getForm().submit({
                                        url: ContentModelViewer.properties.url.datastream.add,
                                        waitMsg: 'Creating...',
                                        success: function(form, action) {
                                            var store = Ext.data.StoreManager.lookup('datastreams');
                                            store.sync();
                                        }
                                    });
                                }
                            }]
                        }).show();
                    /*
                        var url = ContentModelViewer.properties.url.datastream.add;
                        Ext.Ajax.request({
                            url: url,
                            method: 'POST',
                            success: function(response){
                                    
                            }
                        });*/
                    }
                }, {
                    xtype: 'button',
                    text: 'Remove',
                    disabled: true,
                    cls: 'x-btn-text-icon',
                    iconCls: 'remove-datastream-icon',
                    id: 'remove-datastream',
                    handler : function() {
                        var grid = this.up('gridpanel');
                        var selectionModel = grid.getSelectionModel();
                        if(selectionModel.hasSelection()) {
                            var record = selectionModel.selected.first();
                            var dsid = record.get('dsid');
                            var url = ContentModelViewer.properties.url.datastream.purge(dsid);
                            // @todo make sure the grid shows loading...
                            Ext.Ajax.request({
                                url: url,
                                method: 'POST',
                                success: function(response){
                                    
                                }
                            });
                        }
                    }
                }, {
                    xtype: 'button',
                    text: 'Edit',
                    disabled: true,
                    cls: 'x-btn-text-icon',
                    iconCls: 'edit-datastream-icon',
                    id: 'edit-datastream',
                    handler : function() {
                        var grid = this.up('gridpanel');
                        var selectionModel = grid.getSelectionModel();
                        if(selectionModel.hasSelection()) {
                            var record = selectionModel.selected.first();
                            var dsid = record.get('dsid');
                            var url = ContentModelViewer.properties.url.datastream.download(dsid);
                            Ext.Ajax.request({
                                url: url,
                                method: 'POST',
                                success: function(response){
                                    
                                }
                            });
                        }
                    }
                },  {
                    xtype: 'button',
                    text: 'Download',
                    disabled: true,
                    cls: 'x-btn-text-icon',
                    iconCls: 'download-datastream-icon',
                    id: 'download-datastream',
                    handler : function(button) {
                        var grid = this.up('gridpanel');
                        var selectionModel = grid.getSelectionModel();
                        if(selectionModel.hasSelection()) {
                            var record = selectionModel.selected.first();
                            var dsid = record.get('dsid');
                            var url = ContentModelViewer.properties.url.datastream.download(dsid);
                            Ext.Ajax.request({
                                url: url,
                                method: 'POST',
                                success: function(response){
                                    
                                }
                            });
                        }
                    }
                }, {
                    xtype: 'button',
                    text: 'View',
                    disabled: true,
                    cls: 'x-btn-text-icon',
                    iconCls: 'view-datastream-icon',
                    id: 'view-datastream',
                    handler : function() {
                        var grid = this.up('gridpanel');
                        var selectionModel = grid.getSelectionModel();
                        if(selectionModel.hasSelection()) {
                            var record = selectionModel.selected.first();
                            var dsid = record.get('dsid');
                            var url = ContentModelViewer.properties.url.datastream.download(dsid);
                            Ext.Ajax.request({
                                url: url,
                                method: 'POST',
                                success: function(response){
                                    
                                }
                            });
                        }
                    }
                }]
            },{
                xtype: 'pagingtoolbar',
                store: Ext.data.StoreManager.lookup('datastreams'),   // same store GridPanel is using
                dock: 'bottom',
                displayInfo: true
            }]
        }]
    })
});
