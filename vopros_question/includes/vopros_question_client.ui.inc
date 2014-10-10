<?php

/**
 * @file
 * Provides a controller for building an vopros client overview form.
 */

/**
 * UI controller class for the admin interface.
 */
class VoprosClientUIController extends VoprosUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Display a list of clients.';
    return $items;
  }
}
