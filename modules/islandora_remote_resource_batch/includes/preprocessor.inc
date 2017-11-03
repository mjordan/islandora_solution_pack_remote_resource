<?php

/**
 * @file
 * Implementation of classes from the batch ingest framework.
 */

/**
 * Batch preprocessor class.
 */
class IslandoraRemoteResourceBatchPreprocessor extends IslandoraBatchPreprocessor {

  /**
   * Function to get the OBJ files from the input directory.
   */
  public function preprocess() {
    module_load_include('inc', 'islandora_remote_resource_batch', 'includes/utilities');

    // Accumulate a list of the objects added to the queue.
    $added = array();

    // Ingest from URL list.
    if (strlen($this->parameters['url_list'])) {
      $rows = file($this->parameters['url_list']);
      $url_list = islandora_remote_resource_batch_get_csv_records($rows, $this->parameters['url_list_delimiter']);
      foreach ($url_list as $record) {
        // We only want to ingest objects that aren't already in our repo.
        $remote_url = $record['URL'];
        if (!islandora_remote_resource_batch_query_solr($remote_url)) {
          $this->parameters['datastreams'] = $record;
          $batch_object = new IslandoraRemoteResourceBatchObject($this->connection, '', $this->parameters);
          // Add the instances to the Islandora batch queue.
          $this->addToDatabase($batch_object);
          $added[] = $batch_object;
        }
      }
      return $added;
    }

    // Ingest from directory scan.
    $input_path = $this->parameters['target'] . DIRECTORY_SEPARATOR . '*.txt';
    foreach (glob($input_path) as $filepath) {
      // The OBJ files end in .txt.
      if (preg_match('/\.txt$/', $filepath)) {
        $remote_url = file_get_contents($filepath);
        // We only want to ingest objects that aren't already in our repo.
        if (!islandora_remote_resource_batch_query_solr($remote_url)) {
          $batch_object = new IslandoraRemoteResourceBatchObject($this->connection, $filepath, $this->parameters);
          // Add the instances to the Islandora batch queue.
          $this->addToDatabase($batch_object);
          $added[] = $batch_object;
        }
      }
    }
    return $added;
  }

}
