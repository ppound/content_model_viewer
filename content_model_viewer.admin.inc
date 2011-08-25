<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Prepocess the Admin Settings page theme.
 * 
 * @param array $variables 
 */
function template_preprocess_content_model_viewer_settings(array &$variables) {
  $variables['content_models'] = array();
  $variables['content_models'][] = array('label' => 'Test IR Object', 'pid' => 'ir:1');
}

/**
 * Gets the form that allows the users to add customized settings for a Content Model.
 * 
 * @param array $form_state 
 *   The Drupal Form State.
 * 
 * @return array
 *   The Add Custom Settings form.
 */
function content_model_viewer_add_custom_settings_form(array &$form_state) {
  $form = array(
    'fieldset' => array(
      '#type' => 'fieldset',
      '#title' => 'Customize a Content Model',
      'content_model' => array(
        '#type' => 'textfield',
        '#title' => 'Content Model',
        '#required' => true,
        '#autocomplete_path' => 'admin/settings/content_model_viewer/form/autocomplete/models',
        '#description' => 'Choose a Content Model to customize.',
        '#default_value' => isset($form_state['values']['content_model']) ? $form_state['values']['content_model'] : NULL,
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => 'Customize'
      )
    )
  );
  return $form;
}

/**
 * Validates the add custom settings form.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function content_model_viewer_add_custom_settings_form_validate(array $form, array &$form_state) {
  
}

/**
 * Submits the add custom settings form.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function content_model_viewer_add_custom_settings_form_submit(array $form, array &$form_state) {
  
}

/**
 * Gets the form that defines the default settings for any content models that don't provided 
 * thier own custom settings.
 * 
 * @param array $form_state 
 *   The Drupal Form State.
 * 
 * @return array
 *   The Default Setting form.
 */
function content_model_viewer_default_settings_form(array &$form_state) {
  $form = array(
    'fieldset' => array(
      '#type' => 'fieldset',
      '#title' => 'Configure default settings.',
      '#description' => 'These setting will be applied to all content models that don\'t define their own custom settings.',
      'datastreams' => array(
        '#type' => 'fieldset',
        '#title' => 'Data stream Display Rules (View/Download)',
        '#description' => 'Datastream rules only apply to datastreams shown in the Overview/Viewer tab panels.',
        'table' => array(
          '#theme' => 'content_model_viewer_datastream_rules_table'
        )
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => 'Save Changes'
      )
    )
  );
  return $form;
}

/**
 * Validates the default settings form.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function content_model_viewer_default_settings_form_validate(array $form, array &$form_state) {
  
}

/**
 * Submits the default settings form.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function content_model_viewer_default_settings_form_submit(array $form, array &$form_state) {
  
}

/**
 * Autocomplete the content model name..
 * 
 * @param string $string
 * @return string
 */
function content_model_viewer_autocomplete_models($string) {
  $content_models = content_model_viewer_get_available_models();
  $output = array($string => $string);
  $match = strtolower($string);
  foreach ($content_models as $key => $content_model) {
    if (preg_match("/^{$match}/i", $content_model) !== 0) {
      $output[$content_model] = $content_model;
    }
  }
  return drupal_json($output);
}

/**
 * Get Content Models for the Autocomplete Field.
 * 
 * @return array
 */
function content_model_viewer_get_available_models() {
  module_load_include('inc', 'fedora_repository', 'api/fedora_utils');
  $offset = 0;
  $limit = 1000;
  $query = 'select $object  $model from <#ri>
  where (walk($model <fedora-model:hasModel><info:fedora/fedora-system:ContentModel-3.0>
  and $model <fedora-model:hasModel> $object))
  minus $object <mulgara:is><info:fedora/fedora-system:FedoraObject-3.0>
  minus $object <mulgara:is><info:fedora/fedora-system:ContentModel-3.0>
  minus $object <mulgara:is><info:fedora/fedora-system:ServiceDefinition-3.0>
  minus $object <mulgara:is><info:fedora/fedora-system:ServiceDeployment-3.0>
  order by $object';
  $url = variable_get('fedora_repository_url', 'http://localhost:8080/fedora/risearch');
  $url .= "?type=tuples&flush=TRUE&format=Sparql&limit=$limit&offset=$offset&lang=itql&stream=on&query=" . htmlentities(urlencode($query));
  $content = trim(do_curl($url));
  if ($content != '') {
    $doc = new DOMDocument();
    $doc->loadXML($content);
    $path = new DOMXPath($doc);
    $path->registerNamespace('sparql', 'http://www.w3.org/2001/sw/DataAccess/rf1/result');
    $results = $path->query('//sparql:result');
    if ($results->length > 0) {
      $count = $results->length;
      for ($i = 0; $i < $count; $i++) {
        $result = $results->item($i);
        $pid = substr($path->query('sparql:object/@uri', $result)->item(0)->value, 12); // 12 characteres in "info:fedora/"
        $content_models[$pid] = $pid;
      }
    }
  }
  return $content_models;
}