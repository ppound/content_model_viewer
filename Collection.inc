<?php

// $Id$

/**
 * @file
 *
 */

/**
 * 
 */
class Collection {

  /**
   * The pid of this Collection.
   * 
   * @var string
   */
  protected $pid;

  /**
   * Named default properties.
   * 
   * @var array
   */
  protected $default;

  /**
   * Create a Collection class.
   * 
   * @param string $pid 
   */
  public function __construct($pid) {
    $this->pid = $pid;
    $sort = array(new stdClass(), new stdClass());
    $sort[0]->property = 'label';
    $sort[0]->direction = 'ASC';
    $filter = array(new stdClass());
    $filter[0]->property = 'label';
    $filter[0]->value = NULL;
    $this->default = array(
      'sort' => $sort,
      'filter' => $filter
    );
  }

  /**
   * 
   * @return array
   */
  public function getMembers() {

    $offset = isset($_REQUEST['start']) ? $_REQUEST['start'] : 0;
    $limit = isset($_REQUEST['limit']) ? $_REQUEST['limit'] : 5;
    $filter = isset($_REQUEST['filter']) ? json_decode($_REQUEST['filter']) : $this->default['filter'];
    $sort = isset($_REQUEST['sort']) ? json_decode($_REQUEST['sort']) : $this->default['sort'];
    $results = $this->findMembers('all');
    $results = $this->filter($results, $filter);
    $results = $this->sort($results, $sort);
    list($results, $count) = $this->limit($results, $offset, $limit);
    $results = $this->populate($results);
    return array($results, $count);
  }

  public function getTreeMembers() {
    $query = 'select $object $title $model $created
    count(
      select $pid
      from <#ri>
      where  $object <fedora-model:hasModel> $sub_model
    and $sub_model <fedora-model:hasModel> $sub_parent_model
    and $sub_parent_model <mulgara:is> <info:fedora/islandora:collectionCModel>
    and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
    and ($pid <fedora-rels-ext:isMemberOfCollection> $object
      or $pid <fedora-rels-ext:isMemberOf> $object )
    )
    from <#ri>
    where ($object <info:fedora/fedora-system:def/model#label> $title
    and $object <fedora-model:hasModel> $model
    and $object <fedora-model:createdDate> $created
    and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
    and ($object <fedora-rels-ext:isMemberOfCollection> <info:fedora/'. $this->pid.'>
    or $object <fedora-rels-ext:isMemberOf> <info:fedora/'. $this->pid .'>))
    minus $model <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
    minus $model <mulgara:is> <info:fedora/fedora-system:ContentModel-3.0>
    order by $title'; 
    /*$query = 'select $object $title $model $created 
    count(
      select $pid 
      from <#ri> 
      where ($pid <fedora-rels-ext:isMemberOfCollection> $object 
      or $pid <fedora-rels-ext:isMemberOf> $object )
    )
    from <#ri>
    where ($object <info:fedora/fedora-system:def/model#label> $title
    and $object <fedora-model:hasModel> $model
    and $object <fedora-model:createdDate> $created
    and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
    and ($object <fedora-rels-ext:isMemberOfCollection> <info:fedora/' . $this->pid . '>
    or $object <fedora-rels-ext:isMemberOf> <info:fedora/' . $this->pid . '>)) 
    minus $model <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
    minus $model <mulgara:is> <info:fedora/fedora-system:ContentModel-3.0>
    order by $title';*/
    $results = $this->query($query);
    $results = $this->populate($results);
    return $results;
  }

  public function getParent($pid) {
    $results = $this->findParent($pid);
    $results = $this->populate($results);
    return array($results, $count);
  }

  /**
   * Find the members of this collection based on type
   * 
   * @param string $type
   * 
   * @return array 
   */
  private function findMembers($type) {
    switch ($type) {
      case 'collections':
        return $this->findMemberCollections();
      case 'objects':
        return $this->findMemberObjects();
      case 'all':
      default:
        return $this->findAllMembers();
    }
  }

   /** 
   * Runs a SPARQL query and returns the results in a array.
   * 
   * @return array
   */
  private function findParent($pid) {
    $query = 'select $object $title $model $created from <#ri>
                             where ($object <info:fedora/fedora-system:def/model#label> $title
                             and $object <fedora-model:hasModel> $model
                             and $object <fedora-model:createdDate> $created
                             and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
                             and (<info:fedora/' . $pid . '> <fedora-rels-ext:isMemberOfCollection> $object
                             or <info:fedora/' . $pid . '> <fedora-rels-ext:isMemberOf> $object))
                             minus $model <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
                             minus $model <mulgara:is> <info:fedora/fedora-system:ContentModel-3.0>
                             order by $title';
    return $this->query($query);
  } 

  /**
   * Runs a SPARQL query and returns the results in a array.
   * 
   * @return array
   */
  private function findAllMembers() {
    $query = 'select $object $title $model $created from <#ri>
                             where ($object <info:fedora/fedora-system:def/model#label> $title
                             and $object <fedora-model:hasModel> $model
                             and $object <fedora-model:createdDate> $created
                             and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
                             and ($object <fedora-rels-ext:isMemberOfCollection> <info:fedora/' . $this->pid . '>
                             or $object <fedora-rels-ext:isMemberOf> <info:fedora/' . $this->pid . '>))
                             minus $model <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
                             minus $model <mulgara:is> <info:fedora/fedora-system:ContentModel-3.0>
                             order by $title';
    return $this->query($query);
  }

  /**
   * Runs a SPARQL query and returns the results in a array.
   * 
   * @return array
   */
  private function findMemberObjects() {
    $query = 'select $object $title $model $parent_model $created from <#ri>
                             where ($object <info:fedora/fedora-system:def/model#label> $title
                             and $object <fedora-model:createdDate> $created
                             and $object <fedora-model:hasModel> $model
                             and $model <fedora-model:hasModel> $parent_model
                             and $parent_model <mulgara:is> <info:fedora/fedora-system:ContentModel-3.0>
                             and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
                             and ($object <fedora-rels-ext:isMemberOfCollection> <info:fedora/' . $this->pid . '>
                             or $object <fedora-rels-ext:isMemberOf> <info:fedora/' . $this->pid . '>))
                             minus $model <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>
                             order by $title';
    return $this->query($query);
  }

  /**
   * Runs a SPARQL query and returns the results in a array.
   * 
   * @return array
   */
  private function findMemberCollections() {
    $query = 'select $object $title $model $parent_model $created from <#ri>
                             where ($object <info:fedora/fedora-system:def/model#label> $title
                             and $object <fedora-model:createdDate> $created
                             and $object <fedora-model:hasModel> $model
                             and $model <fedora-model:hasModel> $parent_model
                             and $parent_model <mulgara:is> <info:fedora/islandora:collectionCModel>
                             and $object <fedora-model:state> <info:fedora/fedora-system:def/model#Active>
                             and ($object <fedora-rels-ext:isMemberOfCollection> <info:fedora/' . $this->pid . '>
                             or $object <fedora-rels-ext:isMemberOf> <info:fedora/' . $this->pid . '>))
                             minus $model <mulgara:is> <info:fedora/fedora-system:FedoraObject-3.0>  
                             order by $title';
    return $this->query($query);
  }

  /**
   * Launch a SPARQL query. Returns all members found up to 1,000,000 results.
   * 
   * @param string $query
   *   SPARQL query.
   * 
   * @return array
   */
  private function query($query) {
    module_load_include('inc', 'fedora_repository', 'CollectionClass');
    $collection = new CollectionClass();
    $results = $collection->getRelatedObjects($this->pid, 1000000, NULL, $query);
    $results = trim($results);
    $ret = array();
    if ($results != '') {
      $document = new DOMDocument();
      $document->loadXML($results);
      $path = new DOMXPath($document);
      $path->registerNamespace('sparql', 'http://www.w3.org/2001/sw/DataAccess/rf1/result');
      $results = $path->query('//sparql:result');
      $count = $results->length;
      if ($count > 0) {
        for ($i = 0; $i < $count; $i++) {
          $result = $results->item($i);
          $pid = substr($path->query('sparql:object/@uri', $result)->item(0)->value, 12); // 12 characteres in "info:fedora/"
          $label = $path->query('sparql:title', $result)->item(0)->textContent;
          $created = $path->query('sparql:created', $result)->item(0)->textContent;
          $relatedCount = $path->query('sparql:k0',$result)->item(0)->textContent;
          $ret[] = array('pid' => $pid, 'label' => $label, 'created' => $created, 'relatedCount' => $relatedCount);
        }
      }
      return $ret;
    }
    return $ret;
  }

  /**
   * Filter the results based on the search string.
   * 
   * @param array $results
   * @param string $search
   * 
   * @return array
   */
  private function filter(array $results, $filter) {
    $search = isset($filter[0]->value) ? trim($filter[0]->value) : NULL;
    if (isset($search) && $search != '') {
      $ret = array();
      foreach ($results as $result) {
        $matches = preg_match("/^.*{$search}.*/i", $result['label']);
        if ($matches > 0) {
          $ret[] = $result;
        }
      }
      return $ret;
    }
    return $results;
  }

  /**
   * Sort the results.
   * 
   * @param array $results
   * @param string $sort
   * 
   * @return array
   */
  private function sort(array $results, $sort) {
    switch ($sort[0]->property) {
      case 'created':
        if ($sort[0]->direction == 'ASC') {
          usort($results, array($this, 'date_newest_sort'));
        }
        else {
          usort($results, array($this, 'date_oldest_sort'));
        }
        break;
      case 'label':
      default:
        if ($sort[0]->direction == 'ASC') {
          usort($results, array($this, 'alpha_asc_sort'));
        }
        else {
          usort($results, array($this, 'alpha_desc_sort'));
        }
        break;
    }
    return $results;
  }

  /**
   * Sort by the title in ascending order.
   * 
   * @param string $a
   * @param string $b
   * 
   * @return boolean
   */
  private function alpha_asc_sort($a, $b) {
    return strcmp($a['label'], $b['label']);
  }

  /**
   * Sort by the title in descending order.
   * 
   * @param string $a
   * @param string $b
   * 
   * @return boolean
   */
  private function alpha_desc_sort($a, $b) {
    return strcmp($b['label'], $a['label']);
  }

  /**
   * Sort by the creation date from newest to oldest.
   * 
   * @param string $a
   * @param string $b
   * 
   * @return boolean
   */
  private function date_newest_sort($a, $b) {
    $a = strtotime($a['created']);
    $b = strtotime($b['created']);
    if ($a == $b) {
      return 0;
    }
    return ($a < $b) ? -1 : 1;
  }

  /**
   * Sort by the creation date from oldest to newest.
   * 
   * @param string $a
   * @param string $b
   * 
   * @return boolean
   */
  private function date_oldest_sort($a, $b) {
    $a = strtotime($a['created']);
    $b = strtotime($b['created']);
    if ($a == $b) {
      return 0;
    }
    return ($a > $b) ? -1 : 1;
  }

  /**
   * Limit the results to a given range.
   * 
   * @param array $results
   * @param integer $offset
   * @param integer $limit
   * 
   * @return array
   */
  private function limit(array $results, $offset, $limit) {
    $count = count($results);
    $data = array_slice($results, $offset, $limit);
    return array($data, $count);
  }

  /**
   * Fills out properties for each object.
   * 
   * @param array $results
   * 
   * @return array
   */
  private function populate($results) {
    global $base_url;
    $module_path = drupal_get_path('module', 'content_model_viewer');
    $image_path = '/' . $module_path . '/images';
    foreach ($results as &$result) {
      $pid = $result['pid'];
      $content_model_pid = $this->getContentModelPid($pid);
      $result['link'] = "$base_url/fedora/repository/$pid";
      $result['tn'] = $image_path . '/object.png';
      $result['description'] = $this->getDescription($pid);
    }
    return $results;
  }

  /**
   * Attempts to get the description from the objects DC datastream.
   * 
   * @param string $pid 
   *   The Fedora Object.
   * 
   * @return string
   *   The description from the objects DC if possible, otherwise a default description is returned.
   */
  private function getDescription($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $default_description = 'Default Description, Check this object\'s metadata and ensure a description is present.';
    $item = new Fedora_Item($pid);
    $dc = $item->get_datastream_dissemination('DC');
    $document = new DOMDocument();
    $document->loadXML($dc);
    $xpath = new DOMXPath($document);
    $xpath->registerNamespace('dc', 'http://purl.org/dc/elements/1.1/');
    $results = $xpath->query('//dc:description');
    $count = $results->length;
    if ($count > 0) {
      $description = '';
      for ($i = 0; $i < $count; $i++) {
        $node = $results->item($i);
        $description .= $node->textContent . ' ';
      }
      $description = trim($description);
      return empty($description) ? $default_description : $description;
    }
    return $default_description;
  }

  /**
   * Get Content Model Pid.
   * 
   * @param string $pid
   * 
   * @return string
   */
  private function getContentModelPid($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $item = new Fedora_Item($pid);
    $rels_ext = $item->get_datastream_dissemination('RELS-EXT');
    $document = new DOMDocument();
    $document->loadXML($rels_ext);
    $xpath = new DOMXPath($document);
    $xpath->registerNamespace('rdf', "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
    $xpath->registerNamespace('fedora-model', "info:fedora/fedora-system:def/model#");
    $results = $xpath->query('/rdf:RDF/rdf:Description/fedora-model:hasModel/@rdf:resource');
    return substr($results->item(0)->value, 12); // 12 characteres in "info:fedora/"
  }

}
