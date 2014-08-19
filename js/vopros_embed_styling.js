/**
 * Styling.
 */
(function ($) {
  $(document).ready(function () {

    /**
     * Set active state/class to the chosen answer type.
     * @TODO: PROPER COMMENTS!
     */
    var active_answer_type = 'active_answer_type';
    var disabled_answer_type = 'disabled_answer_type';
    var answer_type = $('#edit-user-answer-preference');
    var answer_input = $('input', answer_type);

    // Set the active class to the checked radio on page load.
    answer_type.find('input:checked').parent().addClass(active_answer_type);

    answer_type.find('input:disabled').parent().addClass(disabled_answer_type);

    answer_input.change(function() {
      answer_input.parent().removeClass(active_answer_type);
      $(this).parent().addClass(active_answer_type);
    });

    /**
     * Add a "required" sign to the user email field.
     *
     * Because we use the same form and the email isn't required when
     * you want an answer by SMS/text, we then use a custom validation
     * handler to check this but we're missing out on the "required"
     * icon. This is why we manually append it.
     */
    $('#edit-user-email').prev('label').append('<span class="form-required">*</span>');
    $('#edit-user-mobile-phone').prev('label').append('<span class="form-required">*</span>');

    /**
     * Title margin!
     *
     * Set the margin of the title after loading all images (the logo)
     * on the page.
     */
    $(window).load(function(){
      var header = $('.header');
      var title = $('#page-title');
      var header_height = parseFloat(header.outerHeight());
      var header_padding_top = parseFloat(header.css('padding-top'));
      var title_height = parseFloat(title.outerHeight());
      // Subtract title height from the header and divide it by 2.
      // Then subtract the header's top padding.
      title.css('margin-top', ((header_height - title_height) / 2) - header_padding_top);
    });

  });
})(jQuery);
