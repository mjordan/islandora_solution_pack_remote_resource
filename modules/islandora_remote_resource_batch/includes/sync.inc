<?php

/**
 * @file
 * Functions related to synchronizing changes to remote resources with
 * their local islandora:sp_remote_resource counterparts.
 */

/**
 * Gets all OBJ files in input directory.
 *
 * Passes each off to function that updates accompanying datastreams.
 *
 * @param string $input_path
 *   The absolute path of the input directory.
 */
function islandora_remote_resource_batch_sync($input_path) {
  module_load_include('inc', 'islandora_remote_resource_batch', 'includes/utilities');
  $input_path = $input_path . DIRECTORY_SEPARATOR . '*.txt';
  foreach (glob($input_path) as $obj_path) {
    // The OBJ files end in .txt.
    if (preg_match('/\.txt$/', $obj_path)) {
      $remote_url = file_get_contents($obj_path);
      if ($pid = islandora_remote_resource_batch_query_solr($remote_url)) {
        islandora_remote_resource_batch_update_datastreams($pid, $obj_path);
      }
    }
  }
}

/**
 * Updates datastreams for the object based on filename.
 *
 * Creates datastream if not present.
 *
 * @problem: 3 new versions are created, not 1.
 *
 * For a base OJB file with the name somefile.txt, a datastream with the
 * DSID 'FOO' will be created from a file with the name somfile.FOO.ext.
 * The datastream's mimetype will be derived from .ext. Its label will
 * be 'FOO datastream'.
 *
 * @param string $pid
 *   PID of the Islandora object.
 * @param string $path
 *   The absolute path to the OBJ datastream.
 */
function islandora_remote_resource_batch_update_datastreams($pid, $path) {
  $pathinfo = pathinfo($path);
  $dir_name = $pathinfo['dirname'];
  $file_name = $pathinfo['filename'];
  $base_path = $dir_name . DIRECTORY_SEPARATOR . $file_name;

  $object = islandora_object_load($pid);

  $ds_filepaths = glob($base_path . '*');
  foreach ($ds_filepaths as $ds_filepath) {
    // We don't want the OBJ .txt file.
    if ($ds_filepath != $base_path . '.txt') {
      $ds_filepath_filename = pathinfo($ds_filepath, PATHINFO_FILENAME);
      // The DSID is the part after the final . in $ds_filepath_filename; we
      // just get the DSID using PATHINFO_EXTENSION because it's convenient.
      $dsid = pathinfo($ds_filepath_filename, PATHINFO_EXTENSION);
      if (strlen($dsid)) {
        // Perform a checksum comparison on the old and new datastream
        // content before going any further.
        if (isset($object[$dsid])) {
          $ds_content_hash = md5($object[$dsid]->content);
          $new_ds_content = file_get_contents($ds_filepath);
          $new_ds_content_hash = md5($new_ds_content);
          if ($new_ds_content_hash == $ds_content_hash) {
            drush_log(t("Existing datastream content and new content for @pid / @dsid are identical, skipping update",
              array('@dsid' => $dsid, '@pid' => $object->id)), "ok");
            continue;
          }
        }

        // Create a new datastream if one with the DSID doesn't exist.
        $ds = isset($object[$dsid]) ? $object[$dsid] : $object->constructDatastream($dsid);
        $mime_detector = new MimeDetect();
        $ds->mimetype = $mime_detector->getMimetype($ds_filepath);
        $ds->label = $dsid . ' datastream';
        $ds->setContentFromFile($ds_filepath, FALSE);
        if (isset($object[$dsid])) {
          drush_log(t("Updating datastream @pid / @dsid",
            array('@dsid' => $dsid, '@pid' => $object->id)), "ok");
        }
        else {
          $object->ingestDatastream($ds);
          drush_log(t("Adding new datastream @pid / @dsid",
            array('@dsid' => $dsid, '@pid' => $object->id)), "ok");
        }
      }
    }
  }
}
