<?php

/**
 * @file
 * This file contains all theme and preprocess functions.
 */

/**
 * Prepares variables for islandora-simple-xml templates.
 *
 * Default template: islandora-simple-xml.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - islandora_object: An AbstractObject from Islandora.
 */
function islandora_solution_pack_remote_resource_preprocess_islandora_solution_pack_remote_resource_default(array &$variables) {
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/metadata');

  $islandora_object = $variables['islandora_object'];

  // We should eventually remove the DC object and dc_array code as it only
  // exists to not break legacy implementations.
  try {
    if (isset($islandora_object['DC']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $islandora_object['DC'])) {
      $dc = $islandora_object['DC']->content;
      $dc_object = DublinCore::importFromXMLString($dc);
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $islandora_object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }

  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($islandora_object);
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->asArray() : array();
  $variables['islandora_object_label'] = $islandora_object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_remote_resource__' . str_replace(':', '_', $islandora_object->id);
  $variables['metadata'] = '';
  $variables['description'] = '';

  $remote_resource_url = $islandora_object['OBJ']->content;

  $variables['islandora_content'] = $remote_resource_url;
}
