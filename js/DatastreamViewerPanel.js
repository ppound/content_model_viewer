Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.DatastreamViewerPanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.datastreamviewerpanel',
    autoScroll: true,
    constructor: function(config) {
      this.callParent(arguments);
      // @todo first auto load.
    },
    config: {
      loader: {
        renderer: 'html',
        loadMask: true,
        autoLoad: true
      }
    },
    view: function(pid, dsid, viewFunction) {
      var properties = ContentModelViewer.properties;
      var url = properties.url;
      var loader = this.getLoader();
      loader.load({
        url: url.datastream.view(pid, dsid),
        success: viewFunction
      });
    }
  });
});