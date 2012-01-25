/**
 * Display the viewer.
 */

Ext.onReady(function(){
  // Local Variables
  var setup = ContentModelViewer.setup;
  var properties = ContentModelViewer.properties;
  var widgets = ContentModelViewer.widgets;
  var pid = properties.pid,
  collectionPid = properties.pids.collection, 
  focusedPid = properties.pids.focused, 
  dsid = properties.dsid;
  
  // Init system
  setup.initContentArea();
  setup.initExtJSFeatures();
  /**
   * @todo set the Collection PID/Focuse PID from #URL
   * Assuming we can get the Collection PID, and Focus PID from the #URL
   */
  var tabs = [];
  if(widgets.OverviewPanel !== undefined) {
    if(properties.isCollection) {
      tabs.push(Ext.create('ContentModelViewer.widgets.OverviewPanel', {
        title:'Concept Overview',
        itemId: 'concept-overview',
        pid: pid
      }));
    }
  }
  if(widgets.CollectionPanel !== undefined) {
    tabs.push(Ext.create('ContentModelViewer.widgets.CollectionPanel', {
      pid: collectionPid
    }));
  }
  if(widgets.OverviewPanel !== undefined) {
    if(!properties.isCollection) {
      tabs.push(Ext.create('ContentModelViewer.widgets.OverviewPanel', {
        title:'Resource Overview',
        itemId: 'resource-overview',
        pid: focusedPid
      }));
    }
  }
  if(widgets.ViewerPanel !== undefined) {
    tabs.push(Ext.create('ContentModelViewer.widgets.ViewerPanel', {
      pid:focusedPid,
      dsid:dsid
    }));
  }
  if(widgets.ManagePanel !== undefined) {
    tabs.push(Ext.create('ContentModelViewer.widgets.ManagePanel', {
      pid:focusedPid
    }));
  }
  // Create container for Application
  ContentModelViewer.container =  Ext.create('Ext.container.Container', {
    renderTo: 'content-model-viewer',
    height: 800,
    width: 920,
    frame: true,
    defaults: {
      split: true
    },
    layout: {
      type: 'border'
    },
    items: [Ext.create('ContentModelViewer.widgets.TreePanel'), {
      xtype: 'tabpanel',
      region: 'center',
      id: 'cmvtabpanel',
      width: 760,
      height: 800,
      items: tabs,
      listeners: {
        afterrender: function(){
          var token = window.location.hash.substr(1);
          if ( token && this.isVisible(token)) {
            this.setActiveTab(token);
          }
        }
      } 
    }]
  }); 
  setup.setUpGlobalEvents();
});
