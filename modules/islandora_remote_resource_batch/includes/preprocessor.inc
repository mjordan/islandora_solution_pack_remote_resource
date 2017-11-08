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

    if (isset($this->parameters['type']) && $this->parameters['type'] == 'zip') {
      // @todo: Add a check for a falase return here.
      $path_to_input_files = $this->openZip($this->parameters['target']);
    }
    else {
      $path_to_input_files = $this->parameters['target'];
    }

    $added = array();

    $input_path = $path_to_input_files . DIRECTORY_SEPARATOR . '*.txt';
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

  /**
   * Expands the uploaded .zip file.
   *
   * @param string $zip_uri
   *   The Drupal URI to the zip archive.
   *
   * @return string|bool
   *   The filesystem path to the unzipped archive, or FALSE
   *   if unzipping the archive failed.
   */
  protected function openZip($zip_uri) {
    $zip = new ZipArchive();

    $zip_file = drupal_realpath($zip_uri);
    if (($error = $zip->open($zip_file)) !== TRUE) {
      return FALSE;
    }

    $tmp_path = file_directory_temp() . DIRECTORY_SEPARATOR . pathinfo($zip_file, PATHINFO_FILENAME);
    $zip->extractTo($tmp_path);

    $zip->close();

    return $tmp_path;
  }

}
