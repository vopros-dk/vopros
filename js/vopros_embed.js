/**
 *
 * This is not included with drupal_add_js, but outputted directly to
 * the browser. Some !variables will be replaced, @see
 * vopros_embed_question_modal_js().
 */

// Creating our own scope as to not pollute the global scope.
(function() {
  var $ = jQuery.noConflict(true);

  // Add stylesheet for magnificPopup.
  $('head').append('<link rel="stylesheet" href="!mag_style" type="text/css" />');
  // Add our own CSS.
  $('head').append('<link rel="stylesheet" href="!vopros_style" type="text/css" />');


  $(document).ready(function() {
    var query = {};
    if (typeof window.ask_vopros != "undefined") {
      query = window.ask_vopros;
    }

    // URL is always set to the current location.
    query.url = window.location.href;

    // Fix urls and attach Magnific popup to the link.
    $('a[href*="!ask_url"]').querystring(query).magnificPopup({
      type: 'iframe',
    });
  });
})();
