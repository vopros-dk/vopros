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
  $form['banana'] = array(
    '#type' => 'textfield',
    '#title' => t('test'),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'vopros_embed') . '/vopros_embed.css'),
    ),
  );

  return $form;
}
