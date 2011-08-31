<?php

// $Id$

/**
 * @file
 *
 */
abstract class AbstractViewer {

  /**
   * Loads all the required Javascript/CSS/Etc that each installed viewer must be aware of.
   */
  public static function LoadRequiredResourcesForAllViewers() {
    content_model_viewer_include_viewers();
    $classes = get_declared_classes();
    foreach ($classes as $class) {
      $reflection = new ReflectionClass($class);
      $parent = $reflection->getParentClass();
      if ($parent && $parent->name == 'AbstractViewer') {
        $instance = new $class();
        $instance->AddRequiredResources();
      }
    }
  }

  /**
   * Adds all of the required javascript/css/etc that this viewer needs to run.
   */
  abstract public function addRequiredResources();

  /**
   * Gets a javascript function that must be run for the viewer to be initialized.
   * 
   * The javascript function will be called after the HTML has been rendered for the viewer.
   * 
   * @return string
   *   The javascript initialization function if one exists, NULL otherwise.
   */
  public function getInitializationFunction() {
    return NULL;
  }

  /**
   * Renders the HTML needed for the viewer.
   * 
   * @param string $pid
   *   The Fedora Object.
   * @param string $dsid
   *   The data stream to render.
   * 
   * @return string
   *   The rendered HTML required for showing this viewer.
   */
  abstract public function render($pid, $dsid);
}