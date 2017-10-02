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
function islandora_solution_pack_remote_resource_preprocess_islandora_solution_pack_remote_resource(array &$variables) {
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  drupal_add_css(drupal_get_path('module', 'islandora_solution_pack_remote_resource') . '/css/default.theme.css');
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/metadata');

  $islandora_object = $variables['islandora_object'];

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
  $variables['metadata'] = islandora_retrieve_metadata_markup($islandora_object);
  $variables['description'] = islandora_retrieve_description_markup($islandora_object);

  $remote_resource_url = $islandora_object['OBJ']->content;
  $content = t('This resource is hosted on another website: <a href="!url">!url</a>.', array('!url' => $remote_resource_url));

  if (variable_get('islandora_solution_pack_remote_resource_show_tn', 1)) {
    if (isset($islandora_object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $islandora_object['TN'])) {
      $variables['display_tn'] = TRUE;
      $params = array(
        'path' => "/islandora/object/" . $islandora_object->id . "/datastream/TN/view",
        'alt' => $islandora_object->label,
        'title' => $islandora_object->label,
      );
      $variables['tn_markup'] = theme('image', $params);
    }
    else {
      $variables['display_tn'] = FALSE;
    }
  }
  else {
    $variables['display_tn'] = FALSE;
  }

  $variables['islandora_content'] = $content;
}
