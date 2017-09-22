<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_solution_pack_remote_resource_admin(array $form, array &$form_state) {
  $form = array();
  $form['islandora_solution_pack_remote_resource_redirection_type'] = array(
    '#type' => 'select',
    '#title' => t('Redirection type'),
    '#default_value' => variable_get('islandora_solution_pack_remote_resource_redirection_type', 'interstitial'),
    '#options' => array(
      'interstitial' => t('show user an interstitial page'),
      'goto' => t('automatic redirect'),
    ),
    '#description' => t('Choose whether the user is shown an interstitial page containing a link to the remote resource, or is automatically redirected.'),
  );  

  $module_path = drupal_get_path('module', 'islandora_solution_pack_remote_resource');
  $default_tn_path = DRUPAL_ROOT . DIRECTORY_SEPARATOR . $module_path . DIRECTORY_SEPARATOR . 'images' . DIRECTORY_SEPARATOR . 'remote.png';
  if (file_exists(variable_get('islandora_solution_pack_remote_resource_default_thumbnail_path', $default_tn_path))) {
    $tn_exists_markup = '<br /><img src="/misc/watchdog-ok.png" /> File exists.';
  }
  else {
    $tn_exists_markup = '<br /><img src="/misc/watchdog-error.png" /> File does not exist.';
  }
  $form['islandora_solution_pack_remote_resource_default_thumbnail_path'] = array(
    '#title' => t('Path to default thumbnail image'),
    '#type' => 'textfield',
    '#size' => 100,
    '#default_value' => variable_get('islandora_solution_pack_remote_resource_default_thumbnail_path', $default_tn_path),
    '#description' => t('Absolute filesystem path to the default thumbnail image for remote resource objects. Leave empty to not attach a thumbnail. !tn_exists', array('!tn_exists' => $tn_exists_markup)),
    '#maxlength' => 255,
  );

  return system_settings_form($form);
}
