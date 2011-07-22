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
            region: 'center',
            items: [Ext.create('Ext.grid.Panel', {
                selType: 'cellmodel',
                plugins: [Ext.create('Ext.grid.plugin.CellEditing', {
                    clicksToEdit: 1
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
            })]
        }, {
            xtype: 'panel',
            title: 'Detailed Information',
            region: 'east',
            width: 200
        }]
    })
});
