<?php

/**
 * @ file
 * Admin forms and callback functions for client admin.
 */

/**
 * Client settings form
 */
function vopros_question_client_settings_form($form, &$form_state) {

  $form['markup'] = array(
    '#markup' => 'Here comes the configuration of quanda to clients mapping.',
  );

  return $form;
  // return system_settings_form($form);
}

/**
 * Form to edit or create a client.
 */
function vopros_question_client_form($form, &$form_state, $client) {
  if (empty($client)) {
    $client = entity_create('vopros_question_client', array());
  }

  field_attach_form('vopros_question_client', $client, $form, $form_state);

  $form['client'] = array(
    '#type' => 'value',
    '#value' => $client,
  );

  $form['agency'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#required' => TRUE,
    '#default_value' => !empty($client->agency) ? $client->agency : '',
    '#description' => t('The ID of the client. For libraries this is their agency ID in the form <em>DK-123456</em>. For other clients it can be any unique (preferably lowercase) string.'),
  );

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#default_value' => !empty($client->label) ? $client->label : '',
    '#description' => t('Human readable label for this client.'),
  );

  $form['form']['buttons'] = array(
    '#weight' => 100,
  );

  $form['form']['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => array('primary-action')),
  );

  return $form;
}

/**
 * Validation handler for vopros_question_client_form().
 *
 * Checks for agency uniqueness.
 */
function vopros_question_client_form_validate(&$form, &$form_state) {
  $client = $form_state['values']['client'];
  $query = db_select('vopros_question_client', 'vqc')
    ->fields('vqc', array('client_id'))
    ->condition('agency', $form_state['values']['agency']);

  if (!$client->is_new) {
    $query->condition('client_id', $client->client_id, '<>');
  }
  $clients = $query->execute()
      ->fetchAll();

  if (!empty($clients)) {
    form_error($form['agency'], t('Machine name must be unique.'));
  }

  field_attach_validate('vopros_client', $client, $form, $form_state);
}

/**
 * Submit handler for vopros_question_client_form().
 */
function vopros_question_client_form_submit(&$form, &$form_state) {
  $client = $form_state['values']['client'];

  $info = entity_get_property_info('vopros_question_client');
  dpm($info);
  foreach (array_keys($info['properties']) as $key) {
    if (isset($form_state['values'][$key])) {
      $client->{$key} = $form_state['values'][$key];
    }
  }

  // Notify field widgets.
  field_attach_submit('vopros_question_client', $client, $form, $form_state);

  $client->save();

  // Add the client to the $form_state for other modules to use.
  $form_state['client'] = $client;
  $form_state['redirect'] = 'admin/vopros/clients/' . $client->client_id;
}
