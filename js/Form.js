$(document).ready(function() {
  var parent = $('#tabs-tabset').parent();
  if(parent.length) {
    var form = $('#object-metadata-form');
    var content = form.remove();
    parent.empty();
    parent.append(content);
  }
  Drupal.attachBehaviors();
});

