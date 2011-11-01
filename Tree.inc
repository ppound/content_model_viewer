<?php
class Tree {
  private $pid = null; //pid from url
  private $root = null; //repository root pid
  private $node = null; //Node requested by tree 
  private $path = '/'; //default path used for tree array key. 
  private $tree = array(); //tree placeholder
  private $pMap = array(); //parent map.
  private $requestedNode = null;

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

    //Store the nod requested by the tree
    if($this->pid != $this->root && (!isset($this->node) || $this->node == 'root')){
      $this->requetedNode = $this->pid;
    }

    //Build the tree    
    $tree = $this->buildTree($requestedPid);

    //JSON encode the tree
    $json = json_encode(array('success' => TRUE, 'data' => $tree));
    
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
    list($members, $total) = $collection->getMembers();

    //Loop through members and build tree
    foreach($members as $k=>$arr){
      $collection = new Collection($arr['pid']);
      list($children, $total) = $collection->getMembers();

      $node['text'] = (empty($arr['label'])) ? $arr['pid'] : $arr['label']; 
      $node['link'] = $arr['link'];
      $node['id'] = $arr['pid'];
      $node['leaf'] = (isset($children)) ? false : true;
      $node['checked'] = false;
      if($arr['pid'] == $this->pid){
        $node['expanded'] = true;
      }//end if
      $node['parent'] = $pid;
      if(isset($children)){
        $node['children'] = $this->buildTree($arr['pid']);
      }//end if
      $data[] = $node;
    }//end for
    
    return $data;
  }//end get_treemembers();

  /* TODO
  * This function recurses through the tree to find the requested pid, then needs to expand each parent above.
  * not yet working
  */
  private function expandNodes($tree) {
    //Check if root level nodes match requested pid and if so expand. 
    foreach($tree as $key=>$arr){
      if($arr['pid'] == $this->pid){
        $tree[$k]['expanded'] = true;
        return true;
      }else{
        if(isset($arr['children'])){
          if($this->expandNodes($arr['children'])){
            $tree[$k]['expanded'] = true;
          }
        }
      }
    }

    return false;
  }//end expandNodes();
}//end class Tree;
?>
