<?php
/**
 * @file
 * Administrative page for vopros_chat.
 */

/**
 * Form callback.
 *
 * Settings form.
 */
function vopros_embed_setting_form($form, &$form_state) {

  // Logo upload.
  $form['vopros_embed_logo'] = array(
    '#type' => 'file',
    '#title' => t('Logo'),
    '#description' => t('The logo to display in the contact window.'),
    '#suffix' => theme(
      'image', array(
        'path' => image_style_url(
          'medium',
          variable_get('vopros_embed_logo', '')
        )
      )
    ),
  );

  // Form handlers.
  $form['#validate'][] = 'vopros_embed_setting_form_validate';
  $form['#submit'][] = 'vopros_embed_setting_form_submit';

  return system_settings_form($form);
}

/**
 * Custom form validation.
 */
function vopros_embed_setting_form_validate($form, &$form_state) {

  // Check and prepare the Vopros directory.
  $directory = file_stream_wrapper_uri_normalize('public://' . variable_get('document_path', '') . '/vopros');
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    drupal_set_message(t('The file directory @dir does not exist or is not writable. Please contact an administrator.', array('@dir' => $directory)), 'error');
    return;
  }

  // Temporary save the file and run validation.
  $file = file_save_upload(
    'vopros_embed_logo',
    array(
      // Validates file is really an image.
      'file_validate_is_image' => array(),
      // Validate extensions.
      'file_validate_extensions' => array('png gif jpg jpeg'),
    )
  );

  // Get more info about the file.
  $file_info = image_get_info($file->uri);

  // Try to permanently save the file.
  if ($file) {
    if ($file = file_move($file, $directory . '/vopros_embed_logo.' . $file_info['extension'], FILE_EXISTS_REPLACE)) {
      $form_state['values']['vopros_embed_logo'] = $file;
      // Flush all previous image styles of the old image.
      image_path_flush($directory . '/vopros_embed_logo.' . $file_info['extension']);
    }
    else {
      form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
  else {
    form_set_error('file', t('No file was uploaded.'));
  }
}

/**
 * Custom submit handler.
 */
function vopros_embed_setting_form_submit($form, &$form_state) {
  $file = $form_state['values']['vopros_embed_logo'];
  unset($form_state['values']['vopros_embed_logo']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  variable_set('vopros_embed_logo', $file->uri);
}
