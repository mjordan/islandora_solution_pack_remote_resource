<?php

/**
 * @file
 * Implementation of Drush hooks.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_remote_resource_batch_drush_command() {
  $items = array();

  $items['islandora_remote_resource_batch_preprocess'] = array(
    'aliases' => array('irrbp'),
    'description' => 'Preprocess Islandora Remote Resource objects into batch queue entries.',
    'drupal dependencies' => array('islandora_batch', 'islandora_solution_pack_remote_resource'),
    'options' => array(
      'target' => array(
        'description' => 'The target directory to scan.',
        // 'required' => TRUE,
      ),
      'namespace' => array(
        'description' => 'The namespace for objects created by this command.',
        'required' => TRUE,
      ),
      'parent' => array(
        'description' => 'The collection to which the generated items should be added.',
        'required' => TRUE,
      ),
      'parent_relationship_uri' => array(
        'description' => 'The namespace URI of the relationship to the parent. Defaults to "info:fedora/fedora-system:def/relations-external#".',
        'value' => 'optional',
      ),
      'parent_relationship_pred' => array(
        'description' => 'The predicate of the relationship to the parent. Defaults to "isMemberOf".',
        'value' => 'optional',
      ),
      'url_list' => array(
        'description' => 'A file containing the URLs of remote resources to ingest. See the README for more information.',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  // Does not use the Batch API. Included in the Remote Resource Batch module
  // anyway. URL value is indexed in the Solr field 'OBJ_t'.
  $items['islandora_remote_resource_batch_sync'] = array(
    'aliases' => array('irrbs'),
    'description' => 'Updates datastreams (other than OBJ) in Islandora Remote Resource objects.',
    'drupal dependencies' => array('islandora_batch', 'islandora_solution_pack_remote_resource'),
    'options' => array(
      'target' => array(
        'description' => 'The target directory to scan.',
        'required' => TRUE,
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );

  return $items;
}

/**
 * Implements hook_drush_command().
 */
function drush_islandora_remote_resource_batch_preprocess() {
  // Due to how Drush bootstrapping works, the connection may be created
  // without credentials (when your site's front page is
  // 'islandora/object/some:object', for example). Resetting to ensure a new
  // connection gets created should fix it.
  drupal_static_reset('islandora_get_tuque_connection');
  $connection = islandora_get_tuque_connection();
  $parameters = array(
    'namespace' => drush_get_option('namespace'),
    'target' => drush_get_option('target'),
    'parent' => drush_get_option('parent'),
    'parent_relationship_uri' => drush_get_option('parent_relationship_uri', 'info:fedora/fedora-system:def/relations-external#'),
    'parent_relationship_pred' => drush_get_option('parent_relationship_pred', 'isMemberOf'),
    'url_list' => drush_get_option('url_list', ''),
  );

  // Run the preprocessor.
  $preprocessor = new IslandoraRemoteResourceBatchPreprocessor($connection, $parameters);
  $preprocessed = islandora_batch_handle_preprocessor($preprocessor);
  drush_log(t("SetId: @s", array('@s' => $preprocessor->getSetId())), "ok");
}

/**
 * Implements hook_drush_command().
 */
function drush_islandora_remote_resource_batch_sync() {
  module_load_include('inc', 'islandora_remote_resource_batch', 'includes/sync');
  $dir_to_scan = drush_get_option('target');
  islandora_remote_resource_batch_sync($dir_to_scan);
}

/**
 * Parent collection validation.
 */
function drush_islandora_remote_resource_batch_preprocess_validate() {
  $parent = drush_get_option('parent');
  $object = islandora_object_load($parent);
  if (!$object) {
    return drush_set_error('This is not a valid parent', dt('The specified parent (@parent) was unable to be loaded.',
      array('@parent' => $parent)));
  }
}
