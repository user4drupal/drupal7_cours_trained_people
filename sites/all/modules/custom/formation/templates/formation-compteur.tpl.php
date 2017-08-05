<?php
// Available variables:
//   - $node: the node object.
//   - $compteur: number of times the current node has been seen in full mode.
//   - $classes: list of class sent.
?>

<p class='<?php print $classes; ?>'>
    <?php print t('The node %title has been viewed %compteur time(s). (TEMPLATE)', 
    		array('%title' => $node->title, '%compteur' => $compteur)); ?>
</p>