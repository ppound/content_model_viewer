<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'content_model_viewer', 'ContentModelViewer');

/**
 * 
 */
class DefaultContentModelViewer extends ContentModelViewer {

  /**
   * Construct the object.
   * 
   * @param string $pid 
   */
  public function __construct($pid) {
    parent::__construct($pid);
  }

  /**
   * Renders the HTML for overview panel in the viewer.
   * 
   * @return string
   */
  public function getOverview() {
    
  }

  /**
   * Gets the required datastream information for viewer.
   * 
   * Format: 
   *  @todo include format.
   * 
   * @return array
   */
  public function getDatastreams() {
    
  }

}