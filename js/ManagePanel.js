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
            store: Ext.data.StoreManager.lookup('datastreams'),
            dockedItems: [{
                xtype: 'toolbar',
                dock: 'top',
                items: [{
                    xtype: 'button',
                    text: 'Add',
                    cls: 'x-btn-text-icon',
                    iconCls: 'add-datastream-icon',
                    id: 'add-datastream'
                }, {
                    xtype: 'button',
                    text: 'Remove',
                    cls: 'x-btn-text-icon',
                    iconCls: 'remove-datastream-icon',
                    id: 'remove-datastream'
                }, {
                    xtype: 'button',
                    text: 'Edit',
                    cls: 'x-btn-text-icon',
                    iconCls: 'edit-datastream-icon',
                    id: 'edit-datastream'
                },  {
                    xtype: 'button',
                    text: 'Download',
                    cls: 'x-btn-text-icon',
                    iconCls: 'download-datastream-icon',
                    id: 'download-datastream'
                }, {
                    xtype: 'button',
                    text: 'View',
                    cls: 'x-btn-text-icon',
                    iconCls: 'view-datastream-icon',
                    id: 'view-datastream'
                }, {
                    xtype: 'button',
                    text: 'Save',
                    id: 'save-datastream', 
                    handler : function() {
                        var store = Ext.data.StoreManager.lookup('datastreams');
                        store.sync();
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
