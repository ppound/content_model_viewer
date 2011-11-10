<?php
class Tree {
  private $pid = null; //pid from url
  private $root = null; //repository root pid
  private $node = null; //Node requested by tree 
  private $parents = array(); //Parents of current $pid

  public function __construct($pid, $node=null) {
    //Set the PID
    $this->pid = $pid;

    //Set the node
    $this->node = $node;

    //Set the root
    $this->root = variable_get('fedora_repository_pid', 'islandora:root');
  }//end __construct();

  /*
  * Get tree returns a JSON formated array of the tree structure requested. 
  */
  public function getTree(){
    //A pid is passed in via the drupal menu that represents the page your on. 
    //A second pid is passed by the tree when the nod is expanded. This requestedPid is the pid passed by the tree
    //If it is not set or if it is root, then we want to build the tree based on the root node, otherwise build the tree for the passed node. 
    $requestedPid = (!isset($this->node) || $this->node == 'root') ? $this->root : $this->node;

    //Get all the parents of this pid
    $this->getParents($this->pid);

    //Build the tree    
    $tree = $this->buildTree($requestedPid);

    //JSON encode the tree
    $json = json_encode(array('success' => TRUE, 'data' => $tree, 'parents'=>$this->parents));
    
    return $json;
  }//end __construct();

  /*
  *Recursive function to build tree
  */
  private function buildTree($pid){
    //Load the Collection class
    module_load_include('inc', 'content_model_viewer', 'Collection');

    //Lookup members of this PID
    $collection = new Collection($pid);
    $members = $collection->getTreeMembers();

    //Loop through members and build tree
    foreach($members as $k=>$arr){
      $node['text'] = (empty($arr['label'])) ? $arr['pid'] : $arr['label'];
      if($arr['relatedCount'] > 0) {
        $node['text'] .= ' ('.$arr['relatedCount'].')';
        $node['leaf'] = false;
      }else{
        $node['leaf'] = true;
      }//end if
      $node['link'] = $arr['link'];
      $node['id'] = $arr['pid'];
      $node['dsid'] = $arr['dsid'];
      $node['view'] = $arr['view'];

      //Check if this pid is in the parents array so we know whether to expand it or not. 
      if($arr['pid'] == $this->pid ||in_array($arr['pid'],$this->parents) === true){
        $node['expanded'] = true;
        $node['cls'] = 'x-tree-bold';
      }else{
        $node['expanded'] = false;
        $node['cls'] = '';
      }//end if

      //$node['leaf'] = false;

      //Add node to the data array
      $data[] = $node;
    }//end for
    
    return $data;
  }//end get_treemembers();

  /*
  * This function queries the resource index to find all the parents of the requested pid. 
  * When we are building the tree, we need to know the parents to know which nodes to expand.
  */
  private function getParents($pid) {
    //Root is expanded by default so make sure the pid is not root
    if($pid != $this->root) {
      //Load the collection class
      module_load_include('inc', 'content_model_viewer', 'Collection');
      $collection = new Collection($pid);

      //Get the parents of the current pid. If pid has multiple parents, this should be handled.
      list($parent, $total) = $collection->getParent($pid);
      foreach($parent as $k=>$arr) {
        //Make sure we're not at the root pid, otherwise save the returned pid as a parent and check
        //to see if it has a non-root level parent.
        if($arr['pid'] != $this->root){
          $this->parents[] = $arr['pid'];
          $this->getParents($arr['pid']);
        }//end if
      }//end foreach
    }//end if
  }//end expandTree();
}//end class Tree;
?>
