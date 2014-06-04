<?php

/**
 * @file
 * Page and form callbacks.
 */

/**
 * Form builder.
 *
 * Returns a form for asking a question.
 */
function vopros_embed_question($form, &$form_state) {
  $form['#attached'] = array(
    'css' => array(drupal_get_path('module', 'vopros_embed') . '/css/vopros_embed.css'),
  );

  $agency_id = isset($_GET['agency_id']) ? check_plain($_GET['agency_id']) : '';

  // We can't work without an agency.
  if (empty($agency_id)) {
    $form['error'] = array(
      '#markup' => t('Whoops, widget not configured properly. Please contact the site and tell them about this.'),
    );

    return $form;
  }

  $question = vopros_question_new();
  $form['question'] = array(
    '#type' => 'value',
    '#value' => $question,
  );

  // Fall back to Referer header.
  $referrer = isset($_GET['url']) ? check_plain($_GET['url']) : $_SERVER['HTTP_REFERER'];

  $agency_mail = isset($_GET['agency_mail']) ? check_plain($_GET['agency_mail']) : '';


  $form['question_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Question content'),
    '#required' => TRUE,
    '#default_value' => '',
    '#weight' => -70,
  );

  $form['user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => isset($_GET['name']) ? check_plain($_GET['name']) : '',
    '#weight' => -60,
  );

  $form['user_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
    '#default_value' => isset($_GET['mail']) ? check_plain($_GET['mail']) : '',
    '#weight' => -50,
  );

  $form['user_mobile_phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile phone'),
    '#default_value' => isset($_GET['phone']) ? check_plain($_GET['phone']) : '',
    '#weight' => -40,
  );

  $form['user_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#required' => TRUE,
    '#default_value' => isset($_GET['zip']) ? check_plain($_GET['zip']) : '',
    '#weight' => -30,
  );

  // Pick out a specific subset of options.
  $options = array_intersect_key(_vopros_question_user_answer_preference(), array('mail' => TRUE, 'sms' => TRUE));
  $form['user_answer_preference'] = array(
    '#type' => 'radios',
    '#title' => t('How to recieve answer'),
    '#options' => $options,
    '#default_value' => 'mail',
    '#weight' => -20,
  );

  $deadline_options = _vopros_question_deadline();
  $form['question_deadline'] = array(
    '#type' => 'radios',
    '#title' => t('Deadline'),
    '#required' => TRUE,
    '#options' => $deadline_options,
    '#default_value' => -1,
    '#weight' => -10,
  );

  $form['agency_id'] = array(
    '#type' => 'value',
    '#value' => $agency_id,
  );

  $form['agency_mail'] = array(
    '#type' => 'value',
    '#value' => $agency_mail,
  );

  $form['from_url'] = array(
    '#type' => 'value',
    '#value' => $url,
  );

  $form['form']['question_usage'] = array(
    '#type' => 'value',
    '#value' => variable_get('vopros_question_usage_default', ''),
  );

  if (module_exists('vopros_editorial')) {
    // We have to call field_attach_form, else it wont copy over the values to
    // the entity in field_attach_submit().
    field_attach_form('vopros_question', $question, $form, $form_state, NULL, array('field_name' => 'vopros_editorial'));

    // Hardcoded.
    $form['vopros_editorial']['und']['#type'] = 'value';
    $form['vopros_editorial']['und']['#value'] = 2;
  }

  // Remove the pre_render callback that field_attach_form added. We don't want
  // it to reorder the form elements.
  unset($form['#pre_render']);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit question'),
    '#attributes' => array('class' => array('primary-action')),
  );

  return $form;
}

/**
 * Validate handler for vopros_embed_question().
 */
function vopros_embed_question_validate($form, &$form_state) {
  if ($form_state['values']['user_answer_preference'] == 'sms' &&
    empty($form_state['values']['user_mobile_phone'])) {
    form_error($form['user_mobile_phone'], t('Mobile phone number required for SMS replies.'));
  }

  // Validate fields.
  field_attach_validate('vopros_question', $question, $form, $form_state);

}

/**
 * Submit handler for vopros_embed_question().
 */
function vopros_embed_question_submit($form, &$form_state) {
  $question = $form_state['values']['question'];
  $info = entity_get_property_info('vopros_question');

  foreach ($info['properties'] as $key => $property) {
    if (isset($form_state['values'][$key])) {
      $question->{$key} = $form_state['values'][$key];
    }
  }
  $log = array();

  // Ensure that there's an qanda_service_name fer this sender.
  $agency_id = $question->agency_id;
  $qanda_options = _vopros_question_qanda_serice_options();
  if (!isset($qanda_options[$agency_id])) {
    $options = variable_get('vopros_question_qanda_service_options', '');
    // @todo look up agency name.
    $new_options = trim($options) . "\n" . $agency_id . '|' . $agency_id . "\n";
    variable_set('vopros_question_qanda_service_options', $new_options);
  }
  $question->qanda_service_name = $agency_id;

  if ($form_state['values']['from_url']) {
    $log[] = "Sendt fra " . $form_state['values']['from_url'];
  }

  if ($form_state['values']['agency_mail']) {
    $log[] = "Vidresend til " . $form_state['values']['agency_mail'] . " hvis dette er et spørgsmål der bedre kan besvares lokalt.";
  }

  if ($log) {
    $question->log = implode("\n", $log);
  }

  // This is odd, but that's how vopros_question_question_form_submit() does it.
  if (!isset($question->created) || empty($question->created)) {
    // Set the created time 1 second in the past to make it different from the
    // changed.  This is to make sure that we don't hit the edit page on first
    // view.
    $question->created = REQUEST_TIME - 1;
  }

  field_attach_submit('vopros_question', $question, $form, $form_state);

  $question->save();
  $form_state['redirect'] = 'embed/ask_question/submitted';
}

/**
 * Page callback for the question submitted page.
 */
function vopros_embed_question_submitted_page() {
  $build = array(
    '#markup' => t('Your question has been submitted.'),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'vopros_embed') . '/css/vopros_embed.css'),
    ),
  );

  return $build;
}

/**
 *
 */
function vopros_embed_question_modal_js() {
  drupal_add_http_header('Content-Type', 'application/javascript');

  $js = file_get_contents(drupal_get_path('module', 'vopros_embed') . '/libraries/jquery-1.11.1.min.js');
  $js .= "\n";

  $js .= file_get_contents(drupal_get_path('module', 'vopros_embed') . '/libraries/magnific-popup-0.9.9/jquery.magnific-popup.min.js');
  $js .= "\n";

  $our_js = file_get_contents(drupal_get_path('module', 'vopros_embed') . '/js/vopros_embed.js');
  $css = array(
    '!mag_style' => '/' . drupal_get_path('module', 'vopros_embed') . '/libraries/magnific-popup-0.9.9/magnific-popup.css',
    '!vopros_style' => '/' . drupal_get_path('module', 'vopros_embed') . '/css/vopros_embed_popup.css',
  );
  $js .= strtr($our_js, $css);
  $js .= "\n";

  print $js;
}
