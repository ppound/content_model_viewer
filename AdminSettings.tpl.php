<br/>
<div id="admin-settings-main">
  <h3><?php print t('Custom Settings') ?></h3>
  <?php if (count($content_models) > 0) : ?>
    <div id="admin-settings-table">
      <?php $table_rows = array(); ?>
      <?php foreach ($content_models as $content_model) : ?>
        <?php $pid = $content_model['pid']; ?>  
        <?php $label = $content_model['label']; ?>  
        <?php $edit = l('Edit', "admin/settings/content_model_viewer/$pid/edit"); ?>
        <?php $remove = l('Delete', "admin/settings/content_model_viewer/$pid/delete"); ?>
        <?php $table_rows[] = array($label, $pid, $edit, $remove) ?>
      <?php endforeach; ?>
      <?php print theme_table(array(t('Content Model'), t('pid'), t('Edit Settings'), t('Delete Settings')), $table_rows); ?>
    </div>
  <?php else : ?>
    <div id="admin-settings-table-missing">There are no custom viewer settings for any Content Models.</div>
  <?php endif; ?>
  <div id="admin-settings-add-form">
    <?php print drupal_get_form('content_model_viewer_add_custom_settings_form'); ?>
  </div>
  <div id="admin-settings-custom-form">
    <?php print drupal_get_form('content_model_viewer_custom_settings_form'); ?>
  </div>
  <div id="admin-settings-default-form">
    <h3><?php print t('Default Settings') ?></h3>
    <?php print drupal_get_form('content_model_viewer_datastream_rules_form'); ?>
  </div>
</div>