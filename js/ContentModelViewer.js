var tabs = [];
if(typeof OverviewPanel === 'function') {
    tabs.push(new OverviewPanel);
}
if(typeof CollectionPanel === 'function') {
    tabs.push(new CollectionPanel);
}
if(typeof ViewerPanel === 'function') {
    tabs.push(new ViewerPanel);
}
if(typeof ManagePanel === 'function') {
    tabs.push(new ManagePanel);
}
var ContentModelViewer = Ext.extend(Ext.tab.Panel, {
    width: 800,
    height: 600,
    items: tabs
});
Ext.onReady(function(){
    Ext.QuickTips.init();
    var viewer = new ContentModelViewer({
        renderTo: 'content-model-viewer'
    });
    viewer.show();
});