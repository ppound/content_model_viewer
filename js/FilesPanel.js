Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.FilesPanel', {
    extend: 'Ext.panel.Panel',
    alias: 'widget.filespanel',
    constructor: function(config) {
      this.callParent(arguments);
      var properties = ContentModelViewer.properties;
      var url = properties.url;
      var datastreams = Ext.create('Ext.view.View', {
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
    title: 'Files',
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
          var view = filesPanel.down('dataview');
          var selectionModel = view.getSelectionModel();
          if(selectionModel.hasSelection()) {
            var record = selectionModel.selected.first();
            filesPanel.dsid = record.get('view');
            filesPanel.viewFunction = record.get('view_function');
            // @todo make this a function of the datastream viewer.
            var viewer = Ext.getCmp('datastream-viewer');
            var loader = viewer.getLoader();
            loader.load({
              url: url.datastream.view(filesPanel.pid, filesPanel.dsid)
            });
            // @todo add focus function to tabpanel section.
            var viewerPanel = viewer.up('panel');
            var tabpanel = viewer.up('tabpanel');
            tabpanel.setActiveTab(viewerPanel);
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
          var filesPanel = this.findParentByType('filespanel');
          var view = filesPanel.down('dataview');
          var selectionModel = view.getSelectionModel();
          if(selectionModel.hasSelection()) {
            var record = selectionModel.selected.first();
            var dsid = record.get('dsid');
            var url = url.datastream.download(pid, dsid);
            var form = Ext.get("datastream-download-form");
            form.set({
              action: url
            });
            document.forms["datastream-download-form"].submit();
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
/*
  {
    xtype: 'panel',
    title: 'Files',
    width: 260,
    collapsible: true,
    collapsed: false,
    split: true,
    region: 'east',
      
    items: [{
        xtype: 'dataview',
        store: Ext.create('Ext.data.Store', {
          storeId:'files',
          model: ContentModelViewer.models.Datastream,
          autoLoad: true,
          pageSize: 4,
          proxy: {
            type: 'rest',
            url : url.object.datastreams(pid, dsid),
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
        listeners: {
          selectionchange: function(view, selections, options) {
            var button, record = selections[0];
            if(record) {
              button = Ext.getCmp('viewer-view-file');
              record.get('view') ? button.enable() : button.disable();
              button = Ext.getCmp('viewer-download-file');
              record.get('download') ? button.enable() : button.disable();
            }
          } 
        }    
      }]
  }
    
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
  listeners: {
    selectionchange: function(view, selections, options) {
      var button, record = selections[0];
      if(record) {
        button = Ext.getCmp('viewer-view-file');
        record.get('view') ? button.enable() : button.disable();
        button = Ext.getCmp('viewer-download-file');
        record.get('download') ? button.enable() : button.disable();
      }
    } 
  }    
});
});




{
xtype: 'panel',
title: 'Files',
width: 260,
collapsible: true,
collapsed: false,
split: true,
region: 'east',
dockedItems: [{
    xtype: 'toolbar',
    dock: 'top',
    items: [{
        xtype: 'button',
        text: 'View',
        cls: 'x-btn-text-icon',
        iconCls: 'view-datastream-icon',
        disabled: true,
        id: 'viewer-view-file',
        handler : function() {
          var view = this.up('panel').down('dataview');
          var selectionModel = view.getSelectionModel();
          if(selectionModel.hasSelection()) {
            var record = selectionModel.selected.first();
            ContentModelViewer.functions.selectDatastreamRecord(record);
            ContentModelViewer.functions.viewSelectedDatastreamRecord();
          }
        }
      }, {
        xtype: 'button',
        text: 'Download',
        cls: 'x-btn-text-icon',
        iconCls: 'download-datastream-icon',
        disabled: true,
        id: 'viewer-download-file',
        handler : function() {
          var pid = this.findParentByType('viewerpanel').pid;
          var view = this.up('panel').down('dataview');
          var selectionModel = view.getSelectionModel();
          if(selectionModel.hasSelection()) {
            var record = selectionModel.selected.first();
            var dsid = record.get('dsid');
            var url = url.datastream.download(pid, dsid);
            var form = Ext.get("datastream-download-form");
            form.set({
              action: url
            });
            document.forms["datastream-download-form"].submit();
          }
        }
      }]
  },{
    xtype: 'pagingtoolbar',
    store: Ext.data.StoreManager.lookup('files'),   // same store GridPanel is using
    dock: 'bottom'
  }],
items: [{
    xtype: 'dataview',
    store: Ext.create('Ext.data.Store', {
      storeId:'files',
      model: ContentModelViewer.models.Datastream,
      autoLoad: true,
      pageSize: 4,
      proxy: {
        type: 'rest',
        url : url.object.datastreams(pid, dsid),
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
    listeners: {
      selectionchange: function(view, selections, options) {
        var button, record = selections[0];
        if(record) {
          button = Ext.getCmp('viewer-view-file');
          record.get('view') ? button.enable() : button.disable();
          button = Ext.getCmp('viewer-download-file');
          record.get('download') ? button.enable() : button.disable();
        }
      } 
    }    
  }]
}*/