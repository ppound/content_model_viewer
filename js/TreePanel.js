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
    width: 250,
    useArrows: true,
    listeners: {
      itemdblclick: {
        fn: function(view, record, item, index, event) {
            //record is the data node that was clicked
            //item is the html dom element in the tree that was clicked
            //index is the index of the node relative to its parent
            link = record.data.link;
            window.location = link;
        }
      },
      itemclick: function(view, record, item, index, e){
        selected_node = record;
        /*if(record) {
          button = Ext.getCmp('viewer-tree-view');
          button.enable();
          //record.get('view') ? button.enable() : button.disable(); 
        }//end if*/
      },
      selectionchange: function(view, selections, options) {
        var button, record = selections[0];
        if(record) {
          overviewButton = Ext.getCmp('viewer-tree-overview');
          manageButton = Ext.getCmp('viewer-tree-manage');
          resourcesButton = Ext.getCmp('viewer-tree-resources');
          record.get('id') ? overviewButton.enable() : overviewButton.disable();
          record.get('id') ? manageButton.enable() : manageButton.disable();
          record.get('id') ? resourcesButton.enable() : resourcesButton.disable();
        }   
      }
    },
    dockedItems: [{
      xtype: 'toolbar',
      dock: 'top',
      items: [{
          xtype: 'button',
          text: 'Overview',
          cls: 'x-btn-text-icon',
          iconCls: 'view-datastream-icon',
          disabled: true,
          id: 'viewer-tree-overview',
          handler : function() {
            var record = selected_node.data.link + '#overview';
            window.location = record;
          }   
        },
        {
          xtype: 'button',
          text: 'Resources',
          cls: 'x-btn-text-icon',
          iconCls: 'view-datastream-icon',
          disabled: true,
          id: 'viewer-tree-resources',
          handler : function() {
            var record = selected_node.data.link + '#collection';
            window.location = record;
          }   
        },
        {
          xtype: 'button',
          text: 'Manage',
          cls: 'x-btn-text-icon',
          iconCls: 'edit-datastream-icon',
          disabled: true,
          id: 'viewer-tree-manage',
          handler : function() {
            var record = selected_node.data.link + '#manage';

            window.location = record;
          } 
        }
      ]
    }]
});
