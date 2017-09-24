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
    $input_path = $this->parameters['target'] . DIRECTORY_SEPARATOR . '*.txt';

    // Accumulate a list of the objects added to the queue.
    $added = array();

    // Get input files and process each one.
    foreach (glob($input_path) as $filepath) {
      // The OBJ files end in .txt.
      if (preg_match('/\.txt$/', $filepath)) {
        $batch_object = new IslandoraRemoteResourceBatchObject($this->connection, $filepath, $this->parameters);
        // Add the instances to the Islandora batch queue.
        $this->addToDatabase($batch_object);
        $added[] = $batch_object;
      }
    }
    return $added;
  }

}
