
/**
 * Add a close button to the page if we've been iframed.
 */
(function ($) {
  $(document).ready(function () {
    if (window != window.top) {
      $('.close-popup.element-hidden').each(function () {
        $(this).click(function () {
          // Post message to parent frame.
          window.top.postMessage('ask_vopros_close', '*');
        }).removeClass('element-hidden');
      });
    }
  });
})(jQuery);
