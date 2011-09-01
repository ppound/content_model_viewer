<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'content_model_viewer', 'viewers/AbstractViewer');

class XMLDatastreamViewer extends AbstractViewer {

  /**
   * Adds all of the required javascript/css/etc that this viewer needs to run.
   */
  public function addRequiredResources() {
    $path = drupal_get_path('module', 'content_model_viewer') . '/js';
    drupal_add_js($path . '/XML.js');
    dom_document_pretty_print_include_files();
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
    return 'loadXMLViewer';
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
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $item = new Fedora_Item($pid);
    $doc = new DOMDocument();
    $doc->loadXML($item->get_datastream_dissemination($dsid));
    return dom_document_pretty_print($doc, TRUE);
  }

}