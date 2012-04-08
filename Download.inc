<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'fedora_repository', 'api/fedora_item');

/**
 * Constructs a filename for a datatream based on its label and mime type.
 * 
 * @param string $label
 * @param string $mime_type 
 * 
 * @return string
 */
function get_suggest_filename($label, $mime_type) {
  $suggested_file_name = "$label";
  $pos = strpos($suggested_file_name, '.');
  if ($pos === FALSE) {
    module_load_include('inc', 'fedora_repository', 'MimeClass');
    $mime_class = new MimeClass();
    $ext = $mime_class->get_extension($mime_type);
    $suggested_file_name = "$label.$ext";
  }
  return $suggested_file_name;
}

/**
 * Get the fedora username and password, based on the current drupal user.
 * 
 * @global object $user
 * 
 * @return array
 *  Where the first element is the fedora username, and the second is the fedora password. 
 */
function get_fedora_user_and_password() {
  global $user;
  if (isset($user) && $user->uid != 0) {
    return array($user->name, $user->pass);
  }
  return array('anonymous', 'anonymous');
}

/**
 * Constructs a url to the given optionally versoned datastream.
 * 
 * @param string $pid
 * @param string $dsid
 * @param string $version
 * 
 * @return string
 */
function get_fedora_datastream_url($pid, $dsid, $version = NULL) {
  $fedora_base_url = variable_get('fedora_base_url', 'http://localhost:8080/fedora');
  $dsid = ($version) ? "$dsid/$version" : $dsid;
  return $fedora_base_url . '/objects/' . $pid . '/datastreams/' . $dsid . '/content';
}

/**
 * Creates a cURL handle that can be used to a datastream from Fedora.
 * 
 * @param string $pid
 * @param string $dsid
 * @param string $version
 * 
 * @return resource
 *  A cURL handle on success, false on errors.
 */
function get_curl_handle($pid, $dsid, $version = NULL) {
  list($fedora_user, $fedora_password) = get_fedora_user_and_password();
  $url = get_fedora_datastream_url($pid, $dsid, $version);
  $curl_handle = curl_init();
  if ($curl_handle !== FALSE) {
    curl_setopt($curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($curl_handle, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_setopt($curl_handle, CURLOPT_FAILONERROR, 1); // Fail on errors
    curl_setopt($curl_handle, CURLOPT_FOLLOWLOCATION, 1); // allow redirects
    curl_setopt($curl_handle, CURLOPT_USERAGENT, "Mozilla/4.0 pp(compatible; MSIE 5.01; Windows NT 5.0)");
    curl_setopt($curl_handle, CURLOPT_URL, $url);
    curl_setopt($curl_handle, CURLOPT_USERPWD, "$fedora_user:$fedora_password");
    curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 0); // return into a variable
    curl_setopt($curl_handle, CURLOPT_URL, $url);
    return $curl_handle;
  }
  return FALSE;
}

/**
 * Populates the response header with the appropriate information.
 *
 * @param string $pid
 * @param string $dsid 
 */
function populate_header($pid, $dsid) {
  $item = new Fedora_Item($pid);
  $datastream = &$item->datastreams[$dsid];
  $mime_type = $datastream['MIMEType'];
  $label = $datastream['label'];
  $data_stream_info = $item->get_datastream_info($dsid);
  $content_size = $data_stream_info->datastream->size;
  $filename = get_suggest_filename($label, $mime_type);
  header("Content-type: $mime_type");
  if ($content_size > 0) {
    header("Content-length: $content_size");
  }
  header("Content-Disposition: attachment; filename=\"$filename\"");
}

/**
 * Test if the datastream exists.
 * 
 * @param string $pid
 * @param string $dsid
 * 
 * @return boolean 
 */
function datastream_exists($pid, $dsid) {
  $item = new Fedora_Item($pid);
  return isset($item->datastreams[$dsid]);
}

/**
 * Downloads the given datastream to the client's computer.
 * 
 * @param string $pid
 * @param string $dsid
 * @param string $version 
 */
function download_datastream($pid, $dsid, $version = NULL) {
  if (!function_exists("curl_init")) {
    drupal_set_message(t('No curl support.'), 'error');
    exit();
  }
  if (!datastream_exists($pid, $dsid)) {
    drupal_not_found();
    exit();
  }
  populate_header($pid, $dsid);
  $curl_handle = get_curl_handle($pid, $dsid, $version);
  curl_exec($curl_handle);
  curl_close($curl_handle);
}
