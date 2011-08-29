<?php

// $Id$

/**
 * @file
 *
 */
class Datastreams {

  protected $pid;

  /**
   *
   * @param string $pid 
   */
  public function __construct($pid) {
    $this->pid = $pid;
  }

  public function get($filter = FALSE) {
    $count = 0;
    $data = array();
    $start = $_GET['start'];
    $limit = $_GET['limit'];
    $end = (isset($limit)) ? $start + $limit : NULL;
    foreach ($this->item->datastreams as $dsid => $attr) {
      $mime = $attr['MIMEType'];
      $visible = ($filter) ? $this->viewable($dsid) || $this->downloadable($dsid) : FALSE;
      //$view = $this->viewable($dsid) ? $this->getViewableDatastream($dsid, $filter);
      $view_function = $this->getViewerLoadFunction($view);
      $download = $this->getDownloadableDatastream($dsid, $filter);
      if ($visible) {
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
    }
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

  private function viewable($dsid) {
    if (content_model_viewer_model_has_settings($pid)) {
      
    }
  }

  private function downloadable($dsid) {
    
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