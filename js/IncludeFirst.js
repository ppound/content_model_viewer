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
    ContentModelViewer.properties.dsid = $('#dsid').text();
    ContentModelViewer.properties.viewFunction = $('#view_function').text();
    ContentModelViewer.properties.url = {
        object: {
            overview: $('#object_overview_url').text(),
            properties: $('#object_properties_url').text(),
            datastreams: $('#object_datastreams_url').text(),
            members: $('#object_members_url').text(),
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
     * Define Functions
     */
    ContentModelViewer.functions = {
        selectDatastreamRecord: function(record) {
            ContentModelViewer.properties.dsid = record.get('dsid');
            ContentModelViewer.properties.viewFunction = record.get('view_function');
        },
        viewSelectedDatastreamRecord: function() {
            var viewer = Ext.getCmp('datastream-viewer');
            var loader = viewer.getLoader();
            loader.load({
                url: ContentModelViewer.properties.url.datastream.view(ContentModelViewer.properties.dsid)
            });
            var viewerPanel = viewer.up('panel');
            var tabpanel = viewer.up('tabpanel');
            tabpanel.setActiveTab(viewerPanel);
        },
        callDatastreamViewFunction: function() {
            var pid = ContentModelViewer.properties.pid;
            var dsid = ContentModelViewer.properties.dsid;
            var view_function = ContentModelViewer.properties.viewFunction;
            if(view_function) {
                eval(view_function)(pid, dsid);
            }
        }
    }
    /**
     * Define Models.
     */
    Ext.define('ContentModelViewer.models.FedoraObject', {
        extend: 'Ext.data.Model',
        fields: [{
            name: 'link',  
            type: 'string'
        }, {
            name: 'label',  
            type: 'string'
        }, {
            name: 'description',   
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
        }, {
            name: 'tn',
            type: 'string'
        }]
    });
    Ext.define('ContentModelViewer.models.ObjectProperties', {
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
        }]
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
        }, {
            name: 'view_function', 
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
        storeId:'members',
        model: ContentModelViewer.models.FedoraObject,
        autoLoad: true,
        autoSync: true,
        pageSize: 10,
        proxy: {
            type: 'ajax',
            url : ContentModelViewer.properties.url.object.members,
            reader: {
                type: 'json',
                root: 'data'
            }
        }
    });
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
        storeId:'files',
        model: ContentModelViewer.models.Datastream,
        autoLoad: true,
        pageSize: 4
    });
});