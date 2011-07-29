<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'fedora_repository', 'api/fedora_item');

/**
 * Models the display information a field books should have.
 */
class FieldBookModelViewer {

  /**
   * Object Pid.
   * 
   * @var string
   */
  protected $pid;
  /**
   * The Object.
   * 
   * @var Fedora_Item
   */
  protected $item;

  /**
   * Creates FieldBookModelViewer instance.
   * 
   * @param string $pid 
   */
  public function __construct($pid) {
    $this->pid = $pid;
    $this->item = new Fedora_Item($pid);
  }

  /**
   * Gets HTML to be rendered into the overview Panel.
   * 
   * @return string
   */
  public function getOverview() {
    // Get some data render a template with the info!
    // @todo get some metdata info, and run some itql queries.
    return '<div>FieldBook</div>';
  }

  /**
   * Gets datastreams.
   * 
   * @return array
   */
  public function getDatastreams() {
    $count = 0;
    $data = array();
    $start = $_GET['start'];
    $limit = $_GET['limit'];
    $end = (isset($limit)) ? $start + $limit : NULL;
    foreach ($this->item->datastreams as $dsid => $attr) {
      $passed_start = $count >= $start;
      $within_limit = (empty($end) || $count < $end);
      $within_range = $passed_start && $within_limit;
      if ($within_range) {
        $stream['dsid'] = $dsid;
        $stream['label'] = $attr['label'];
        $stream['state'] = $attr['state'];
        $stream['mime'] = $attr['MIMEType'];
        $stream['created'] = '';
        $stream['view'] = $this->getViewableDatastream($dsid); // From Files Panel
        $stream['view_function'] = ($stream['view']) ?
            $this->getViewerLoadFunction($stream['view']) :
            null;
        $stream['download'] = $this->getDownloadableDatastream($dsid); // From Files Panel
        $stream['tn'] = $this->getThumbnail($dsid);
        $data[] = $stream;
      }
      $count++;
    }
    return $data;
  }

  /**
   * Gets a datastream to view.
   * 
   * Some datastreams with mime-type like 'application/pdf' are not viewable, In these cases we need to 
   * use a derived type to view the datastream. Most of the time we will return.
   * 
   * @param string $dsid 
   * 
   * @return string
   *   The dsid of the datastream to use in conjunction with the datastream viewer. 
   *   NULL if this datastream can't be viewed
   */
  private function getViewableDatastream($dsid) {
    return $dsid;
  }

  /**
   * 
   * 
   * @param string $dsid 
   * 
   * @return string
   */
  private function getDownloadableDatastream($dsid) {
    return $dsid;
  }

  /**
   *
   * @param string $dsid 
   * 
   * @return string
   */
  private function getViewerLoadFunction($dsid) {
    
  }

  /**
   * Gets the url to the image that will be used as a thumbnail for this object.
   * 
   * @param string $dsid 
   * 
   * @return string
   */
  private function getThumbnail($dsid) {
    $module_path = drupal_get_path('module', 'content_model_viewer');
    $image_path = $module_path . './images/mime/';
    $default_image = $image_path . 'Default.png';
    return $default_image;
  }

}