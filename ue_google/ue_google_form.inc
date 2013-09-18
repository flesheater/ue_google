<?php 
/*
 * @file
 * Google map module
 * The google map configuration form
 * 
 */

/**
 * The configuration form
 */
function ue_google_form($form, &$form_state) {

  $query_count = db_select('ue_google_maps');
  $query_count->addExpression('COUNT (map_id)', 'map_count');
  $data_count = $query_count->execute()->fetchObject();
  
  if ($data_count->map_count == 0) {
    $query3 = db_insert('ue_google_maps')
      ->fields(array('google_key', 'marker_latitude', 'marker_longitude', 'marker_name', 'center_latitude', 'center_longitude', 'start_zoom'))
      ->values(array('google_key' => '', 'marker_latitude' => 0.0, 'marker_longitude' => 0.0, 'marker_name' => '', 'center_latitude' => 0.0, 'center_longitude' => 0.0, 'start_zoom' => 16));
     $terd = $query3->execute();
    
    $map_data = db_select('ue_google_maps', 'g')->fields('g')->execute()->fetchObject();
  }
  else {
  
    $map_data = db_select('ue_google_maps', 'g')->fields('g')->execute()->fetchObject();
  }

  $form['google_key'] = array(
    '#type' => 'textfield',
    '#default_value' => $map_data->google_key,
    '#title' => t('Google APi Key'),
    '#description' => t('Get google maps api key from <a href="https://developers.google.com/maps/signup" target="blank">here</a>.'),
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  $form['marker_latitude'] = array(
    '#type' => 'textfield',
    '#default_value' => $map_data->marker_latitude,
    '#title' => t('Marker Latitude'),
    '#description' => t('The latitude of the marker'),
    '#size' => 40,
    '#maxlength' => 120,
  );

  $form['marker_longitude'] = array(
    '#type' => 'textfield',
    '#default_value' => $map_data->marker_longitude,
    '#title' => t('Marker longitude'),
    '#description' => t('The longitude of the marker'),
    '#size' => 40,
    '#maxlength' => 120,
  );

  $form['marker_name'] = array(
    '#type' => 'textfield',
    '#default_value' => $map_data->marker_name,
    '#title' => t('The marker name'),
    '#description' => t('The marker name. This field will also be used if no 
                         coordinates are specified for the marker and the 
                         centering of the map to generate such.'),
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  $form['center_latitude'] = array(
    '#type' => 'textfield',
    '#default_value' => $map_data->center_latitude,
    '#title' => t('The latitude'),
    '#description' => t('The latitude of the map'),
    '#size' => 40,
    '#maxlength' => 120,
  );

  $form['center_longitude'] = array(
    '#type' => 'textfield',
    '#default_value' => $map_data->center_longitude,
    '#title' => t('The longitude'),
    '#description' => t('The longitude of the map'),
    '#size' => 40,
    '#maxlength' => 120,
  );

  $form['start_zoom'] = array(
    '#type' => 'textfield',
    '#default_value' => $map_data->start_zoom,
    '#title' => t('Start zoom'),
    '#description' => t('The start zoom of the map'),
    '#size' => 40,
    '#maxlength' => 120,
    '#required' => TRUE,
  );

  
  // Submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save map settings'),
  );

  return $form;
}

function ue_google_form_validate(&$form, &$form_state) {
  if ($form_state['values']['marker_latitude'] < 0) {
    form_set_error('marker_latitude', t('The latitude can\'t be negative'));
  }
}

function ue_google_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  
  if (empty($values['marker_latitude']) || empty($values['marker_longitude'])) {
    $coordinates = _ue_google_get_coordinates_based_on_address($values['marker_name']);
    $values['marker_longitude'] = $coordinates[0]->lon;
    $values['marker_latitude'] = $coordinates[0]->lat;
  }

  if (empty($values['center_longitude']) || empty($values['center_latitude'])) {
    $coordinates = _ue_google_get_coordinates_based_on_address($values['marker_name']);
    $values['center_longitude'] = $coordinates[0]->lon;
    $values['center_latitude'] = $coordinates[0]->lat;
  }

  
  $query = db_update('ue_google_maps')
    ->fields(array(
        'google_key'       => $values['google_key'], 
        'marker_latitude'  => $values['marker_latitude'], 
        'marker_longitude' => $values['marker_longitude'], 
        'marker_name'      => $values['marker_name'], 
        'center_latitude'  => $values['center_latitude'], 
        'center_longitude' => $values['center_longitude'], 
        'start_zoom'       => $values['start_zoom']
      ))->execute();
}
