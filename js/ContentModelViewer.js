/**
 * Display the viewer.
 */

Ext.onReady(function(){
  // Local Variables
  var setup = ContentModelViewer.setup;
  var properties = ContentModelViewer.properties;
  // Init system
  setup.initContentArea();
  setup.initExtJSFeatures();
  // Create tabbed interface
  var pid = properties.pid;
  var dsid = properties.dsid;
  // Defined a major/minor pid?
  
  var tabs = [];
  // Create Tabs for each of the Panels if they are defined 
  if(ContentModelViewer.widgets.OverviewPanel !== undefined) {
    tabs.push(Ext.create('ContentModelViewer.widgets.OverviewPanel', {
      pid:pid
    }));
  }
  if(ContentModelViewer.widgets.CollectionPanel !== undefined) {
    tabs.push(Ext.create('ContentModelViewer.widgets.CollectionPanel', {
      pid:pid
    }));
  }
  if(ContentModelViewer.widgets.ViewerPanel !== undefined) {
    tabs.push(Ext.create('ContentModelViewer.widgets.ViewerPanel', {
      pid:pid,
      dsid:dsid
    }));
  }
  if(ContentModelViewer.widgets.ManagePanel !== undefined) {
    tabs.push(Ext.create('ContentModelViewer.widgets.ManagePanel', {
      pid:pid
    }));
  }

  //Create layout for project and add tree menu and tab panel.
  ContentModelViewer.container = Ext.create('Ext.container.Container', {
    renderTo: 'content-model-viewer',
    frame: true,
    height: 800,
    width: 920,
    layout: {
      type: 'border'
    },
    defaults: {
      split: true
    },
    items: [
    Ext.create('ContentModelViewer.widgets.TreePanel'), {
      xtype: 'tabpanel',
      region: 'center',
      id: 'cmvtabpanel',
      width: 760,
      height: 800,
      activeTab: 0,
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
