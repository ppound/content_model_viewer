Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.DatastreamViewerPanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.datastreamviewerpanel',
    autoScroll: true,
    constructor: function(config) {
      this.callParent(arguments);
      this.load(config.pid, config.dsid, config.viewFunction);
    },
    id: 'datastream-viewer', // Should never overwrite, we should only create this panel once in the viewer.
    itemId: 'datastream-viewer',
    loader: {
      url: 'undefined',
      renderer: 'html',
      successful: 'alert',
      loadMask: true
    },
    load: function(pid, dsid, viewFunction) {
      var properties = ContentModelViewer.properties;
      var url = properties.url;
      var loader = this.getLoader();
      loader.load({
        url: url.datastream.view(pid, dsid),
        success: function() {
          if(viewFunction) {
            eval(viewFunction)(pid, dsid);
          }
        }
      });
    },
    view: function(pid, dsid, viewFunction) {
      this.load(pid, dsid, viewFunction);
      var viewerPanel = this.findParentByType('viewerpanel');
      Ext.getCmp('cmvtabpanel').setActiveTab(viewerPanel);
    }
  });
});