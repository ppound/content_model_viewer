<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');

class XMLDatastreamViewer {

  /**
   * The object pid.
   * 
   * @var type 
   */
  protected $pid;
  /**
   * Datastream ID.
   * 
   * @var string
   */
  protected $dsid;

  /**
   *
   * @param string $pid
   * @param string $dsid 
   */
  public function __construct($pid, $dsid) {
    
  }

}