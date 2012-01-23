<?php

/**
 * @file
 * 
 * Helper for generating the proper JSON for populating the Tree panel, which renders
 * Collections aka 'Concepts'
 */

/**
 * Helper class for generating JSON.
 */
class Tree {

  /**
   * PID from url
   * @var string
   */
  private $pid = null;

  /**
   * Repository root pid
   * @var string
   */
  private $root = null;

  /**
   * Node requested by tree
   * @var string
   */
  private $node = null;

  /**
   * Parents of current PID
   * @var type 
   */
  private $parents = array();

  /**
   * Constructor
   * 
   * @param string $pid
   * @param string $node 
   */
  public function __construct($pid, $node = null) {
    $this->pid = $pid;
    $this->node = ($node != 'root' && !empty($node)) ? substr($node, 0, -8) : $node; // Sanitize $node value
    $this->root = variable_get('fedora_repository_pid', 'islandora:root');
  }

  /**
   * Get tree returns a JSON formated string of the tree structure requested. 
   * 
   * @return string
   *   JSON encoded string. 
   */
  public function getTree() {
    /**
     * A pid is passed in via the drupal menu that represents the page your on. A second pid is 
     * passed by the tree when the nod is expanded. This requested_pid is the pid passed by the tree.
     * If it is not set or if it is root, then we want to build the tree based on the root node, 
     * otherwise build the tree for the passed node. 
     */
    $requested_pid = (!isset($this->node) || $this->node == 'root') ? $this->root : $this->node;
    $this->getParents($this->pid); // Sets an instance variable used in other functions thoughout this function.
    $tree = $this->buildTree($requested_pid);
    return json_encode(array('success' => TRUE, 'data' => $tree, 'parents' => $this->parents));
  }

  /**
   * Recursive function to build tree
   * 
   * @param string $pid
   * 
   * @return string 
   */
  private function buildTree($pid) {
    module_load_include('inc', 'content_model_viewer', 'Collection');
    // Lookup members of this PID  
    $collection = new Collection($pid);
    $members = $collection->getTreeMembers();
    // Loop through members and build tree
    foreach ($members as $member) {
      $node['text'] = (empty($member['label'])) ? $member['pid'] : $member['label'];
      if ($member['relatedCount'] > 0) {
        $node['text'] .= ' (' . $member['relatedCount'] . ')';
        $node['leaf'] = false;
      }
      else {
        $node['leaf'] = true;
      }
      $node['link'] = $member['link'];
      $node['id'] = $member['pid'] . '|||' . rand(10000, 99999);
      $node['pid'] = $member['pid'];
      $node['dsid'] = $member['dsid'];
      $node['view'] = $member['view'];
      $node['iconCls'] = preg_replace('/^[^:]*\:/', '', $member['model']); // Strip the namespace
      // Check if this pid is in the parents array so we know whether to expand it or not. 
      if ($member['pid'] == $this->pid || in_array($member['pid'], $this->parents) === true) {
        $node['expanded'] = true;
        $node['cls'] = 'x-tree-bold';
      }
      else {
        $node['expanded'] = false;
        $node['cls'] = '';
      }
      //$node['leaf'] = false;
      // Add node to the data array
      $data[] = $node;
    }
    return $data;
  }

  /**
   * This function queries the resource index to find all the parents of the requested pid. 
   * When we are building the tree, we need to know the parents to know which nodes to expand.
   * 
   * @param string $pid 
   */
  private function getParents($pid) {
    // Root is expanded by default so make sure the pid is not root
    if ($pid != $this->root) {
      // Load the collection class
      module_load_include('inc', 'content_model_viewer', 'Collection');
      $collection = new Collection($pid);
      // Get the parents of the current pid. If pid has multiple parents, this should be handled.
      list($parent, $total) = $collection->getParent($pid);
      foreach ($parent as $k => $arr) {
        // Make sure we're not at the root pid, otherwise save the returned pid as a parent and check
        // to see if it has a non-root level parent.
        if ($arr['pid'] != $this->root) {
          $this->parents[] = $arr['pid'];
          $this->getParents($arr['pid']);
        }
      }
    }
  }

}
