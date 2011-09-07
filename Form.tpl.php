<div id="object-metadata-form">
  <?php if ($action == 'edit') : ?>
    <?php print drupal_get_form('content_model_viewer_edit_metadata_form', $pid); ?>
  <?php else : ?>
    <?php print drupal_get_form('content_model_viewer_ingest_metadata_form', $pid); ?>
  <?php endif ?>
</div>
