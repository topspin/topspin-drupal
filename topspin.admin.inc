<?php
// $Id: $

/**
 *
 *  @file
 *  Administrative functionality for Topspin Media module.
 *
 */


/**
 * System settings form.
 * 
 * @return struct
 * - Settings form array.
 */
function topspin_settings_form() {
  
  // General.
  $form['general'] = array('#type' => 'fieldset', '#weight' => 0, '#title' => t('General'), '#description' => '', '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['general']['topspin_show_empty_cart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show empty cart?'),
    '#description' => t("Should the cart UI appear when it's empty?"),
    '#options' => array(
      0 => t('No'),
      1 => t('Yes'),
    ),
    '#default_value' => variable_get('topspin_show_empty_cart', 1),
  );
  
  // Account.
  $form['account'] = array('#type' => 'fieldset', '#weight' => 4, '#title' => t('Account'), '#description' => t('Your account information on Topspin.'), '#collapsible' => TRUE, '#collapsed' => TRUE);
  $form['account']['topspin_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#description' => t('The API key you receive from Topspin to use their API with.'),
    '#required' => TRUE,
    '#default_value' => variable_get('topspin_api_key', ''),
  );
  $form['account']['topspin_api_user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#description' => t('The user to use to access the API (usually your email address).'),
    '#required' => TRUE,
    '#default_value' => variable_get('topspin_api_user', ''),
  );
  $form['account']['topspin_api_artist_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Offers Artist ID'),
    '#description' => t("The Artist ID of the artist who's Offers you want to import."),
    '#required' => FALSE,
    '#default_value' => variable_get('topspin_api_artist_id', ''),
  );
  
  // Blocks.
  $form['block'] = array('#type' => 'fieldset', '#weight' => 2, '#title' => t('Blocks'), '#description' => t('Choose which offers you would like to be usable as a block.'), '#collapsible' => TRUE, '#collapsed' => TRUE);
  
  // Create an array by artists.
  $result = topspin_request('offers');
  $artists = array();
  if ($result->offers) {
    foreach ($result->offers as $index => $offer) {
      if (!isset($artists[$offer->artist_id])) {
        $artists[$offer->artist_id] = array();
      }
      $artists[$offer->artist_id][] = $offer;
    }
  }
  
  // Show offers organized by artist.
  foreach ($artists as $artist_id => $offers) {
    foreach ($offers as $index => $offer) {
      // Add artist fieldset if needed. We do it here so that we have easy access to the artist name.
      if (!isset($form['block'][$artist_id])) {
        $form['block'][$artist_id] = array('#type' => 'fieldset', '#title' => check_plain($offer->artist_name), '#collapsible' => TRUE, '#collapsed' => TRUE);
      }
      // Add all "block enable" checkboxes.
      $form['block'][$artist_id]['topspin_enabled_offer_block_'. $offer->id] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($offer->name),
        '#default_value' => variable_get('topspin_enabled_offer_block_'. $offer->id, 0),
      );
    }
  }
  
  // Static store.
  $form['static_store'] = array('#type' => 'fieldset', '#weight' => 3, '#title' => t('Static Store'), '#description' => '', '#collapsible' => TRUE, '#collapsed' => TRUE);
  if ($store_path = variable_get('topspin_store_path', '')) {
    $store_path = t('Currently set to: ') . url($store_path, array('absolute' => TRUE));
  } else {
    $store_path = t('Currently disabled');
  }
  $form['static_store']['topspin_store_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('The internal Drupal path to the store page on your site. !store-path. To remove the page from the system simply remove this value. No trailing or starting slash (/).',
                        array('!store-path' => '<strong>'. $store_path .'</strong>')),
    '#default_value' => variable_get('topspin_store_path', 'store'),
  );
  $form['static_store']['topspin_store_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('The title to use for the page.'),
    '#default_value' => variable_get('topspin_store_title', 'Store'),
    '#required' => TRUE,
  );
  $form['static_store']['topspin_store_grid_size'] = array(
    '#type' => 'select',
    '#title' => t('Total Per Row'),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
    '#description' => t('The total number of offers to display per row.'),
    '#default_value' => variable_get('topspin_store_grid_size', 4),
    '#required' => TRUE,
  );
  $form['static_store']['topspin_store_page_limit'] = array(
    '#type' => 'select',
    '#title' => t('Total Per Page'),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25)),
    '#description' => t('The total number of offers to display per page.'),
    '#default_value' => variable_get('topspin_store_page_limit', 4),
    '#required' => TRUE,
  );
  $form['static_store']['topspin_store_filter_product_type'] = array(
    '#type' => 'select',
    '#title' => t('Product Type'),
    '#description' => t('Which product type to display.'),
    '#options' => array(
      '' => t('All'),
      'image' => t('Image'),
      'video' => t('Video'),
      'track' => t('Track'),
      'album' => t('Album'),
      'package' => t('Package'),
      'other_media' => t('Other media'),
      'merchandise' => t('Merchandise'),
    ),
    '#default_value' => variable_get('topspin_store_filter_product_type', ''),
  );
  $form['static_store']['topspin_store_sort_column'] = array(
    '#type' => 'select',
    '#title' => t('Sorting Field'),
    '#description' => t('Which field to sort the offers by.'),
    '#options' => array(
      'created_at' => t('Created at'),
      'name' => t('Name'),
      'offer_type' => t('Offer type'),
      'artist_id' => t('Artist ID'),
    ),
    '#default_value' => variable_get('topspin_store_sort_column', 'created_at'),
  );
  $form['static_store']['topspin_store_sort_direction'] = array(
    '#type' => 'select',
    '#title' => t('Sort Direction'),
    '#description' => t('Which direction to sort the offers.'),
    '#options' => array(
      'asc' => t('Ascending'),
      'desc' => t('Descending'),
    ),
    '#default_value' => variable_get('topspin_store_sort_direction', 'asc'),
  );
  
  // Advanced.
  $form['advanced'] = array('#type' => 'fieldset', '#weight' => 999, '#title' => t('Advanced'), '#description' => t('Only for experts.'), '#collapsible' => TRUE, '#collapsed' => TRUE);
  $form['advanced']['topspin_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#description' => t('The URL to the API (no trailing slash).'),
    '#required' => TRUE,
    '#default_value' => variable_get('topspin_api_url', 'https://app.topspin.net/api/v1'),
  );
  $form['advanced']['topspin_store_search_parameters'] = array(
    '#type' => 'textfield',
    '#title' => t('Store Search Parameters'),
    '#description' => t('Provide a custom string of parameters to pass through the offer API. Separate parameters with & (i.e. product_type=merchandise&artist_id=2).
                        More information on possible parameters is available in our !link. <strong>Note: </strong> 
                        Using this option will <strong>void</strong> all configured parameters above in the store settings.',
                        array('!link' => l('documentation', 'https://docs.topspin.net/tiki-index.php?page=Offers+API&structure=Dev+Center'))),
    '#default_value' => variable_get('topspin_store_search_parameters', ''),
  );
  $form['advanced']['topspin_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug Mode'),
    '#description' => t('When enabled, debug information is displayed on the screen when requests are made to the Topspin API.'),
    '#default_value' => variable_get('topspin_debug', FALSE),
    '#return_value' => TRUE,
  );
  
  $form = system_settings_form($form);
  
  // Add a custom submit handler and make sure it gets hit last.
  $form['#submit'][] = 'topspin_settings_form_submit';
  
  return $form;
}


/**
 *  Settings form submit callback. Handle saving of enabled blocks.
 *
 *  @param struct
 *  - Form.
 *  @param struct
 *  - Form state.
 */
function topspin_settings_form_submit($form, &$form_state) {
  $enabled_offers = array();
  
  // Get all offers.
  $result = topspin_request('offers');
  if ($result->offers) {
    
    // Build a structured array of offers to enable as blocks.
    foreach ($form['#post'] as $name => $value) {
      // Add enabled blocks to $enabled_offers array for saving later.
      if (strpos($name, 'topspin_enabled_offer_block_') === 0) {
        $offer_id = array_pop(explode('_', $name));
        $enabled_offers[$offer_id] = topspin_get_offer_by_id($offer_id, $result->offers);
      }
    }
  }
  
  // Save enabled blocks.
  topspin_enabled_offer_blocks($enabled_offers);
  
  // Clear menu cache so that the store link is represented accurately.
  module_invoke('menu', 'rebuild');
}

