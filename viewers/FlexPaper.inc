<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'content_model_viewer', 'viewers/AbstractViewer');

/**
 * Default viewer for a datastream if none is defined this will be used as a backup.
 * 
 * Creates an iframe and has fedora attempt to render the resource.
 */
class FlexPaperViewer extends AbstractViewer {

  /**
   * Adds all of the required javascript/css/etc that this viewer needs to run.
   */
  public function addRequiredResources() {
    $module_path = drupal_get_path('module', 'content_model_viewer');
    $js_path = $module_path . '/js';
    drupal_add_js("sites/all/libraries/flexpaper/js/flexpaper_flash.js");
    drupal_add_js("sites/all/libraries/swfobject/swfobject.js");
    drupal_add_js($js_path . '/Flexpaper.js');
  }

  /**
   * Gets a javascript function that must be run for the viewer to be initialized.
   * 
   * The javascript function will be called after the HTML has been rendered for the viewer.
   * 
   * @return string
   *   The javascript initialization function if one exists, NULL otherwise.
   */
  public function getInitializationFunction() {
    return 'loadFlexPlayer';
  }

  /**
   * Renders the HTML needed for the viewer.
   * 
   * @param string $pid
   *   The Fedora Object.
   * @param string $dsid
   *   The data stream to render.
   * 
   * @return string
   *   The rendered HTML required for showing this viewer.
   */
  public function render($pid, $dsid) {
    return '<div id="playerFlexPaper"></div>';
  }

}