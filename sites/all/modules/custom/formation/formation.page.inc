 <?php


/**
 * Fonction de callback pour la page "formation".
 */
function formation_page() {
  $name = user_is_logged_in() ? $GLOBALS['user']->name : t('guest');
  $output = '<p>' . t('You are on the Formation page. Your name is %name',
                    array('%name' => $name)) . '</p>';

  return $output;
}

/**
 * Fonction de callback pour le flux RSS.
 * Cette fonction fabrique le contenu du flux et l'envoie au navigateur.
 */
function formation_flux_page($arg1 = '1', $arg2 = NULL, $arg3 = NULL) {
  // Fabrique le XML du flux.
  // Comme la fabrication d'un flux RSS sort du cadre de notre exercice,
  // on utilise une fonction prête à l'emploi, créée spécialement pour
  // la formation. Cette fonction se trouve dans le module "formation_tools".
  $xml = genere_flux(array($arg1, $arg2, $arg3));
  // Change le Content-Type pour prévenir le navigateur qu'on lui envoie
  // un flux, et pas une page web classique.
  drupal_add_http_header('Content-Type', 'application/rss+xml; charset=utf-8');
  // Affiche le flux dans le navigateur.
  print $xml;
}

/**
 * Fonction de callback pour la page listant les noeuds d'un certain type.
 *
 * L'argument $nodetype correspond à ce qui sera tapé dans l'URL, à droite
 * du chemin "formation/liste-noeuds".
 * Exemple : quand qqn visitera monsite.com/formation/liste-noeuds/page,
 *           la fonction ci-dessous sera appelée avec l'argument "page".
 */
function formation_liste_noeuds_page($nodetype = '') {

  // Une amélioration possible (non demandée dans l’exercice)
  // serait d’afficher la liste de tous les types de noeud
  // au cas où aucun $nodetype n’est précisé.
  // La fonction node_type_get_names() permet de récupérer
  // dans la base de données la liste des types de noeud existants.
  $all_types = node_type_get_names();

  if (!$nodetype) {
    foreach ($all_types as $name => $title) {
      $items[] = l(t($title), current_path() . '/' . $name);
    }
    $output = theme('item_list', array('items' => $items));
  }
  else {
    // Récupère le NID et le titre de tous les noeuds de type $nodetype.
    // Requête "classique".
    /*$result = db_query("SELECT nid, title
              		FROM {node}
              		WHERE type = :nodetype
              		ORDER BY title ASC",
    		            array(':nodetype' => $nodetype)
    		      );*/
    // Variante : on ne liste que 10 noeuds en utilisant une pagination.
    $query = db_select('node', 'n')
              ->fields('n', array('nid', 'title'))
              ->condition('type', $nodetype, '=')
              ->orderBy('title', 'ASC')
  						->extend('PagerDefault')     //Pager Extender
              ->limit(10);
    $result = $query->execute();

    $type_name = $all_types[$nodetype];
    // Crée un lien vers chaque noeud, et stocke le lien dans un tableau $items.
    $items = array();
    foreach ($result as $record) {
      // La fonction l() renvoie un lien <a href="">...</a>.
      // Elle prend 2 arguments :
      //   - le texte du lien (ici : le titre du noeud)
      //   - le chemin du lien (ici : node/NID)
      // Si les alias d'URL sont activés, la fonction l() les utilisera
      // automatiquement pour construire le lien.
      $items[] = l($record->title, 'node/' . $record->nid);
    }
    // Prépare les variables pour theme_item_list().
    $variables = array(
  	  'items' => $items,
  	  'title' => 'List of ' . $type_name,
    );
    // Renvoie la liste des noeuds formatés avec la fonction theme_item_list().
    $output = theme('item_list', $variables);
    // On ajoute ici la pagination avec la fonction theme_pager().
    $output .= theme('pager');
  }

  return $output;
}

/**
 * Fonction de callback pour la page "résultat".
 */
function formation_resultat_page($resultat = '') {
	// Vérifie si les données ont été stockées dans la session.
  $output = isset($_SESSION['output']) ? $_SESSION['output'] : $resultat;
  // Ici on purge ce que l'on a stocké en session.
  if (isset($_SESSION['output'])) {
  	unset($_SESSION['output']);
  }

	return "$output";
}

/**
 * Menu callback.
 * Affiche le compteur d'un noeud.
 */
function formation_statistics_page($node) {
  $compteur = formation_get_compteur($node->nid);

  // Version 1 - Le HTML est généré directement par la fonction de callback.
//  return t("The node %title has been viewed %compteur time(s).",
//               array('%title' => $node->title, '%compteur' => $compteur,));

  // Version 2 - Le HTML est généré par une fonction de thème (c'est mieux !!).
//  return theme('formation_compteur', array('compteur' => $compteur, 'node' => $node));

  // Version 3 - On utilise un Render Array (c'est encore mieux !).
   return array(
     '#theme'    => 'formation_compteur',
     '#compteur' => $compteur,
     '#node'     => $node,
   );
}
