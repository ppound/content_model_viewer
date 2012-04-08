Ext.define('ContentModelViewer.widgets.TreePanel', {
  extend: 'Ext.tree.Panel',
viewConfig : {selectedItemCls : "even"},
  region: 'west',
  store:'treemembers',
  autoLoad: false,
  root: 'data',
  rootVisible: false,
  collapsible: true,
  collapseDirection: 'left',
  title: 'Projects',
  width: 250,
  useArrows: true,
  listeners: {
    itemclick: {
      fn: function(view, record, item, index, event) {
        //record is the data node that was clicked
        //item is the html dom element in the tree that was clicked
        //index is the index of the node relative to its parent
        //link = record.data.link;
        //window.location = link;
		var nodeId = record.data.id;
		//var node = view.getCmp(nodeId);
		var func = ContentModelViewer.functions;
		var cmv = ContentModelViewer
		var viewer = Ext.getCmp('viewerpanel'), manage = Ext.getCmp('managepanel');
                var collection = Ext.getCmp('collectionpanel');
                var pager_top = Ext.getCmp('collection-pager-top');
                var pid = record.get('pid');
		func.setCollectionPid(pid);
                var pager_top = Ext.getCmp('collection-pager-top');
                // there is an issue with the page number in the pager not refreshing automatically
		// so we move to the first page here this seems to update both pagers (top and bottom)
                // this might make more sense to put in includefirst.js in the set collectionPid function but
                // i'm not sure of the consequences of it living there
                pager_top.moveFirst();//
                //func.setFocusedPid(pid,true);
                func.addAutoCompleteJavascript();//make sure the autocomplete js is loaded
		var tabpanel = Ext.getCmp('cmvtabpanel');
		var conceptOverview = tabpanel.getComponent('concept-overview');
                if(!conceptOverview) { // Create
                    tabpanel.insert(0, Ext.create('ContentModelViewer.widgets.OverviewPanel', {
                    title:'Concept Overview',
                    itemId: 'concept-overview',
                    pid: pid
                    }));
                    conceptOverview = tabpanel.getComponent('concept-overview');
                }
		tabpanel.setActiveTab(conceptOverview);
		conceptOverview.setPid(pid);
		cmv.properties.pids.focused = pid;
		viewer.setPid(pid);manage.setPid(pid);
                
		var resourceOverview = tabpanel.getComponent('resource-overview');
		//tabpanel.remove(resourceOverview);
      }
    }
  }
});
