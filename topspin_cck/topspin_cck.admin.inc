<?php
// $Id: $

/**
 *
 *  @file
 *  Management functionality for Topspin Media CCK module.
 *
 */


/**
 * Content form.
 * 
 * @return struct
 * - Settings form array.
 */
function topspin_cck_content_form() {
  // Account.
  $form['imports'] = array('#type' => 'fieldset', '#title' => t('Imports'), '#description' => '', '#collapsible' => TRUE);
  $form['imports']['topspin_override_existing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Overwrite existing nodes?'),
    '#description' => '<span style="color:#ff0000;">'. t('Use the overwrite option with caution. Local overrides to existing imported nodes will be wiped out.') .'</span>',
  );
  
  $form['imports']['topspin_import_button'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh TopSpin Data'),
  );
  
  // Add a custom submit handler and make sure it gets hit last.
  $form['#submit'][] = 'topspin_cck_settings_form_submit';
  
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
function topspin_cck_settings_form_submit($form, &$form_state) {
  $override = NULL;
  if ($form['#post']['topspin_override_existing']) {
    $override = 1;
  }
  topspin_cck_import_offers($override);
}


/**
 * TODO: Finish this functionality that displays imported nodes.
 *
 * @return string
 */
function topspin_cck_existing_imports_list() {
  
  return drupal_get_form('');
}


/**
 * Admin additions to hook_form_alter().
 */
function _topspin_cck_admin_form_alter(&$form, &$form_state, $form_id) {
  // Add Import related stuff to TopSpin configs.
  if ($form_id == 'topspin_settings_form') {
    $form['imports'] = array('#type' => 'fieldset', '#weight' => 1, '#title' => t('Product Importing'), '#description' => '', '#collapsible' => TRUE, '#collapsed' => FALSE);
    $form['imports']['topspin_cck_import_on_cron'] = array(
      '#type' => 'checkbox',
      '#title' => t('Import TopSpin products on CRON?'),
      '#description' => t("Should new products be imported and existing products be updated when CRON is run?"),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
      '#default_value' => variable_get('topspin_cck_import_on_cron', 1),
    );
    $form['imports']['topspin_cck_unpublish_on_import'] = array(
      '#type' => 'checkbox',
      '#title' => t('Unpublish Drupal nodes for new imports?'),
      '#description' => t("Should new products be unpublished the first time they are imported? <em>Note: This will never change the publish status of existing product nodes.</em>"),
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
      '#default_value' => variable_get('topspin_cck_unpublish_on_import', 1),
    );
  }
}

