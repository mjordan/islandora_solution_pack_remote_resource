<?php

/**
 * @file
 * Functions related to synchronizing changes to remote resources with
 * their local islandora:sp_remote_resource counterparts.
 */

function islandora_solution_pack_remote_resource_sync($input_path) {
  $input_path = $input_path . DIRECTORY_SEPARATOR . '*.txt';
  foreach (glob($input_path) as $obj_path) {
    // The OBJ files end in .txt.
    if (preg_match('/\.txt$/', $obj_path)) {
      islandora_solution_pack_remote_resource_update_datastreams($obj_path);
    }
  }
}

// Get value of URL, get corresponding object, update other datastreams.
function islandora_solution_pack_remote_resource_update_datastreams($path) {
  $url = file_get_contents($path);
  $pid = islandora_solution_pack_remote_resource_query_solr($url);

  // @todo: Get the other datastreams, push them to the object CRUD-style.
}

/**
 * Queries Islandora's Solr index to get an object's PID.
 *
 * @param string $remote_url
 *   The remote URL to query Solr for.
 *
 * @return string
 *   The corresponding PID, or FALSE if none is found.
 */
function islandora_solution_pack_remote_resource_query_solr($remote_url) {
  $query = 'OBJ_t' . ':"' . trim($remote_url) . '"';
  $query_processor = new IslandoraSolrQueryProcessor();
  $query_processor->solrQuery = $query;
  $query_processor->solrParams['fl'] = 'PID';
  $query_processor->executeQuery(FALSE);
  // @todo: Check to see if the number found is > 1 and if so, report it and move on.
  if ($query_processor->islandoraSolrResult['response']['numFound'] === 1) {
    $pid = $query_processor->islandoraSolrResult['response']['objects'][0]['solr_doc']['PID'];
    return $pid;
  }
  else {
    return FALSE;
  }
}
