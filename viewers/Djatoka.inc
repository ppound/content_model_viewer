<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'content_model_viewer', 'viewers/AbstractViewer');

class DjatokaViewer extends AbstractViewer {

  /**
   * Adds all of the required javascript/css/etc that this viewer needs to run.
   */
  public function addRequiredResources() {
    // None
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
    return NULL;
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
    $viewer_url = variable_get('fedora_base_url', '') . '/get/' . $pid . '/ilives:viewerSdef/getViewer?dsid=' . $dsid;
    return '<iframe src="' . $viewer_url . '" frameborder="0" style="width: 100%; height: 835px;" scrolling="no">Errors: unable to load viewer</iframe>';
  }

}