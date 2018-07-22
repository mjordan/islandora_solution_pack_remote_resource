<?php

/**
 * @file
 * Handles the creation of the stub object as part of the ingest process.
 */

/**
 * Defines a form for creating the stub object.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_solution_pack_remote_resource_create_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $form = array();
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => '',
    '#descrtption' => t('URL of the remote resource'),
    '#required' => TRUE,
  );
  $form['linktext'] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#default_value' => '',
    '#description' => t('If empty, the URL will be used as the link text.'),
  );

  if (user_access('upload remote resource thumbnails')) {
    $tn_extensions = array('gif png jpg jpeg');
    $form['islandora_solution_pack_remote_resource_thumbnail_upload'] = array(
      '#type' => 'checkbox',
      '#title' => t("Add a custom thumbnail to this object?"),
      '#default_value' => FALSE,
    );
    $form['thumbnail'] = array(
      '#title' => t('Thumbnail image'),
      '#type' => 'managed_file',
      '#required' => FALSE,
      '#description' => t('Select thumbnail file to upload.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong><br />This file is optional.', array('@size' => $upload_size, '@ext' => $tn_extensions[0])),
      '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
      '#upload_location' => 'temporary://',
      '#upload_validators' => array(
        'file_validate_extensions' => $tn_extensions,
        // Assume it's specified in MB.
        'file_validate_size' => array($upload_size * 1024 * 1024),
      ),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_solution_pack_remote_resource_thumbnail_upload"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  return $form;
}

/**
 * Addes the uploaded file into the ingestable objects 'OBJ' datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_solution_pack_remote_resource_create_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);
  if (empty($object['OBJ'])) {
    $ds = $object->constructDatastream('OBJ', 'M');
    $object->ingestDatastream($ds);
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
  $ds->label = 'Remote URL';
  $ds->mimetype = 'text/plain';

  if (isset($form_state['values']['thumbnail']) && ($form_state['values']['thumbnail'] > 0)) {
    if (empty($object['TN'])) {
      $ds = $object->constructDatastream('TN', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['TN'];
    }
    $thumbnail_file = file_load($form_state['values']['thumbnail']);
    $thumbnail_path = drupal_realpath($thumbnail_file->uri);
    $ds->setContentFromFile($thumbnail_path, FALSE);
    $ds->label = 'Thumbnail';
    $ds->mimetype = $thumbnail_file->filemime;
  }
  else {
    // Use default thumbnail.
    $default_thumbnail_path = variable_get('islandora_solution_pack_remote_resource_default_thumbnail_path', '');
    if (strlen($default_thumbnail_path) && file_exists($default_thumbnail_path)) {
      if (empty($object['TN'])) {
        $ds = $object->constructDatastream('TN', 'M');
        $object->ingestDatastream($ds);
      }
      else {
        $ds = $object['TN'];
      }
      $thumbnail_mime_detector = new MimeDetect();
      $ds->mimetype = $thumbnail_mime_detector->getMimetype($default_thumbnail_path);
      $ds->label = 'Thumbnail';
      $ds->setContentFromFile($default_thumbnail_path, FALSE);
    }
  }
}
