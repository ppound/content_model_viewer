Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.CollectionPanel', {
    extend: 'Ext.panel.Panel',
    itemId: 'collection',
    title: 'Collection',
    dockedItems: [{
      xtype: 'toolbar',
      dock: 'top',
      items: [ {
        xtype: 'tbtext', 
        text: 'Sort By: '
      }, {
        xtype: 'sortbutton',
        text : 'Label',
        listeners: {
          changedirection: function(direction) {
            var store = Ext.data.StoreManager.lookup('members');
            var sorters = store.sorters;
            var title = sorters.get('label');
            title.setDirection(direction);
            store.load();
          }
        }
      }, {
        xtype: 'sortbutton',
        text : 'Created',
        listeners: {
          changedirection: function(direction) {
            var store = Ext.data.StoreManager.lookup('members');
            var sorters = store.sorters;
            var date = sorters.get('created');
            date.setDirection(direction);
            store.load();
          }
        }
      }, {
        xtype: 'tbtext',
        text: 'Search'
      }, {
        xtype: 'textfield',
        hideLabel: true
      }, {
        xtype: 'button',
        text: 'Go',
        handler: function(button, event) {
          var store = Ext.data.StoreManager.lookup('members');
          var filters = store.filters;
          var label = filters.get(0);
          var toolbar = button.up('toolbar');
          var search = toolbar.down('textfield');
          var value = Ext.String.trim(search.getValue());
          label.value = value != '' ? value : null;
          store.load();
        }
      }]
    },  {
      id: 'collection-pager-top',
      xtype: 'pagingtoolbar',
      store: Ext.data.StoreManager.lookup('members'),   // same store GridPanel is using
      dock: 'top',
      displayInfo: true
    }, {
      id: 'collection-pager-bottom',
      xtype: 'pagingtoolbar',
      store: Ext.data.StoreManager.lookup('members'),   // same store GridPanel is using
      dock: 'bottom', 
      displayInfo: true
    }],
    items: [{
      xtype: 'dataview',
      store: Ext.data.StoreManager.lookup('members'),
      itemSelector: 'div.file-item',
      emptyText: 'No Files Available',
      deferEmptyText: false,
      itemTpl: new Ext.XTemplate(
        '<tpl for=".">',
        '   <div class="member-item">',
        '       <a href="{link}">',
        '           <img class="member-item-img" src="{tn}"></img>',
        '       </a>',
        '       <a href="{link}">',
        '       <h2 class="member-item-label">{[fm.ellipsis(this.getLabel(values.label), 100, true)]}</h2>',
        '       </a>',
        '       <p class="member-item-description">{[fm.ellipsis(values.description, 350, true)]}</p>',
        '   </div>',
        '</tpl>',
        {
          compiled: true,
          disableFormats: true,
          getLabel: function(label) {
            var empty = jQuery.trim(label) == '';
            return empty ? 'Default Label: (Please notify an administrator to provide a label)' : label;
          }
        })
    }]
  });
});