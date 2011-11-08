Ext.define('ContentModelViewer.widgets.TreePanel', {
    extend: 'Ext.tree.Panel',
    uses: [
        'Ext.tree.Panel'
    ],
    region: 'west', 
    store:'treemembers',
    autoLoad: false,
    root: 'data',
    rootVisible: false,
    collapsible: true,
    collapseDirection: 'left',
    title: 'Projects',
    width: 200,
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
      },
      /*itemclick: function(view, record, item, index, e){
        selected_node = record;
        if(record) {
          button = Ext.getCmp('viewer-view-file1');
          button.enable();
          //record.get('view') ? button.enable() : button.disable(); 
        }//end if
      },*/
      selectionchange: function(view, selections, options) {
        var button, record = selections[0];
        if(record) {
          button = Ext.getCmp('viewer-view-file1');
          record.get('id') ? button.enable() : button.disable();
        }   
      }
    },
    dockedItems: [{
      xtype: 'toolbar',
      dock: 'top',
      items: [{
          xtype: 'button',
          text: 'View',
          cls: 'x-btn-text-icon',
          iconCls: 'view-datastream-icon',
          disabled: true,
          id: 'viewer-view-file1',
          handler : function() {
            var record = selected_node.data.id;
            ContentModelViewer.functions.selectDatastreamRecord(record);
            ContentModelViewer.functions.viewSelectedDatastreamRecord();
          }   
        }
      ]
    }]
});
