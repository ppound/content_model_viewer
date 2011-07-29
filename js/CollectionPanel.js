Ext.onReady(function(){
    Ext.define('ContentModelViewer.widgets.CollectionPanel', {
        extend: 'Ext.panel.Panel',
        itemId: 'collection',
        title: 'Collection',
        dockedItems: [{
            xtype: 'toolbar',
            dock: 'top',
            items: [{
                xtype: 'button',
                text: 'Edit Permissions',
                cls: 'x-btn-text-icon',
                iconCls: 'edit-datastream-icon',
                id: 'edit-xacml'
            }, {
                xtype: 'button',
                text: 'Purge Object',
                cls: 'x-btn-text-icon',
                iconCls: 'remove-datastream-icon',
                id: 'purge-object'
            }]
        }, {
            id: 'collection-pager',
            xtype: 'pagingtoolbar',
            store: Ext.data.StoreManager.lookup('members'),   // same store GridPanel is using
            dock: 'bottom',
            displayInfo: true
        }],
        items: [{
            xtype: 'dataview',
            store: Ext.data.StoreManager.lookup('members'),
            itemSelector: 'div.file-item',
            emptyText: 'No Files Availible',
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