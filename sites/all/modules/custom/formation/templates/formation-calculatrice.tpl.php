<?php
/**
 * Available variables:
 * - $form: the form to render.
 */
?>

<table>
  <tr>
    <td colspan='2'><?php print drupal_render($form['presentation']) ?></td>
    <td><?php print drupal_render($form['resultat']) ?></td>
  </tr>
  <tr>
    <td><?php print drupal_render($form['value1']) ?></td>
    <td><?php print drupal_render($form['operator']) ?></td>
    <td><?php print drupal_render($form['value2']) ?></td>
  </tr>
  <tr>
    <td colspan='3'><?php print drupal_render_children($form) ?></td>
  </tr>
</table>