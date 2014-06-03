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
    'css' => array(drupal_get_path('module', 'vopros_embed') . '/vopros_embed.css'),
  );

  $form['question_content'] = array(
    '#type' => 'textarea',
    '#title' => t('Question content'),
    '#required' => TRUE,
    '#default_value' => '',
  );

  $form['user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => isset($_GET['name']) ? check_plain($_GET['name']) : '',
  );

  $form['user_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => isset($_GET['mail']) ? check_plain($_GET['mail']) : '',
  );

  $form['user_postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#default_value' => isset($_GET['zip']) ? check_plain($_GET['zip']) : '',
  );

  $deadline_options = _vopros_question_deadline();
  $form['question_deadline'] = array(
    '#type' => 'select',
    '#title' => t('Deadline'),
    '#options' => $deadline_options,
    '#default_value' => -1,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit question'),
    '#attributes' => array('class' => array('primary-action')),
  );

  return $form;
}
