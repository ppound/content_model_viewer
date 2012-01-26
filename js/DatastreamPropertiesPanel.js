Ext.onReady(function(){
  Ext.define('ContentModelViewer.widgets.DatastreamPropertiesPanel', {
    extend: 'Ext.grid.Panel',
    alias: 'widget.datastreampropertiespanel',
    config: {
      pid: 'required'
    },
    constructor: function(config) {
      this.store = Ext.create('Ext.data.Store', {
        model: ContentModelViewer.models.Datastream,
        autoLoad: true,
        autoSync: true,
        proxy: {
          type: 'rest',
          url : ContentModelViewer.properties.url.object.datastreams(config.pid),
          reader: {
            type: 'json',
            root: 'data',
            totalProperty: 'total'
          }
        }
      });
      this.callParent(arguments);
      this.addDocked({
        itemId: 'pager',
        xtype: 'pagingtoolbar',
        store: this.store,
        dock: 'bottom',
        displayInfo: true
      });
    },
    setPid: function(pid) {
      this.pid = pid;
      this.store.setProxy({
        type: 'rest',
        url : ContentModelViewer.properties.url.object.datastreams(pid),
        extraParams: {
          filter: true
        },
        reader: {
          type: 'json',
          root: 'data',
          totalProperty: 'total'
        }
      });
      this.store.load();
    },
    getSelected: function() {
      var selectionModel = this.getSelectionModel();
      if(selectionModel.hasSelection()) {
        return selectionModel.selected.first();
      }
      return null;
    },
    title: 'Datastreams',
    itemId: 'datastreams',
    selType: 'rowmodel',
    plugins: [Ext.create('Ext.grid.plugin.RowEditing', {
      clicksToEdit: 2
    })],
    columns: [{
      header: 'ID',  
      dataIndex: 'dsid',
      flex: 1
    }, {
      header: 'Label', 
      dataIndex: 'label', 
      field:{
        xtype:'textfield',
        allowBlank:false
      },
      flex: 1
    }, {
      header: 'State', 
      dataIndex: 'state',
      field:{
        xtype:'combobox',
        store: Ext.create('Ext.data.Store', {
          fields: ['value', 'name'],
          data : [{
            "value":"A", 
            "name":"Active"
          }, {
            "value":"I", 
            "name":"Inactive"
          }]
        }),
        queryMode: 'local',
        displayField: 'name',
        valueField: 'value',
        value: "Inactive"
      }
    }, {
      header: 'Mime Type', 
      dataIndex: 'mime',
      field:{
        xtype:'textfield',
        allowBlank:false
      },
      flex: 1
    }, {
      header: 'Date Created', 
      dataIndex: 'created',
      flex: 1
    }],
    listeners: {
      selectionchange: function(view, selections, options) {
        var record = selections[0];
        if(record) {
          var components = {
            remove: true,
            edit: record.get('edit'),
            view: record.get('view'),
            download: record.get('download')
          }
          var toolbar = this.getComponent('toolbar');
          for(var component in components) {
            var enable = components[component];
            component = toolbar.getComponent(component);
            enable ? component.enable() : component.disable();
          }
        }
      }      
    },
    dockedItems: [{
      xtype: 'toolbar',
      itemId: 'toolbar',
      dock: 'top',
      items: [{
        text: 'Add',
        itemId: 'add',
        xtype: 'button',
        cls: 'x-btn-text-icon',
        iconCls: 'add-datastream-icon',
        handler: function() {
          var datastreamproperties = this.findParentByType('datastreampropertiespanel');
          Ext.create('Ext.window.Window', {
            title: 'Add Datastream',
            height: 250,
            width: 375,
            layout: 'fit',
            items: [{
              xtype: 'form',
              bodyPadding: 10,
              items: [{
                xtype: 'textfield',
                name: 'dsid',
                fieldLabel: 'Identifier',
                width: 300
              }, {
                xtype: 'textfield',
                name: 'label',
                fieldLabel: 'Label',
                width: 300
              }, {
                xtype:'combobox',
                name: 'state',
                fieldLabel: 'State',
                store: Ext.create('Ext.data.Store', {
                  fields: ['value', 'name'],
                  data : [{
                    "value":"A", 
                    "name":"Active"
                  }, {
                    "value":"I", 
                    "name":"Inactive"
                  }]
                }),
                queryMode: 'local',
                displayField: 'name',
                valueField: 'value',
                value: "I"
              }, {
                xtype:'combobox',
                name: 'control',
                fieldLabel: 'Control Group',
                store: Ext.create('Ext.data.Store', {
                  fields: ['value', 'name'],
                  data : [{
                    "value":"X", 
                    "name":"Internal XML"
                  }, {
                    "value":"M", 
                    "name":"Managed Content"
                  }, {
                    "value":"E", 
                    "name":"External Referenced Content"
                  }, {
                    "value":"R", 
                    "name":"Redirect Referenced Content"
                  }]
                }),
                queryMode: 'local',
                displayField: 'name',
                valueField: 'value',
                value: "M",
                width: 300,
                listeners: {
                  change: function(combobox) {
                    var form = combobox.up('form');
                    var upload_panel = form.getComponent('upload-panel');
                    var external = combobox.value == 'E';
                    var redirect = combobox.value == 'R';
                    var item = (external || redirect) ? 'url' : 'file';
                    upload_panel.getLayout().setActiveItem(item);
                  }
                }
              }, {
                xtype: 'panel',
                unstyled: true,
                id: 'upload-panel',
                layout: {
                  type: 'card'
                },
                items: [{
                  xtype: 'filefield',
                  name: 'file',
                  itemId: 'file',
                  fieldLabel: 'File'
                }, {
                  xtype:'textfield',
                  itemId: 'url',
                  fieldLabel: 'Location',
                  name: 'url'
                }],
                buttons: [{
                  text: 'Add',
                  formBind: true, // Only enabled once the form is valid
                  handler: function(button) {
                    button.up('form').getForm().submit({
                      url: ContentModelViewer.properties.url.datastream.add(datastreamproperties.pid),
                      waitMsg: 'Creating...',
                      success: function(form, action) {
                        var pager = Ext.getCmp('datastream-pager');
                        pager.doRefresh();
                        Ext.Msg.alert('Status', 'Successfully Added datastream.');
                        var window = button.up('window');
                        window.close();
                      },
                      failure: function(form, action) {
                        switch (action.failureType) {
                          case Ext.form.action.Action.CLIENT_INVALID:
                            Ext.Msg.alert('Failure', 'Form fields may not be submitted with invalid values');
                            break;
                          case Ext.form.action.Action.CONNECT_FAILURE:
                            Ext.Msg.alert('Failure', 'Ajax communication failed');
                            break;
                          case Ext.form.action.Action.SERVER_INVALID:
                            Ext.Msg.alert('Failure', action.result.msg);
                        }
                        var window = button.up('window');
                        window.close();
                      }
                    });
                  }
                }]
              }]
            }]
          }).show();
        }
      }, {
        text: 'Remove',
        itemId: 'remove',
        xtype: 'button',
        cls: 'x-btn-text-icon',
        iconCls: 'remove-datastream-icon',
        disabled: true,
        handler : function() {
          var datastreamProperties = this.findParentByType('datastreampropertiespanel');
          Ext.Msg.show({
            title:'Remove Datastream?',
            msg: 'Are you sure you want to remove this datastream? This action cannot be undone.',
            buttons: Ext.Msg.YESNO,
            fn: function(choice) {
              if(choice == 'yes') {
                var record = datastreamProperties.getSelected();
                if(record) {
                  Ext.Ajax.request({
                    url: ContentModelViewer.properties.url.datastream.purge(datastreamProperties.pid, record.get('dsid')),
                    method: 'POST',
                    success: function(response){
                      var pager = datastreamProperties.getComponent('pager');
                      pager.doRefresh();
                      Ext.Msg.alert('Status', 'Successfully removed datastream.');
                    }
                  });
                }
              }
            },
            icon: Ext.window.MessageBox.QUESTION
          });
        }
      }, {
        text: 'Edit',
        itemId: 'edit',
        xtype: 'button',
        cls: 'x-btn-text-icon',
        iconCls: 'edit-datastream-icon',
        disabled: true,
        handler : function() {
          var datastreamProperties = this.findParentByType('datastreampropertiespanel');
          var record = datastreamProperties.getSelected();
          if(record) {
            var form = Ext.get("datastream-edit-form");
            var dsid = form.down('input[name="dsid"]');
            var action = form.down('input[name="action"]');
            form.set({
              action: window.location // Same Spot.
            });
            dsid.set({
              value: record.get('dsid')
            });
            action.set({
              value: 'edit'
            });
            document.forms["datastream-edit-form"].submit();              
          }
        }
      }, {
        text: 'Download',
        itemId: 'download',
        xtype: 'button',
        cls: 'x-btn-text-icon',
        iconCls: 'download-datastream-icon',
        disabled: true,
        handler : function(button) {
          var datastreamProperties = this.findParentByType('datastreampropertiespanel');
          var record = datastreamProperties.getSelected();
          if(record) {
            ContentModelViewer.functions.downloadDatastream(datastreamProperties.pid, record.get('dsid'));
          }
        }
      }, {
        text: 'View',
        itemId: 'view',
        xtype: 'button',
        cls: 'x-btn-text-icon',
        iconCls: 'view-datastream-icon',
        disabled: true,
        handler : function() {
          var datastreamProperties = this.findParentByType('datastreampropertiespanel');
          var record = datastreamProperties.getSelected();
          if(record) {
            var dsid = record.get('dsid'), func = record.get('view_function');
            Ext.getCmp('datastream-viewer').view(datastreamProperties.pid, dsid, func);
          }
        }
      }]
    }]
  });
});