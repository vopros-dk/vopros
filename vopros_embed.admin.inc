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

  // Header: Title color.
  $form['vopros_embed_header_title_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Title color'),
    '#description' => t('Provide a hex value for the title color.'),
    '#default_value' => variable_get('vopros_embed_header_title_color', '#DDD'),
  );

  // Header: background color.
  $form['vopros_embed_header_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t('Provide a hex value for the header background.'),
    '#default_value' => variable_get('vopros_embed_header_background_color', '#555'),
  );

  // Logo upload.
  $form['vopros_embed_logo'] = array(
    '#type' => 'file',
    '#title' => t('Logo'),
    '#description' => t('The logo to display in the contact window.'),
  );

  // If a logo has been uploaded, we want to show the current image
  // as a suffix to the "upload file" field.
  $logo = variable_get('vopros_embed_logo', '');
  if (!empty($logo)) {
    $form['vopros_embed_logo']['#suffix'] = theme(
      'image', array(
        'path' => image_style_url(
          'medium',
          $logo
        )
      )
    );
  }

  $form['embed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embed code'),
  );

  $form['embed']['agency_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Agency id'),
    '#input' => FALSE,
    '#description' => t('The agency id of the site, DK-<num> for libraries, custom string for others.'),
  );

  $form['embed']['agency_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Local email'),
    '#input' => FALSE,
    '#description' => t('The email address to direct local inquiries to.'),
  );

  // The Drupal.formatString JS function doesn't support replacing each
  // placeholder more than once, so use different placeholders.
  $code = '<script src="!script_base/question_modal.js"></script>
<a href="!form_base/ask-question?!params">Ask a question</a>
';

  $form['embed']['code'] = array(
    '#type' => 'textarea',
    '#title' => t('Code'),
    '#input' => FALSE,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'vopros_embed') . '/js/vopros_embed.admin.js',
        array(
          'type' => 'setting',
          'data' => array(
            'voprosEmbed' => array(
              'template' => $code,
              'base' => url('embed', array('absolute' => TRUE)),
            ),
          ),
        ),
      ),
    ),
    '#description' => t('Copy this code to the site. The script element can be moved to the HEAD section of the page.'),
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

  // Try to permanently save the file.
  if ($file) {
    // Get more info about the file.
    $file_info = image_get_info($file->uri);

    // Check and prepare the Vopros directory.
    $directory = file_stream_wrapper_uri_normalize('public://' . variable_get('document_path', '') . '/vopros');
    if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
      drupal_set_message(t('The file directory @dir does not exist or is not writable. Please contact an administrator.', array('@dir' => $directory)), 'error');
      return;
    }

    // Save the file in the vopros directory.
    if ($file = file_move($file, $directory . '/vopros_embed_logo.' . $file_info['extension'], FILE_EXISTS_REPLACE)) {
      $form_state['values']['vopros_embed_logo'] = $file;
      // Flush all previous image styles of the old image.
      image_path_flush($directory . '/vopros_embed_logo.' . $file_info['extension']);
    }
    else {
      form_set_error('file', t('Failed to write the uploaded file the site\'s file folder.'));
    }
  }
}

/**
 * Custom submit handler.
 */
function vopros_embed_setting_form_submit($form, &$form_state) {
  $file = $form_state['values']['vopros_embed_logo'];
  if (!empty($file)) {
    unset($form_state['values']['vopros_embed_logo']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    variable_set('vopros_embed_logo', $file->uri);
  }

  variable_set('vopros_embed_header_title_color', $form_state['values']['vopros_embed_header_title_color']);
  variable_set('vopros_embed_header_background_color', $form_state['values']['vopros_embed_header_background_color']);
}
