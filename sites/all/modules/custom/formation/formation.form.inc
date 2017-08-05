 <?php

 /**
  * Fonction déclarant le formulaire de la calculatrice.
  */
 function formation_calculatrice($form, &$form_state) {
   $form = array();
   // Champ destiné à afficher le résultat du calcul.
   // On teste d'abord l'existance du résultat, c'est à dire si un calcul a été fait.
   if (isset($form_state['resultat'])) {
     $form['resultat'] = array(
       '#type' => 'markup',     // Type par défaut.
       '#markup' => '<h2>' . $form_state['resultat'] . '</h2>',
       '#prefix' => '<div style="background:yellow">',
       '#suffix' => '</div>',
     );
   }
	$form['presentation'] = array(
	  '#type' => 'select',
	  '#title' => t('Choose a display type.'),
	  '#options' => array(
	    'radios' => t('Radios'),
	    'select' => t('Select'),
	  ),
	  '#ajax' => array(
	    'wrapper' => 'operation',
	    'callback' => 'presentation_ajax_callback',
	  )
	);
   $form['value1'] = array(
     '#type' => 'textfield',
     '#title' => t('First value'),
     '#description' => t('Enter first numeric value.'),
     '#size' => '20',
     '#required' => TRUE,
     '#prefix' => '<div class="formation-bg">',
     '#suffix' => '</div>',
     '#default_value' => '1',
   );

   $operation_type = isset($form_state['values']['presentation']) ?
     $form_state['values']['presentation'] : 'radios';
   $form['operator'] = array(
     '#type' => $operation_type,
     '#title' => t('Operator'),
     '#description' => t('Choose an operator.'),
     '#options' => array(
       'addition'       => t('Plus'),
       'soustraction'   => t('Minus'),
       'multiplication' => t('Multiply'),
       'division'       => t('Divide'),
     ),
     '#default_value' => 'addition',
     '#prefix' => '<div id="operation">',
     '#suffix' => '</div>',
   );
   $form['value2'] = array(
     '#type' => 'textfield',
     '#title' => t('Second value'),
     '#description' => t('Enter second numeric value.'),
     '#size' => '20',
     '#required' => TRUE,
     '#default_value' => '2',
   );
   $form['calculate'] = array(
     '#type' => 'submit',
     '#value' => t('Calculate'),
   );
   $form['affiche_resultat'] = array(
     '#type' => 'select',
     '#title' => t('Result view method'),
     '#description' => t('Choose how to display the result.'),
     '#options' => array(
       'set_message' => t('Use of drupal_set_message().'),
       'message_redirect' => t('Use of drupal_set_message() on a redirect page.'),
       'argument_redirect' => t('Use of path argument on a redirect page'),
       'session_redirect' => t('Use of $_SESSION to store result and a redirect page'),
       'result_in_form' => t('Show result in form.'),
     ),
     '#default_value' => 'addition',
     '#prefix' => '<div id="operation">',
     '#suffix' => '</div>',
   );

   return $form;
 }

 // Ajax callback functions.
 function presentation_ajax_callback($form, &$form_state) {
   // On renvoie la portion du formulaire php
   // que l'on veut modifier (ici le champ opération).
   return $form['operator'];
 }

 /**
  * Fonction de validation de la calculatrice appelée automatiquement
  * par Drupal.
  */
 function formation_calculatrice_validate($form, &$form_state) {
   $operation = $form_state['values']['operator'];
   $valeur1   = $form_state['values']['value1'];
   $valeur2   = $form_state['values']['value2'];
   $label1    = $form['value1']['#title'];
   $label2    = $form['value2']['#title'];

   if (!is_numeric($valeur1)) {
     form_set_error('value1', t("%label1 must be a numeric value !",
       array('%label1' => $label1)));
   }
   if (!is_numeric($valeur2)) {
     form_set_error('value2');
     $form_state['complete form']['value2']['#description'] = '<span class="error">' . t("%label2 must be a numeric value !", array('%label2' => $label2)) . '</span>';
   }
   if ($valeur2 == '0' && $operation == 'division') {
     form_set_error('value2', t("Can't divide by zero !!!"));
   }
   // Lorsque l'on affiche le résultat dans le formulaire, il faut supprimer sa
   // valeur afin qu'il ne soit pas réaffiché pour un autre calcul qui ne passe
   // pas la validation.
   unset($form_state['complete form']['resultat']);
 }

 /**
  * Fonction de traitement de la calculatrice appelée automatiquement
  * par Drupal lorsqu'aucune erreur de validation n'a été détectée.
  */
 function formation_calculatrice_submit($form, &$form_state) {
   $operation = $form_state['values']['operator'];
   $valeur1   = $form_state['values']['value1'];
   $valeur2   = $form_state['values']['value2'];

   // Si le champ du chois de la méthode d'affichage est masqué, alors on choisit
   // par défaut d'afficher le résultat avec la fonction drupal_set_message().
   if (isset($form_state['values']['affiche_resultat'])) {
     $affiche_resultat = $form_state['values']['affiche_resultat'];
   } else $affiche_resultat = 'set_message';

   // On fait le calcul en fonction de l'opération choisie.
   switch ($operation) {
     case 'addition':
       $resultat  = $valeur1 + $valeur2;
       // Afin de réafficher le calcul effectué, on assigne le symbole mathématique
       // à l'opération choisie.
       $operation = '+';
       break;
     case 'soustraction':
       $resultat  = $valeur1 - $valeur2;
       $operation = '-';
       break;
     case 'division':
       $resultat  = $valeur1 / $valeur2;
       $operation = '/';
       break;
     case 'multiplication':
       $resultat  = $valeur1 * $valeur2;
       $operation = '*';
       break;
   }

   // Affichage du résultat.
   switch ($affiche_resultat) {
     case 'set_message':
       // Affichage via drupal_set_message().
       drupal_set_message($resultat);
       break;
     case 'message_redirect':
       // Affichage via drupal_set_message() sur une page de redirection.
       drupal_set_message($resultat);
       $form_state['redirect'] = 'resultat';
       break;
     case 'argument_redirect':
       // Affichage sur une page de redirection en passant le résultat dans l'URL.
       $form_state['redirect'] = 'resultat/' . $resultat;
       break;
     case 'session_redirect':
       // Affichage sur une page de redirection en passant le résultat en session.
       $_SESSION['output'] = $valeur1 . ' ' . $operation . ' ' . $valeur2 . ' = ' . $resultat;
       $form_state['redirect'] = 'resultat';
       break;
     case 'result_in_form':
       // Affichage du résultat dans le formulaire.
       $form_state['resultat'] = $resultat;
       $form_state['rebuild'] = TRUE;
       break;
   }
 }

 /**
  * Formulaire d'admin pour le choix de la couleur des blocs.
  */
 function formation_block_color_admin_form($form, &$form_state) {
   $form = array();

   $form['formation_block_color_region'] = array(
     '#title' => t('Choose a region.'),
     '#type' => 'select',
     '#options' => system_region_list(variable_get('theme_default')),
     '#default_value' => variable_get('formation_block_color_region', ''),
   );

   $form['formation_block_color'] = array(
     '#title' => t('Choose a color for the blocks.'),
     '#type' => 'select',
     '#options' => array(
       '' => t('none'),
       'blue-class' => t('blue'),
       'orange-class' => t('orange'),
       'green-class' => t('green'),
     ),
     '#default_value' => variable_get('formation_block_color', ''),
   );

   return system_settings_form($form);
 }