Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.ViewerPanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.viewerpanel',
    constructor: function(config) {
      this.callParent(arguments);
      var viewer = Ext.create('ContentModelViewer.widgets.DatastreamViewerPanel', {
        region: 'center',
        pid: config.pid,
        dsid: config.dsid,
        viewFunction: config.viewFunction
      });
      var files = Ext.create('ContentModelViewer.widgets.FilesPanel', {
        region: 'east',
        pid: config.pid
      });
      this.add(viewer);
      this.add(files);
    },
    title: 'Viewer',
    itemId: 'viewer',
    layout: {
      type: 'border'
    }
  });
});