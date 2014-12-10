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

  $form['embed']['answer_preference'] = array(
    '#type' => 'radios',
    '#title' => t('Default answer method'),
    '#options' => _vopros_embed_answer_preferences(),
    '#input' => FALSE,
    '#description' => t('Which answer method should be initially selected?'),
  );

  // The Drupal.formatString JS function doesn't support replacing each
  // placeholder more than once, so use different placeholders.
  // Using async and defer is the currently recommend practice,
  // @see https://www.igvita.com/2014/05/20/script-injected-async-scripts-considered-harmful/
  $code = '<script src="!script_base/question_modal.js" async defer></script>
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

  return system_settings_form($form);
}
