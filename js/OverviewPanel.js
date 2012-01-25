Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.OverviewPanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.overviewpanel',
    config: {
      pid: 'required'
    },
    constructor: function(config) {
      this.callParent(arguments);
      var url = ContentModelViewer.properties.url;  
      var content = Ext.create('Ext.panel.Panel', {
        html: '<div>Loading...</div>',
        autoScroll: true,
        region: 'center',
        loader: {
          url: url.object.overview(config.pid),
          renderer: function(loader, response, active) {
            var json = Ext.JSON.decode(response.responseText);
            loader.getTarget().update(json.data);
            if(json.settings !== null) { // Update settings.
              jQuery.extend(Drupal.settings, json.settings);
              Drupal.attachBehaviors();
            }
            if(json.func) {
              eval(json.func)();
            }
            return true;
          },
          autoLoad: true
        }
      });
      var files = Ext.create('ContentModelViewer.widgets.FilesPanel', {
        region: 'east',
        pid: config.pid
      });
      this.add(content);
      this.add(files);
    },
    itemId: 'overview',
    title: 'Overview',
    layout: {
      type: 'border'
    }
  });
});
