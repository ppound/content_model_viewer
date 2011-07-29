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
   * Renders the HTML needed for the viewer to render.
   * 
   * @param string $pid
   * @param string $dsid
   * 
   * @return string
   */
  abstract public function render($pid, $dsid);
}