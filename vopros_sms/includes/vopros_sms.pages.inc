<?php

/**
 * @file
 * Public page callback functions.
 */

/**
 * Page callback to create questions from SMS.
 *
 * Used by SMS service to create questions
 */
function vopros_sms_create_sms() {
  $default_values = array(
    'user_answer_preference' => 'sms',
    'agency_id' => 'SMS1919',
  );

  $question = vopros_question_new($default_values);
  $message = isset($_GET['msg']) ? iconv('CP1252', 'UTF-8', $_GET['msg']) : '';
  $phone = isset($_GET['origin']) ? $_GET['origin'] : '';
  if ($message && $phone) {
    $words = explode(' ', $message);
    $keyword = array_shift($words);
    $question->qanda_service_name = 'sms';
    $question->question_content = implode(' ', $words);
    $question->user_mobile_phone = $phone;
    $question->question_deadline = 2;
    if (module_exists('vopros_editorial') && $tid = variable_get('vopros_question_default_sms_editoral', '')) {
      $question_wrapper = entity_metadata_wrapper('vopros_question', $question);
      $question_wrapper->vopros_editorial = $tid;
    }
    if (!trim($question->question_content)) {
      $question->question_content = t('Empty question - no content delivered from SMS service.');
    }
    $question->save();
  }
  $output =
'<?xml version="1.0" encoding="iso-8859-1"?>
  <action>
    <type>terminate</type>
  </action>';
  print $output;
  exit();
}
