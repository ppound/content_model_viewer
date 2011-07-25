/**
 * Create Namespaces
 */
Ext.ns('ContentModelViewer');
Ext.ns('ContentModelViewer.properties');
Ext.ns('ContentModelViewer.models');
Ext.ns('ContentModelViewer.widgets');

/**
 * Grab properties from properties stored in the HTML 
 */
Ext.onReady(function(){
    /**
     * Set Properties
     */
    ContentModelViewer.properties.pid = $('#pid').text();
    ContentModelViewer.properties.url = {
        object: {
            overview: $('#object_overview_url').text(),
            properties: $('#object_properties_url').text(),
            datastreams: $('#object_datastreams_url').text(),
            purge: $('#object_purge_url').text()
        },
        datastream: {
            add: $('#datastream_add_url').text(),
            purge: function(dsid) {
                var url = $('#datastream_purge_url').text();
                return url.replace('/dsid/', '/'+dsid+'/');
            },
            properties: function(dsid) {
                var url = $('#datastream_add_url').text();
                return url.replace('/dsid/', '/'+dsid+'/');
            },
            download: function(dsid) {
                var url = $('#datastream_download_url').text();
                return url.replace('/dsid/', '/'+dsid+'/');
            },
            view: function(dsid) {
                var url = $('#datastream_view_url').text();
                return url.replace('/dsid/', '/'+dsid+'/');
            }
        }
    };
    /**
     * Define Models.
     */
    Ext.define('ContentModelViewer.models.FedoraObject', {
        extend: 'Ext.data.Model',
        fields: [{
            name: 'label',  
            type: 'string'
        }, {
            name: 'state',   
            type: 'string'
        }, {
            name: 'owner', 
            type: 'string'
        }, {
            name: 'created', 
            type: 'string'
        }, {
            name: 'modified', 
            type: 'string'
        },],
        validations: [{
            type: 'inclusion', 
            field: 'state',   
            list: ['Active', 'Inactive', 'Deleted']
        }],
        proxy: {
            type: 'rest',
            url : ContentModelViewer.properties.url.object.properties,
            reader: {
                type: 'json',
                root: 'data',
                totalProperty: 'total'
            }
        }
    });
    Ext.define('ContentModelViewer.models.Datastream', {
        extend: 'Ext.data.Model',
        idProperty: 'dsid',
        fields: [{
            name: 'dsid',  
            type: 'string'
        }, {
            name: 'label',  
            type: 'string'
        }, {
            name: 'state',   
            type: 'string'
        }, {
            name: 'created', 
            type: 'string'
        }, {
            name: 'mime', 
            type: 'string'
        }, {
            name: 'view', 
            type: 'boolean'
        }, {
            name: 'download', 
            type: 'boolean'
        }, {
            name: 'tn', 
            type: 'string'
        }],
        validations: [{
            type: 'inclusion', 
            field: 'state',   
            list: ['A', 'I']
        }],
        proxy: {
            type: 'rest',
            url : ContentModelViewer.properties.url.object.datastreams,
            reader: {
                type: 'json',
                root: 'data',
                totalProperty: 'total'
            }
        }
    });
    /**
     * Create Stores
     */
    Ext.create('Ext.data.Store', {
        storeId:'datastreams',
        model: ContentModelViewer.models.Datastream,
        autoLoad: true,
        autoSync: true,
        pageSize: 15,
        listeners: {
            write: function(store, operation) {
                var records = operation.getRecords();
                if(operation.wasSuccessful()) {
                    for (var i = 0; i < records.length; i++) {
                        var record = records[i];
                        record.commit();
                    }
                }
            }
        }
    });
    Ext.create('Ext.data.Store', {
        storeId:'objectProperties',
        model: ContentModelViewer.models.FedoraObject,
        autoLoad: true,
        listeners: {
            write: function(store, operation) {
                var records = operation.getRecords();
                if(operation.wasSuccessful()) {
                    for (var i = 0; i < records.length; i++) {
                        var record = records[i];
                        record.commit();
                    }
                }
            }
        }
    });
});