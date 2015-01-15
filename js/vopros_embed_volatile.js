(function ($) {

  var isTop = null;
  var isVolatile = null;

  Drupal.voprosEmbed = Drupal.voprosEmbed || {};
  Drupal.voprosEmbed.volatile = Drupal.voprosEmbed.volatile || {};
  // Convenience var.
  var volatile = Drupal.voprosEmbed.volatile;

  /**
   * Beforeunload handler.
   *
   * Asks for confirmation if we're volatile.
   */
  var beforeunload = function(e) {
    if (isVolatile === true) {
      // Apparently we need to return at least an empty string to trigger.
      return " ";
    }
    return;
  };

  /**
   * Initialize.
   *
   * If we're the top window, set an beforeunload handler to catch the
   * user leaving the page, and set up a cross frame listener to
   * listen for popup requesting to set the volatility.
   */
  volatile.init = function () {
    isTop = window === window.top;
    if (isTop) {
      // Listen for messages from other frames to set volatility.
      $(window).bind('message', function (e) {
        switch (e.originalEvent.data) {
        case 'vopros_embed_volatile':
          volatile.set(true);
          break;

        case 'vopros_embed_nonvolatile':
          volatile.set(false);
          break;
        }
      });

      // Bind our handler.
      $(window).bind('beforeunload', beforeunload);
    }
  };

  /**
   * Set volatility.
   *
   * If we're volatile, ask user for confirmation before navigating
   * away. If we're top level register it for our own beforeunload
   * handler. If we're not, send an inter-frame message.
   */
  volatile.set = function (state) {
    switch (isTop)  {
      case true:
      isVolatile = state;
      break;

      case false:
      // Post message to parent frame.
      var message = state ? 'vopros_embed_volatile' : 'vopros_embed_nonvolatile';
      window.top.postMessage(message, '*');

      break;
    }
  };

  /**
   * Query volatility.
   *
   * Returns whether we're volatile or not.
   */
  volatile.is = function () {
    return isVolatile;
  };

  volatile.init();

})(jQuery);
