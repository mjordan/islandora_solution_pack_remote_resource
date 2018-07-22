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
  list($url, $linktext) = explode("\n", $object['OBJ']->content);
  return array(
    'url' => array(
      '#title' => t('Remote URL'),
      '#type' => 'textfield',
      '#description' => t('URL to the remote resource.'),
      '#default_value' => $url,
    ),
    'linktext' => array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#default_value' => $linktext,
      '#description' => t('If empty, the URL will be used as the link text.'),
    ),
    'object' => array(
      '#type' => 'value',
      '#value' => $object->id,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save'),
    ),
    'cancel' => array(
      '#markup' => l(t('Cancel'), url('islandora/object/' . $object->id)),
    ),
  );
}

/**
 * From submit function.
 */
function islandora_solution_pack_remote_resource_edit_url_form_submit($form, &$form_state) {
  $object = islandora_object_load($form_state['values']['object']);
  if (!$object['OBJ']) {
    $ds = $object->constructDatastream('OBJ', 'M');
    $ds->mimetype = 'text/plain';
    $ds->label = 'Remote URL';
  }
  else {
    $ds = $object['OBJ'];
  }

  if (strlen($form_state['values']['linktext'])) {
    $obj_content = $form_state['values']['url'] . "\n" . $form_state['values']['linktext'];
  }
  else {
    $obj_content = $form_state['values']['url'];
  }

  $ds->setContentFromString($obj_content);
  drupal_set_message(t("Remote URL updated for !object.", array('!object' => $object->id)));
  drupal_goto("islandora/object/{$object->id}/manage/datastreams");
}
