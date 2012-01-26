Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.ManagePanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.managepanel',
    config: {
      pid: 'required'
    },
    constructor: function(config) {
      this.callParent(arguments);
      var properties = Ext.create('ContentModelViewer.widgets.ObjectPropertiesPanel', {
        pid: config.pid,
        region: 'north'
      });
      var datastreams = Ext.create('ContentModelViewer.widgets.DatastreamPropertiesPanel', {
        pid: config.pid,
        region: 'center'
      });
      this.add(properties);
      this.add(datastreams);
    },
    setPid: function(pid) {
      this.getComponent('properties').setPid(pid);
      this.getComponent('datastreams').setPid(pid);
    },
    itemId: 'managepanel',
    title: 'Manage',
    layout: {
      type: 'border'
    }
  });
});
