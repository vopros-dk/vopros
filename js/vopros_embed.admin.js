
(function ($) {
  Drupal.voprosEmbed = Drupal.voprosEmbed || {};

  /**
   * Function to update embed code with values from input fields.
   */
  var updateCode = function () {
    var settings = Drupal.settings.voprosEmbed;

    var query = [];

    // Add agency.
    query.push('agency_id=' + encodeURIComponent($('#edit-agency-id').val()));

    var email = $('#edit-agency-mail').val();
    if (email) {
      query.push('agency_mail=' + encodeURIComponent(email));
    }

    var parameters = {
      '!script_base': settings.base,
      '!form_base': settings.base,
      '!params': query.join('&')
    };
    var code = Drupal.formatString(settings.template, parameters);
    $('#edit-code').val(code);
  };

  Drupal.behaviors.voprosEmbedLiveCode = {
    attach: function(context, settings) {
      $('#edit-embed input').change(updateCode);
      $('#edit-embed input').keyup(updateCode);
      // Throw away keydown events so the textarea is uneditable, but
      // keep keydown events with modifiers, so keyboard copy works.
      $('#edit-embed textarea').keydown(function(e) {
        if (!(e.altKey || e.ctrlKey || e.metaKey)) {
          e.preventDefault();
          return;
        }
      });
      // Select all when clicked.
      $('#edit-embed textarea').click(function () {
        this.focus();
        this.select();
      });
    }
  };

  // Initialize the code.
  $(document).ready(function() {
    updateCode();
  });
})(jQuery);
