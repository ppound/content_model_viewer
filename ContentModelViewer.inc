<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'fedora_repository', 'api/fedora_item');

/**
 * 
 */
abstract class ContentModelViewer {

  /**
   * The objects PID.
   * 
   * @var string
   */
  protected $pid;
  /**
   * The object
   * 
   * @var Fedora_Item
   */
  protected $item;

  /**
   * 
   * @param string $pid 
   */
  public function __construct($pid) {
    $this->pid = $pid;
    $this->item = new Fedora_Item($pid);
  }

  /**
   * Renders the HTML for overview panel in the viewer.
   * 
   * @return string
   */
  public function getOverview();

  /**
   * Gets the required datastream information for viewer.
   * 
   * Format: 
   *  @todo include format.
   * 
   * @return array
   */
  public function getDatastreams();
}