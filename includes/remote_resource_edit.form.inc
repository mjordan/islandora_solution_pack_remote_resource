<?php

/**
 * @file
 * Remote resource OBJ editing form.
 */

/**
 * Retrives the OBJ edit form.
 *
 * @param AbstractObject $object
 *   The Islandora object.
 *
 * @return array
 *   The form array.
 */
function islandora_solution_pack_remote_resource_edit_url(AbstractObject $object) {
  return drupal_get_form('islandora_solution_pack_remote_resource_edit_url_form', $object);
}

/**
 * Form callback.
 */
function islandora_solution_pack_remote_resource_edit_url_form($form, &$form_state, AbstractObject $object) {
  $url = $object['OBJ']->content;
  return array(
    'url' => array(
      '#title' => t('Remote URL'),
      '#type' => 'textfield',
      '#description' => t('URL to the remote resource.'),
      '#default_value' => $url,
    ),
    'object' => array(
      '#type' => 'value',
      '#value' => $object->id,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
  );
}

/**
 * From submit function.
 */
function islandora_solution_pack_remote_resource_edit_url_form_submit($form, &$form_state) {
  $object = islandora_object_load($form_state['values']['object']);
  if (!$object['OBJ']) {
    $url = $object->constructDatastream('OBJ', 'M');
    $url->mimetype = 'text/plain';
    $url->label = 'Remote URL';
  }
  else {
    $url = $object['OBJ'];
  }
  $url->setContentFromString($form_state['values']['url']);
  try {
    $object->ingestDatastream($url);
  }
  catch (Exception $e) {
    // Do something?
  }
  drupal_set_message(t("Remote URL updated for !object.", array('!object' => $object->id)));
  drupal_goto("islandora/object/{$object->id}/manage/datastreams");
}
