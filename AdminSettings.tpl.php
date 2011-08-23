<div id="admin-settings-main">
  <div id="admin-settings-table">
    <table>
      <tr>
        <th>Content Model</th>
        <th>Edit</th>
        <th>Remove</th>
      </tr>
      <?php foreach ($content_models as $content_model) : ?>
      <?php table(); ?>
        <tr>
          <td><?php print $model ?></td>
          <td><?php print $dsid ?></td>
          <td><?php print $title_field ?></td>
          <td><?php print $form_name ?></td>
          <td><?php print $transform ?></td>
          <td><?php print ($has_template) ? 'True' : 'False' ?></td>
          <td><?php print l("Delete", "admin/content/model/forms/remove/$id") ?></td>
        </tr>
      <?php endforeach; ?>
      </table>
    </div>
    <div id="content-model-actions">
    <?php print $form ?>
  </div>
</div>