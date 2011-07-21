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
     * Define Models.
     */
    Ext.define('ContentModelViewer.models.FedoraObject', {
        extend: 'Ext.data.Model',
        fields: [
            {name: 'label',  type: 'string'},
            {name: 'state',   type: 'string'},
            {name: 'owner', type: 'string'},
            {name: 'created', type: 'string'},
            {name: 'modified', type: 'string'},
        ],
        validations: [
            {type: 'inclusion', field: 'state',   list: ['Active', 'Inactive', 'Deleted']},
        ]
        });
    Ext.define('ContentModelViewer.models.Datastream', {
        extend: 'Ext.data.Model',
        fields: [
            {name: 'dsid',  type: 'string'},
            {name: 'label',  type: 'string'},
            {name: 'state',   type: 'string'},
            {name: 'created', type: 'string'},
            {name: 'mime', type: 'string'},
            {name: 'view', type: 'boolean'},
            {name: 'download', type: 'boolean'},
            {name: 'tn', type: 'string'}
        ],
        validations: [
            {type: 'inclusion', field: 'state',   list: ['A', 'I']},
        ]
    });
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
});