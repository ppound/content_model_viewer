<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'fedora_repository', 'api/fedora_item');
module_load_include('inc', 'content_model_viewer', 'models/AbstractModel');

/**
 * Models the display information a field books should have.
 */
class DefaultModelViewer extends AbstractModel {

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
   * Adds all of the required javascript/css/etc that this model needs to run.
   */
  public function addRequiredResources() {
  }

  /**
   * Gets HTML to be rendered into the overview Panel.
   * 
   * @return string
   */
  public function render() {
    return theme('default_content_model_viewer', $this->pid);
  }

  /**
   * Gets datastreams.
   * 
   * @param boolean $filter
   *   Filter datastreams for users.
   * 
   * @return array
   *   The first value is the array of datastream properties, the second value it the number of datastreams that
   *   can be used.
   */
  public function getDatastreams($filter = false) {
    $count = 0;
    $data = array();
    $start = $_GET['start'];
    $limit = $_GET['limit'];
    $end = (isset($limit)) ? $start + $limit : NULL;
    foreach ($this->item->datastreams as $dsid => $attr) {
      $passed_start = $count >= $start;
      $within_limit = (empty($end) || $count < $end);
      $within_range = $passed_start && $within_limit;
      $allowed = $filter ? $this->isDatastreamVisibleToUser($dsid) : true;
      if ($allowed) {
        if ($within_range) {
          $view = $this->getViewableDatastream($dsid, $filter);
          $view_function = $this->getViewerLoadFunction($view);
          $download = $this->getDownloadableDatastream($dsid, $filter);
          $data[] = array(
            'dsid' => $dsid,
            'label' => $attr['label'],
            'state' => $attr['state'],
            'mime' => $attr['MIMEType'],
            'created' => '',
            'view' => $view,
            'view_function' => $view_function,
            'download' => $download,
            'tn' => $this->getThumbnail($dsid)
          );
        }
        $count++;
      }
    }
    return array($data, $count);
  }

  /**
   * Checks to see if this datastream should be visible to the user.
   * 
   * @param string $dsid 
   * 
   * @return boolean
   */
  private function isDatastreamVisibleToUser($dsid) {
    $mime = $this->item->datastreams[$dsid]['MIMEType'];
    switch ($mime) {
      case 'application/pdf':
        return TRUE;
      default:
        return FALSE;
    }
  }

  /**
   * Checks to see if a viewer exists for the given datastream.
   * 
   * @param string $dsid 
   * 
   * @return boolean
   */
  private function isDatastreamViewable($dsid) {
    $mime = $this->item->datastreams[$dsid]['MIMEType'];
    switch ($mime) {
      case 'application/x-shockwave-flash':
      case 'application/shockwave-flash':
        return TRUE;
      default:
        return FALSE;
    }
  }

  /**
   * Gets the dsid for the datastream to render into the viewer.
   * 
   * Some datastreams with mime-type like 'application/pdf' are not viewable, In these cases we need to 
   * use a derived type to view the datastream. In a case like that we would return the dsid of a 
   * datastream that has a mime-type of 'application/shockwave-flash'.
   * 
   * @param string $dsid 
   * @param boolean $filter
   * 
   * @return string
   *   The dsid of the datastream to use in conjunction with the datastream viewer. 
   *   NULL if this datastream can't be viewed
   */
  private function getViewableDatastream($dsid, $filter = FALSE) {
    if ($filter === FALSE) {
      return $this->isDatastreamViewable($dsid) ? $dsid : NULL;
    }
    $mime = $this->item->datastreams[$dsid]['MIMEType'];
    switch ($mime) {
      case 'application/pdf':
        return 'PDF.swf'; // @todo remove hack.
      default:
        return NULL;
    }
  }

  /**
   * Gets the dsid for the datastream 
   * 
   * Some datastreams may be locked down, such as a original source image. In these cases we may allow the user 
   * to download a reduced quality version of the original source image. In a case like that we would return the dsid
   * for the reduced quality version of the original source image. In most cases this function will return the same 
   * value as its parameter.
   * 
   * @param string $dsid 
   * 
   * @return string
   */
  private function getDownloadableDatastream($dsid, $filter) {
    if ($filter) {
      return $dsid;
    }
    return $dsid;
  }

  /**
   * Returns a string that identifies a javascript function to be run after the viewer is rendered into 
   * the viewer panel.
   * 
   * The javascript function is expected to do any final processing required to render the viewer for the given
   * datastream
   * 
   * @param string $dsid 
   * 
   * @return string
   */
  private function getViewerLoadFunction($dsid = NULL) {
    if (empty($dsid)) {
      return NULL;
    }
    $mime = $this->item->datastreams[$dsid]['MIMEType'];
    if ($mime == 'application/x-shockwave-flash' || $mime == 'application/shockwave-flash') { // application/x-shockwave-flash
      return 'loadFlexPlayer';
    }
    return NULL;
  }

  /**
   * Gets the url to the image that will be used as a thumbnail for this object.
   * 
   * @param string $dsid 
   * 
   * @return string
   */
  private function getThumbnail($dsid) {
    $module_path = '/' . drupal_get_path('module', 'content_model_viewer');
    $image_path = $module_path . '/images/mime/';
    $default_image = $image_path . 'default.png';
    return $default_image;
  }

}

/**
 *
 * @global string $base_url
 * @param array $variables 
 */
function template_preprocess_default_content_model_viewer(array &$variables) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $pid = $variables['pid'];
  $item = new Fedora_Item($pid);
  $xml = $item->get_datastream_dissemination('DC');
  $document = new DOMDocument();
  $document->loadXML($xml);
  $xpath = new DOMXPath($document);
  $result = $xpath->query('//*[local-name()="title"]');
  $title = $result->item(0);
  $result = $xpath->query('//*[local-name()="identifier"]');
  $identifier = $result->item(0);
  $variables['table'] = theme_table(array('Field', 'Value'), array(
    array('Title', $title->textContent),
    array('Identifier', $identifier->textContent))
  );
}