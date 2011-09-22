<?php

// $Id$

/**
 * @file
 *
 */
abstract class AbstractModel {

  /**
   * Loads all the required Javascript/CSS/Etc that content model of the given object requires.
   * 
   * @param string $pid 
   */
  public static function LoadRequiredResourcesFor($pid) {
    $model = content_model_viewer_get_registered_content_model_viewer($pid);
    $model->addRequiredResources();
  }

  /**
   * Adds all of the required javascript/css/etc that this model needs to run.
   */
  abstract public function addRequiredResources();

  /**
   * Gets a javascript function that must be run for the model to be initialized.
   * 
   * The javascript function will be called after the HTML has been rendered for the model.
   * 
   * @return string
   *   The javascript initialization function if one exists, NULL otherwise.
   */
  public function getInitializationFunction() {
    return NULL;
  }

  /**
   * Renders the HTML needed for the model to be shown in the overview panel.
   * 
   * @return string
   *   The rendered HTML required for showing this model in the overview panel.
   */
  abstract public function render();
}