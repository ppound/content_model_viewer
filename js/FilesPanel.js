Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.FilesPanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.filespanel',
    config: {
      pid: 'remeber to pass in'
    },
    constructor: function(config) {
      this.callParent(arguments);
      var properties = ContentModelViewer.properties;
      var url = properties.url;
      var datastreams = Ext.create('Ext.view.View', {
        itemId: 'datastreams',
        itemSelector: 'div.file-item',
        emptyText: 'No Files Available',
        deferEmptyText: false,
        itemTpl: new Ext.XTemplate(
          '<tpl for=".">',
          '   <div class="file-item">',
          '       <div class="file-item-dsid">{[fm.ellipsis(values.dsid, 30, true)]}</div>',
          '       <img class="file-item-img file-item-show-view" src="{tn}"></img>',
          '       <tpl if="this.showLabel(label)">',
          '           <div class="file-item-label">{[fm.ellipsis(values.label, 30, true)]}</div>',
          '       </tpl>',
          '   </div>',
          '</tpl>',
          {
            compiled: true,
            disableFormats: true,
            showLabel: function(label) {
              return jQuery.trim(label) != '';
            }
          }),
        store: Ext.create('Ext.data.Store', {
          model: ContentModelViewer.models.Datastream,
          autoLoad: true,
          pageSize: 4,
          proxy: {
            type: 'rest',
            url : url.object.datastreams(config.pid),
            extraParams: {
              filter: true
            },
            reader: {
              type: 'json',
              root: 'data',
              totalProperty: 'total'
            }
          }
        }),
        listeners: {
          selectionchange: function(view, selections, options) {
            var filesPanel = this.findParentByType('filespanel');
            var toolbar = filesPanel.getComponent('toolbar');
            var button, record = selections[0];
            if(record) {
              button = toolbar.getComponent('view');
              record.get('view') ? button.enable() : button.disable();
              button = toolbar.getComponent('download');
              record.get('download') ? button.enable() : button.disable();
            }
          } 
        }    
      });
      this.add(datastreams);
    },
    getSelected: function() {
      var datastreams = this.getComponent('datastreams');
      var selectionModel = datastreams.getSelectionModel();
      if(selectionModel.hasSelection()) {
        return selectionModel.selected.first();
      }
      return null;
    },
    title: 'Files',
    itemId: 'files',
    width: 260,
    collapsible: true,
    collapsed: false,
    split: true,
    dockedItems: [{
      xtype: 'toolbar',
      dock: 'top',
      itemId: 'toolbar',
      items: [{
        xtype: 'button',
        text: 'View',
        cls: 'x-btn-text-icon',
        iconCls: 'view-datastream-icon',
        itemId: 'view',
        disabled: true,
        handler : function() {
          var filesPanel = this.findParentByType('filespanel');
          var record = filesPanel.getSelected();
          if(record) {
            var dsid = record.get('view'), func = record.get('view_function');
            Ext.getCmp('datastream-viewer').view(filesPanel.pid, dsid, func);            
          }
        }
      }, {
        xtype: 'button',
        text: 'Download',
        cls: 'x-btn-text-icon',
        iconCls: 'download-datastream-icon',
        itemId: 'download',
        disabled: true,
        handler : function() {
          var url = ContentModelViewer.properties.url;
          var filesPanel = this.findParentByType('filespanel');
          var record = filesPanel.getSelected();
          if(record) {
            var form = Ext.get("datastream-download-form");
            form.set({
              action: url.datastream.download(filesPanel.pid, record.get('dsid'))
            });
            document.forms["datastream-download-form"].submit(); // Expected to comedown with the drupal template.
          }
        }
      }]
    },{
      xtype: 'pagingtoolbar',
      store: Ext.data.StoreManager.lookup('files'),   // same store GridPanel is using
      dock: 'bottom'
    }]
  });
});