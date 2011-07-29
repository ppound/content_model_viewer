<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'content_model_viewer', 'AbstractViewer');

/**
 * Default viewer for a datastream if none is defined this will be used as a backup.
 * 
 * Creates an iframe and has fedora attempt to render the resource.
 */
class DefaultViewer extends AbstractViewer {

  /**
   * Nothing is required.
   */
  public function addRequiredResources() {
    
  }

  /**
   * Renders the html needed for the viewer.
   * 
   * @param string $pid
   * @param string $dsid
   * 
   * @return string
   */
  public function render($pid, $dsid) {
    return "<div>No viewer found for $dsid</div>";
  }

}
