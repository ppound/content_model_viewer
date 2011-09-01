<?php

// $Id$

/**
 * @file
 *
 * Defines a class that is used for reterive a paged list of datastream's and thier properties.
 */

/**
 * Get datastreams properties of a given Fedora Object.
 */
class Datastreams {

  /**
   * The object whose datastreams we will retrieve.
   * 
   * @var Fedora_Item
   */
  protected $item;

  /**
   * Create a Datastreams Instance.
   * 
   * @param string $pid 
   *   The PID of the object, whose datastreams properties this class will fetch.
   */
  public function __construct($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $this->item = new Fedora_Item($pid);
  }

  /**
   * Gets the datastream properties required for showing the datastreams in the Content Model Viewer.
   * 
   * @param int $start
   *   The offset into the datastreams of the Fedora Object.
   * @param int $length
   *   The maximum number of datastreams to return.
   * @param boolean $filter
   *   Only show those datastreams which can be shown in the Files Panel of the Overview/Viewer tabs.
   * 
   * @return array 
   *   The data needed to show the datastream properties.
   */
  public function get($start, $length, $filter = FALSE) {
    $data = array();
    $datastreams = $filter ? $this->filter($this->item->datastreams) : $this->item->datastreams;
    $total = count($datastreams);
    $datastreams = array_slice($datastreams, $start, $length);
    foreach ($datastreams as $dsid => $properties) {
      list($label, $state, $mime, $created) = array(
        $properties['label'], $properties['state'], $properties['MIMEType'], NULL // @todo MIME type
      );
      $view = $this->hasViewer($mime) ? $dsid : NULL; // Check if there exists a viewer for the MIME Type
      $view_function = $view ? $this->getViewFunction($dsid, $mime) : NULL;
      $download = $dsid; // Can always download by default.
      /* TODO move this filter exclusion to the filter function... */
      if ($filter) { // Overides the defaults
        list($viewable, $downloadable) = $this->getViewDownloadPermissions($dsid, $mime);
        $view = $viewable ? $this->getViewableDatastream($dsid, $mime) : NULL; // Might not be the current datastream.
        $view_function = $view ? $this->getViewFunction($view, $datastreams[$view]['MIMEType']) : NULL;
        $download = $downloadable ? $this->getDownloadableDatastream($dsid, $mime) : NULL;
        if (empty($view) && empty($download)) { // TEMP HACK.
          $total--;
          continue;
        }
      }
      $data[] = array(
        'dsid' => $dsid,
        'label' => $label,
        'state' => $state,
        'mime' => $mime,
        'created' => $create,
        'view' => $view,
        'view_function' => $view_function,
        'download' => $download,
        'tn' => $this->getThumbnail($dsid)
      );
    }
    return array($data, $total);
  }

  /**
   * Gets the properties for one datastream
   * 
   * @param string $dsid 
   * 
   * @return array
   */
  public function setProperties($dsid, $properties) {
    $properties->created = $dsid;
    return $properties;
  }

  /**
   * Filters the data streams down to a list of data streams which can be either viewed, or downloaded.
   * 
   * @param array $datastreams
   *   All the data streams that belong to the Fedora Object.
   * 
   * @return array 
   *   The datastreams which can be viewed or downloaded.
   */
  private function filter(array &$datastreams) {
    $filtered = array();
    foreach ($datastreams as $dsid => $properties) {
      $mime = $properties['MIMEType'];
      list($view, $download) = $this->getViewDownloadPermissions($dsid, $mime);
      if ($view || $download) {
        $filtered[$dsid] = $properties;
      }
    }
    return $filtered;
  }

  /**
   * Gets the Download and View permission for the given data stream.
   * 
   * @param string $dsid
   *   The data stream whose permissions this function fetches.
   * @param string $mime 
   *   The MIME type of the data stream.
   * 
   * @return array
   *   An array containing two boolean values, the first is the view permission the second is the download permission.
   *    array((bool)$view, (bool)$download)
   */
  private function getViewDownloadPermissions($dsid, $mime) {
    $id = content_model_viewer_get_model_id($this->item->pid); // Custom or Default Settings.
    $result = db_fetch_array(db_query('SELECT view, download FROM %s WHERE id = "%d"', CONTENT_MODEL_VIEWER_SETTINGS_TABLE, $id));
    $view = $result['view'];
    $download = $result['download'];
    if (content_model_viewer_model_has_datastream_mimetype_rule($id, $mime)) {
      $result = content_model_viewer_model_get_datastream_mimetype_rule($id, $mime);
      $view = $result['view'];
      $download = $result['download'];
    }
    if (content_model_viewer_model_has_datastream_dsid_rules($id)) {
      $results = content_model_viewer_model_get_datastream_dsid_rules($id);
      foreach ($results as $row) {
        $pattern = $row['dsid'];
        $matches = ($row['type'] == CONTENT_MODEL_VIEWER_DATASTREAM_DSID_RULE_TYPE_REGEX) ?
            preg_match('/' . $pattern . '/', $dsid) :
            trim($pattern) == trim($dsid);
        if ($matches) {
          $view = $row['view'];
          $download = $row['download'];
          break;
        }
      }
    }
    return array($view, $download);
  }

  /**
   * Determines if a viewer plugin is installed for the given mime type.
   * 
   * @param string $mime 
   * 
   * @return boolean
   *   TRUE if a viewer exists that can handle the given MIME type, FALSE otherwise.
   */
  private function hasViewer($mime) {
    return content_model_viewer_get_viewer_properties($mime) != NULL;
  }

  /**
   * Some datastreams are not viewable such as PDF's but they have derived types like SWF's which can be viewed.
   * 
   * For now the Default Action is to return the same data
   * 
   * @param string $dsid
   *   The Datastream to view.
   * @param string $mime 
   *   The Mime type of the datastream to view.
   * 
   * @return string
   *   The function to call to initialize the viewer, NULL if none is required.
   */
  private function getViewableDatastream($dsid, $mime) {
    // Check to see if their is a viewer associated with the given MIME type.
    if ($this->hasViewer($mime)) {
      return $dsid;
    }
    // Look for a rule for loading this mime type's derived type.
    // If found try to execute this rule.
    // A match on dsid or rdf, this way it can be used with existing data like coalliance or the library.
    //if ($this->searchRDF($dsid)) {
    //}
    // If not derived type is found return NULL.
    return NULL;
  }

  /**
   * Get's a javascript function to call to initialize the viewer. 
   * 
   * Some viewers will not require initialization, in which case this function will return NULL.
   * 
   * @param string $dsid
   *   The Datastream to view.
   * @param string $mime 
   *   The Mime type of the datastream to view.
   * 
   * @return string
   *   The function to call to initialize the viewer, NULL if none is required.
   */
  private function getViewFunction($dsid, $mime) {
    $properties = content_model_viewer_get_viewer_properties($mime);
    if ($properties) {
      module_load_include($properties['type'], $properties['module'], $properties['file']);
      $class = $properties['class'];
      $viewer = new $class();
      return $viewer->getInitializationFunction();
    }
    return NULL;
  }

  /**
   * Some datastreams are restricted for downloading, and a lower quality version may be allowed. In these cases the
   * lower quality datastream will be provided for downloaded.
   * 
   * At the moment this isn't parametrized so it will return the same $dsid it has been given.
   * 
   * @param string $dsid
   *   The Datastream to download.
   * @param string $mime 
   *   The Mime type of the datastream to download.
   * 
   * @return string
   *   The datastream to download.
   */
  private function getDownloadableDatastream($dsid, $mime) {
    return $dsid; // @todo parametrize this so that users can have control over what is downloaded.
  }

  /**
   * Gets the url to the image that will be used as a thumbnail for this object.
   * 
   * If there are no derived thumbnails for this datastream a image indicating the MIME type of the datastream 
   * will be used, if there is no image associated with the MIME type of the datastream, a default image will be used.
   * 
   * @param string $dsid 
   * 
   * @return string
   *   A URL to the image that will be used as a thumbnail for the datastream.
   */
  private function getThumbnail($dsid) {
    $module_path = '/' . drupal_get_path('module', 'content_model_viewer');
    $image_path = $module_path . '/images/mime/';
    $default_image = $image_path . 'default.png';
    return $default_image;
  }

}