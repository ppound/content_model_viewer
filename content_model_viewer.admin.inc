<?php

// $Id$

/**
 * @file
 *
 * Defines a collection of functions and forms that appear in the admin section of the site. They allow the user 
 * to provided settings for this module.
 */
function template_preprocess_content_model_viewer_custom_settings(array &$variables) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $pid = $variables['pid'];
  $item = new Fedora_Item($pid);
  if (!$item->exists()) {
    drupal_set_message(t('The Fedora Object %pid does not exist.', array('%pid' => $pid)), 'error');
    drupal_not_found();
    exit();
  }
  if (!content_model_viewer_model_has_settings($pid)) {
    drupal_set_message(t('The Fedora Object %pid doesn\'t have any custom settings.', array('%pid' => $pid)), 'error');
    drupal_not_found();
    exit();
  }
  $variables['model_id'] = content_model_viewer_get_model_id($pid);
}

/**
 * Provides a form that allows the user to add/remove Custom Settings for this module, on a per Content Model basis.
 * 
 * List all Content Model's that have defined custom settings. 
 * 
 * @param array $form_state
 *   The Drupal Form State.
 * 
 * @return array 
 *   A Drupal Form.
 */
function content_model_viewer_custom_settings_form(array &$form_state) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  $variables['content_models'] = array();
  $result = db_query('SELECT * FROM {content_model_viewer_models} WHERE id > 1');
  $models = array(
    '#header' => array('Content Model', 'PID', 'Edit Settings', 'Delete Settings'),
    '#theme' => 'content_model_viewer_form_table'
  );
  while ($row = db_fetch_object($result)) {
    $item = new Fedora_Item($row->pid);
    $models[] = array(
      'label' => array('#value' => $item->objectProfile->objLabel),
      'pid' => array('#value' => $row->pid),
      'edit' => array('#value' => l('Edit', "admin/settings/content_model_viewer/{$row->pid}/edit")),
      'delete' => array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#pid' => $row->pid, // Default.
        '#submit' => array('content_model_viewer_delete_setting'),
      )
    );
  }
  $models['add'] = array(
    'content_model' => array(
      '#type' => 'textfield',
      '#title' => 'Content Model',
      //'#required' => true,
      '#autocomplete_path' => 'admin/settings/content_model_viewer/form/autocomplete/models',
      '#description' => 'Choose a Content Model to customize. If the content model is already listed please follow the <b>Edit</b> link.',
      '#default_value' => isset($form_state['values']['content_model']) ? $form_state['values']['content_model'] : NULL,
    ),
    0 => array('#value' => ''), // Filler
    1 => array('#value' => ''), // Filler
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Customize',
      '#model_id' => 1, // Default.
      '#submit' => array('content_model_viewer_add_setting'),
      '#validate' => array('content_model_viewer_add_setting_validate'),
    ),
  );
  $form = array(
    'fieldset' => array(
      '#type' => 'fieldset',
      '#title' => 'Content Model Custom Settings',
      '#description' => t('Listed below are all the Content Model that implement custom rules for the Content Viewer.'),
      'models' => $models
    )
  );
  return $form;
}

/**
 * Autocomplete the Content Model name.
 * 
 * Used by the 'content_model_viewer_custom_settings_form'.
 * 
 * @param string $string
 *   The users input.
 * 
 * @return string
 *   A json encoded string containing all the options the user can select.
 */
function content_model_viewer_autocomplete_models($string) {
  $content_models = content_model_viewer_get_available_models();
  $output = array($string => $string);
  $match = strtolower($string);
  foreach ($content_models as $key => $content_model) {
    if (preg_match("/^{$match}/i", $content_model) !== 0 && !content_model_viewer_model_has_settings($content_model)) {
      $output[$content_model] = $content_model;
    }
  }
  return drupal_json($output);
}

/**
 * Validates the add custom setting form.
 * 
 * Called from the 'content_model_viewer_custom_settings_form', when the Customize button is pressed.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function content_model_viewer_add_setting_validate(array $form, array &$form_state) {
  $pid = $form_state['values']['content_model'];
  if (empty($pid)) {
    form_set_error('content_model', t('field is required.'));
  }
  $item = new Fedora_Item($pid);
  if (!$item->exists()) {
    form_set_error('content_model', "The fedora object {$pid} does not exists.");
  }
  if (content_model_viewer_model_has_settings($pid)) {
    form_set_error('content_model', "The content model {$pid} already has custom settings. Click on the edit link above to change them.");
  }
}

/**
 * Submits the add custom setting form.
 * 
 * Called from the 'content_model_viewer_custom_settings_form', when the Customize button is pressed.
 * Redirects the user to the edit page for the new custom setting form.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state
 *   The Drupal Form State.
 */
function content_model_viewer_add_setting(array $form, array &$form_state) {
  $pid = $form_state['values']['content_model'];
  $object = new stdClass();
  $object->pid = $pid;
  drupal_write_record('content_model_viewer_models', $object);
  $form_state['redirect'] = "admin/settings/content_model_viewer/$pid/edit";
}

/**
 * Deletes the settings associated with a given model.
 * 
 * Called from the 'content_model_viewer_custom_settings_form', when a Delete button is pressed.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state 
 *   The Drupal Form State.
 */
function content_model_viewer_delete_setting(array $form, array &$form_state) {
  $pid = $form_state['clicked_button']['#pid'];
  db_query('DELETE FROM content_model_viewer_models WHERE pid = "%s"', $pid);
  drupal_set_message(t('Successfully removed settings for ') . $pid . '.');
  drupal_goto('admin/settings/content_model_viewer');
}

/**
 * Gets the Datastream rules form.
 * 
 * Lists all the existing rules, and provides several fields for adding additional rules.
 * 
 * @param array $form_state
 *   The Drupal Form State.
 * @param int $model_id
 *   The Model associated with the datastream rule.
 *
 * @return array 
 *   A Drupal Form.
 */
function content_model_viewer_datastream_rules_form(array &$form_state, $model_id = 1) {
  $settings = array(
    '#id' => 'datastream-rules',
    '#header' => array('Datastream ID', 'MIME Type', 'View', 'Download'),
    '#theme' => 'content_model_viewer_form_table'
  );
  
  $settings['add'] = array(
    'dsid' => array(
      '#type' => 'textfield',
      '#title' => t(''),
      '#description' => t('A PCRE (Perl-Compatible Regular Expression)'),
      '#default_value' => '.*',
      '#size' => 30,
    ),
    'mime' => array(
      '#type' => 'textfield',
      '#title' => t(''),
      '#default_value' => '*',
      '#description' => t('A valid MIME Type or * to indicate any MIME Type.'),
      '#size' => 30,
    ),
    'view' => array(
      '#type' => 'select',
      '#options' => array(0 => 'Prohibit', 1 => 'Permit')
    ),
    'download' => array(
      '#type' => 'select',
      '#options' => array(0 => 'Prohibit', 1 => 'Permit')
    ),
    'add' => array(
      '#type' => 'submit',
      '#value' => 'Add',
      '#model_id' => $model_id, // Default.
      '#submit' => array('content_model_viewer_add_datastream_rule'),
      '#validate' => array('content_model_viewer_add_datastream_rule_validate'),
    )
  );
  foreach (content_model_viewer_get_settings($model_id) as $row) {
    $settings[] = array(
      'dsid' => array('#value' => $row['dsid']),
      'mime' => array('#value' => $row['mime']),
      'view' => array('#value' => $row['view'] ? t('Permit') : t('Prohibit')),
      'download' => array('#value' => $row['download'] ? t('Permit') : t('Prohibit')),
      'remove' => array(
        '#type' => 'submit',
        '#value' => 'Delete',
        '#rule_id' => $row['id'],
        '#submit' => array('content_model_viewer_delete_datastream_rule')
      ),
      '#attributes' => array(
        'class' => 'draggable'
      )
    );
  }

  $form = array(
    'datastreams' => array(
      '#type' => 'fieldset',
      '#title' => t('Datastream Display Rules (View/Download)'),
      '#description' => '<p>Datastream rules only apply to datastreams shown in the Overview/Viewer tab panels.
        The <b>Datastream ID</b> and <b>MIME Type</b> fields select one or more datastreams, and the 
        <b>View</b> and <b>Download</b> specifiy what actions will be be avalible to the end user.
        If both <b>View</b> and <b>Download</b> 
        are <b>Prohibited</b> then the selected datastreams won\'t appear in the files section of the Viewer.
        The rules governing MIME type are run </p>',
      /**
       * A bit of a hack, I want the default action here to be add not delete, 
       * so I've mirrored the add button and placed it at the begining of the form. 
       * Making it hidden, so it will be the default action.
       */
      'add' => array(
        '#type' => 'submit',
        '#value' => 'Add',
        '#model_id' => $model_id, // Default.
        '#submit' => array('content_model_viewer_add_datastream_rule'),
        '#validate' => array('content_model_viewer_add_datastream_rule_validate'),
        '#attributes' => array('style' => 'position: absolute; left: -9999px; width: 1px; height: 1px;'),
      ),
      'settings' => $settings,
    )
  );
  return $form;
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

/**
 * Theme's a form table for this module.
 * 
 * @param array $element
 *   A Drupal Form Element.
 * 
 * @return sting
 *   HTML that renders a table of settings for datastreams.
 */
function theme_content_model_viewer_form_table(array $element) {
  $rows = array();
  foreach (element_children($element) as $child) {
    $setting = $element[$child];
    $fields = array();
    foreach (element_children($setting) as $property) {
      $field = $setting[$property];
      $fields[] = drupal_render($field);
    }
    $rows[] = array(
      'data' => $fields,
      'class' => isset($setting['#attributes']['class']) ? $setting['#attributes']['class'] : NULL
    );
  }
  $attributes = isset($element['#id']) ? array('id' => $element['#id']) : NULL;
  return theme_table($element['#header'], $rows, $attributes);
}

/**
 * Removes the given datastream rule, identified by the #rule_id property of the clicked button.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state 
 *   The Drupal Form State.
 */
function content_model_viewer_delete_datastream_rule(array $form, array &$form_state) {
  if (isset($form_state['clicked_button']['#rule_id'])) {
    db_query('DELETE FROM content_model_viewer_datastream_rules WHERE id = "%d"', $form_state['clicked_button']['#rule_id']);
    drupal_set_message('Successfully Deleted Datastream Setting');
  }
}

/**
 * Adds a Datastream rule to a Model, identified by the #model_id property of the clicked button.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state 
 *   The Drupal Form State.
 */
function content_model_viewer_add_datastream_rule(array $form, array &$form_state) {
  $object = new stdClass();
  $object->model_id = $form_state['clicked_button']['#model_id'];
  $object->dsid = $form_state['values']['dsid'];
  $object->mime = $form_state['values']['mime'];
  $object->view = $form_state['values']['view'];
  $object->download = $form_state['values']['download'];
  drupal_write_record('content_model_viewer_datastream_rules', $object);
  drupal_set_message('Successfully Added Datastream Setting');
}

/**
 * Validates the add datastream rule portion of the form.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state 
 *   The Drupal Form State.
 */
function content_model_viewer_add_datastream_rule_validate(array $form, array &$form_state) {
  $add = $form['datastreams']['settings']['add'];
  if (empty($add['dsid']['#value'])) {
    form_error($add['dsid'], t('field is required.'));
  }
  else {
    content_model_viewer_regex_validate($add['dsid'], $form_state);
  }
  if (empty($add['mime']['#value'])) {
    form_error($add['mime'], t('field is required.'));
  }
}

/**
 * Validates an elements input by checking if it is valid regex input.
 * 
 * @param array $element
 *   A Drupal Form Element
 * @param array $form_state 
 *   The Drupal Form State.
 */
function content_model_viewer_regex_validate(array $element, array &$form_state) {
  $pattern = '/' . $element['#value'] . '/';
  $result = @preg_match($pattern, '');
  if ($result === FALSE) {
    $message = t('This value provided "');
    $message .= $element['#value'];
    $message .= t('" is not a valid regular expression. For more information click ');
    $message .= l(t('here'), 'http://php.net/manual/en/reference.pcre.pattern.syntax.php');
    form_error($element, $message);
  }
}
