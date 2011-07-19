<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'fedora_repository', 'api/fedora_item');

/**
 *
 * @param type $label
 * @param type $mime_type 
 */
function get_suggest_filename($label, $mime_type) {
  $suggested_file_name = "$label";
  $pos = strpos($suggested_file_name, '.');
  /**
   * Here we used to take an object of, say, type application/pdf with label, say, "My Document"
   * and we assemble the output filename extension based on the post-slash portion of the mimetype.
   * (If the label has a period anywhere in it, we leave it alone.)
   *
   * This is great for simple mimetypes like application/pdf, text/html, image/jpeg, etc.
   * but it's terrible for, say, application/vnd.oasis.opendocument.text (.odt).
   *
   * Instead we'll use the get_extension function in MimeClass.inc to discover a valid extension
   * for the mimetype in question.
   */
  if ($pos === FALSE) {
    module_load_include('inc', 'fedora_repository', 'MimeClass');
    $mime_class = new MimeClass();
    $ext = $mime_class->get_extension($mime_type);
    $suggested_file_name = "$label.$ext";
  }
  return $suggested_file_name;
}

/**
 *
 * @param type $pid
 * @param type $dsid
 * @param type $version 
 */
function download_datastream($pid, $dsid, $version = NULL) {
  global $user;
  /**
   * Check for CURL support.
   */
  if (!function_exists("curl_init")) {
    drupal_set_message(t('No curl support.'), 'error');
    exit();
  }
  /**
   * Check for datastream
   */
  $item = new Fedora_Item($pid);
  $datastream_exists = isset($item->datastreams[$dsid]);
  if (!$datastream_exists) {
    drupal_not_found();
    exit();
  }
  $datastream = &$item->datastreams[$dsid];
  $mime_type = $datastream['MIMEType'];
  $label = $datastream['label'];
  $data_stream_info = $item->get_datastream_info($dsid);
  $content_size = $data_stream_info->datastream->size;
  $filename = get_suggest_filename($label, $mime_type);

  $fedora_user = 'anonymous';
  $fedora_password = 'anonymous';
  if (isset($user) && $user->uid != 0) {
    $fedora_user = $user->name;
    $fedora_password = $user->pass;
  }

  $fedora_base_url = variable_get('fedora_base_url', 'http://localhost:8080/fedora');
  $url = "$fedora_base_url/get/$pid/$dsid" . (($version) ? "/$version" : '');

  $curl_handle = curl_init();
  $user_agent = "Mozilla/4.0 pp(compatible; MSIE 5.01; Windows NT 5.0)";
  curl_setopt($curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
  curl_setopt($curl_handle, CURLOPT_SSL_VERIFYHOST, FALSE);
  curl_setopt($curl_handle, CURLOPT_FAILONERROR, 1); // Fail on errors
  curl_setopt($curl_handle, CURLOPT_FOLLOWLOCATION, 1); // allow redirects
  curl_setopt($curl_handle, CURLOPT_USERAGENT, $user_agent);
  curl_setopt($curl_handle, CURLOPT_URL, $url);
  curl_setopt($curl_handle, CURLOPT_USERPWD, "$fedora_user:$fedora_password");
  curl_setopt($curl_handle, CURLOPT_RETURNTRANSFER, 0); // return into a variable
  curl_setopt($curl_handle, CURLOPT_URL, $url);



  header("Content-type: $mime_type");
  if ($content_size > 0) {
    header("Content-length: $content_size");
  }
  header("Content-Disposition: attachment; filename=\"$filename\"");
  curl_exec($curl_handle);
  curl_close($curl_handle);
}
