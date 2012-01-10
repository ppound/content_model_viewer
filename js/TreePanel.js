Ext.define('ContentModelViewer.widgets.TreePanel', {
  extend: 'Ext.tree.Panel',
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
        link = record.data.link;
        window.location = link;
      }
    }
  }
});
