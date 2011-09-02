<?php

// $Id$

/**
 * @file
 *
 * A collection of funcitons specific to this module, that make interacting with the database convenient.
 */
/**
 * Constants
 */
define("CONTENT_MODEL_VIEWER_SETTINGS_TABLE", 'content_model_viewer_models');
define("CONTENT_MODEL_VIEWER_DATASTREAM_DSID_RULES_TABLE", 'content_model_viewer_datastream_dsid_rules');
define("CONTENT_MODEL_VIEWER_DATASTREAM_MIMETYPE_RULES_TABLE", 'content_model_viewer_datastream_mimetype_rules');
define("CONTENT_MODEL_VIEWER_DATASTREAM_RELS_VIEW_DERIVED_RULES_TABLE", 'content_model_viewer_datastream_rels_view_derived_rules');
define("CONTENT_MODEL_VIEWER_DATASTREAM_DSID_VIEW_DERIVED_RULES_TABLE", 'content_model_viewer_datastream_dsid_view_derived_rules');
define("CONTENT_MODEL_VIEWER_DEFAULT_SETTINGS_MODEL_ID", 1);
define("CONTENT_MODEL_VIEWER_DATASTREAM_DSID_RULE_TYPE_EXACT", 0);
define("CONTENT_MODEL_VIEWER_DATASTREAM_DSID_RULE_TYPE_REGEX", 1);

/**
 *
 * @param string $pid 
 *   A Fedora Object PID.
 * @return boolean
 *   TRUE if there are settings defined for the given Content Model, FALSE otherwise.
 */
function content_model_viewer_model_has_settings($pid) {
  $query = db_query('SELECT count(pid) FROM content_model_viewer_models WHERE id > 1 AND pid = "%s"', $pid);
  return (db_result($query) > 0) ? TRUE : FALSE;
}

/**
 *
 * @param string $pid 
 *   A Fedora Object PID.
 * @return boolean
 *   TRUE if there are settings defined for the given Content Model, FALSE otherwise.
 */
function content_model_viewer_model_has_datastream_mimetype_rule($model_id, $mime) {
  $query = db_query('SELECT count(id) FROM %s WHERE model_id = "%d" AND mime = "%s"', CONTENT_MODEL_VIEWER_DATASTREAM_MIMETYPE_RULES_TABLE, $model_id, $mime);
  return (db_result($query) > 0) ? TRUE : FALSE;
}

/**
 * 
 */
function content_model_viewer_model_get_datastream_mimetype_rule($model_id, $mime) {
  $query = db_query('SELECT view, download FROM %s WHERE model_id = "%d" AND mime = "%s"', CONTENT_MODEL_VIEWER_DATASTREAM_MIMETYPE_RULES_TABLE, $model_id, $mime);
  return db_fetch_array($query);
}

/**
 * 
 */
function content_model_viewer_model_has_datastream_dsid_rules($model_id) {
  $query = db_query('SELECT count(id) FROM %s WHERE model_id = "%d"', CONTENT_MODEL_VIEWER_DATASTREAM_DSID_RULES_TABLE, $model_id);
  return (db_result($query) > 0) ? TRUE : FALSE;
}

/**
 * 
 */
function content_model_viewer_model_get_datastream_dsid_rules($model_id) {
  $rules = array();
  $query = db_query('SELECT * FROM %s WHERE model_id = "%d"', CONTENT_MODEL_VIEWER_DATASTREAM_DSID_RULES_TABLE, $model_id);
  while ($row = db_fetch_array($query)) {
    $rules[] = $row;
  }
  return $rules;
}

/**
 * Gets the custom settings for the given $pid.
 * 
 * If $model is not provided get the default settings.
 * 
 * @param string $model_id
 *   The model the settings are associated with.
 * 
 * @return array
 *   The rules associated with this fedora object.
 */
function content_model_viewer_get_settings($model_id = 1) {
  $settings = array();
  $query = db_query('SELECT * FROM content_model_viewer_datastream_rules WHERE model_id = "%d"', $model_id);
  while ($data = db_fetch_object($query)) {
    $item = array();
    foreach ($data as $key => $value) {
      $item[$key] = $value;
    }
    $settings[] = $item;
  }
  return $settings;
}

/**
 * Orders the settings for the given datastream, based on the MIME Type and DSID of the datastream. Also filter out 
 * settings that don't apply to this datastream.
 * 
 * If MIME type restricts, and DSID passes.
 * 
 * @param array $settings 
 * @param string $mime
 * @param string $dsid
 * 
 * @return array
 */
function content_model_viewer_order_settings(array $settings) {
  
}

/**
 * Gets the model id that is used to identitfy the custom settings for the given content model, identified by $pid.
 * 
 * @param string $pid 
 *   A fedora object pid.
 * 
 * @return int
 *   Returns the model_id  
 */
function content_model_viewer_get_model_id($pid) {
  $model_id = db_result(db_query('SELECT id FROM %s WHERE id > 1 AND pid = "%s"', CONTENT_MODEL_VIEWER_SETTINGS_TABLE, $pid));
  if (!$model_id) {
    module_load_include('inc', 'fedora_repository', 'ContentModel');
    $content_model = ContentModel::loadFromObject($pid);
    $model_id = $content_model ?
        db_result(db_query('SELECT id FROM %s WHERE id > 1 AND pid = "%s"', CONTENT_MODEL_VIEWER_SETTINGS_TABLE, $content_model->pid)) :
        FALSE;
  }
  return $model_id ? $model_id : CONTENT_MODEL_VIEWER_DEFAULT_SETTINGS_MODEL_ID;
}

/**
 * Checks to see if the user can view the given datastream of the given Fedora Object.
 * 
 * @param string $pid
 *   The Fedora Objects pid.
 * @param string $dsid 
 *   The Datastream ID.
 * 
 * @return boolean
 *   TRUE if the end user is permitted to view the datastream, FALSE otherwise.
 */
function content_model_viewer_is_viewing_permitted($pid, $dsid) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  module_load_include('inc', 'fedora_repository', 'ContentModel');
  $item = new Fedora_Item($pid);
  $content_model = ContentModel::loadFromObject($pid);
  $pid = $content_model->pid;
  $model_id = content_model_viewer_model_has_settings($pid) ? content_model_viewer_get_model_id($pid) : 1;
  $settings = content_model_viewer_get_settings($model_id);
  $permitted = TRUE; // Default TRUE.

  foreach ($settings as $setting) {
    if (($setting['mime'])) {
      
    }
  }
  return $permitted;
}