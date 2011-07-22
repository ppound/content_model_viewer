<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'fedora_repository', 'api/fedora_item');

/**
 * This class is used to get/set a fedora object's datastream properties.
 */
class DatastreamProperties {

  /**
   * The given objects PID.
   * 
   * @var string
   */
  protected $pid;
  /**
   * The datasteam.
   * 
   * @var string
   */
  protected $dsid;
  /**
   * The object.
   * 
   * @var Fedora_Item
   */
  protected $item;
  /**
   * Object's Label.
   * 
   * @var string
   */
  protected $label;
  /**
   * The datastreams control group.
   * 
   * Potential values: 
   *  Internal XML Content "I", 
   *  Managed Content "M",
   *  Externally Referenced Content "E",
   *  Redirect Referenced Content "R",
   * 
   * @var string (Read Only)
   */
  protected $control;
  /**
   * The MIME type of the Datastream (required)
   * 
   * @var string
   */
  protected $mime;
  /**
   * The datastream's State.
   * 
   * Either "Active", "Inactive" or "Deleted"
   * 
   * @var string
   */
  protected $state;
  /**
   * An indicator (true/false) as to whether the repository service should version the Datastream.
   * 
   * @var boolean
   */
  protected $versionable;
  /**
   * An optional format identifier for the Datastream such as emerging schemes like PRONOM and 
   * the Global Digital Format Registry (GDRF)
   * 
   * @var string
   */
  protected $format;
  /**
   * One or more alternate identifiers for the Datastream 
   * (such identifiers could be local identifiers or global identifiers such as Handles or DOI)
   * 
   * @var string
   */
  protected $alternate;
  /**
   * The date that this object was created.
   * 
   * @var string
   */
  protected $created;

  /**
   * Creates an object that fetchs the properties of the object identified by $PID. 
   * These properties can be changed, and committed back to fedora.
   * 
   * @param string $pid 
   */
  public function __construct($pid) {
    $this->pid = $pid;
    $this->item = new Fedora_Item($this->pid);
    // @todo implement this class.
  }

  public function getProperties() {
    $data = array();
    $count = 0;
    $start = 0;
    $limit = 5;
    $end = (isset($limit)) ? $start + $limit : NULL;
    foreach ($this->item->datastreams as $dsid => $attr) {
      if ($count >= $start && (empty($end) || $count < $end)) {
        $stream['dsid'] = $dsid;
        $stream['label'] = $attr['label'];
        $stream['state'] = 'A';
        $stream['mime'] = $attr['MIMEType'];
        $data[] = $stream;
      }
      $count++;
    }
    return array($data, $count);
  }

  public function setProperties(/* @todo determine what can be set */) {
    
  }

}