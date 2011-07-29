/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
function loadFlexPlayer(pid, dsid) {
    var base_url = $('#base_url').text();
    var swfURL = base_url + '/fedora/repository/' + pid + '/' + dsid;
    var swfVersionStr = "10.0.0";
    var xiSwfUrlStr = "/sites/all/libraries/flexpaper/js/swfobject/expressInstall.swf";
    var flashvars = {
        SwfFile : escape(swfURL),
        Scale : 0.6,
        ZoomTransition : "easeOut",
        ZoomTime : 0.5,
        ZoomInterval : 0.1,
        FitPageOnLoad : true,
        FitWidthOnLoad : false,
        PrintEnabled : true,
        FullScreenAsMaxWindow : false,
        ProgressiveLoading : true,
        PrintToolsVisible : true,
        ViewModeToolsVisible : true,
        ZoomToolsVisible : true,
        FullScreenVisible : true,
        NavToolsVisible : true,
        CursorToolsVisible : true,
        SearchToolsVisible : true,
        localeChain: "en_US",
        scaleMode: "exactFit"
    };
    var params = {}
    params.quality = "high";
    params.bgcolor = "#ffffff";
    params.allowscriptaccess = "sameDomain";
    params.allowfullscreen = "true";
    var attributes = {
        wmode: "transparent"
    };
    attributes.id = "FlexPaperViewer";
    attributes.name = "FlexPaperViewer";
    swfobject.embedSWF(
        base_url + "/sites/all/libraries/flexpaper/FlexPaperViewer.swf", "playerFlexPaper",
        "100%", "857", swfVersionStr, xiSwfUrlStr, flashvars, params, attributes);
    swfobject.createCSS("#playerFlexPaper", "display:block;text-align:left;");
}

