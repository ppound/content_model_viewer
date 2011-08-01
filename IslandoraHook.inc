<?php

// $Id$

/**
 * @file
 *
 */
class IslandoraHook {

  /**
   * Object Pid
   * 
   * @var string
   */
  protected $pid;

  /**
   * Create the hook for islandcm to call....
   * 
   * @param string $pid 
   */
  public function __construct($pid = NULL) {
    $this->pid = $pid;
  }
  
  /**
   * Render the viewer.
   */
  public function render() {
    return content_model_viewer($this->pid);
  }

}