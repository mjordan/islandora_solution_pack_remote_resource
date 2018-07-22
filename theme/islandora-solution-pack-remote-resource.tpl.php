<?php

/**
 * @file
 * This is the template file for Islandora Remote Resource objects.
 */
?>

<div class="islandora-remote-resource-object islandora">
  <div class="islandora-remote-resource-content-wrapper clearfix">
    <?php if ($display_tn): ?>
      <div class="islandora-remote-resource-display-tn">
        <?php print $tn_markup; ?>
      </div>
    <?php endif; ?>
    <?php if (isset($url)): ?>
      <div class="islandora-remote-resource-content">
        <?php print t('This resource is hosted on another website:'); ?>
        <?php if ($linktext): ?>
          <?php print ' ' . l($linktext, $url); ?>
        <?php else: ?>
          <?php print ' ' . l($url, $url); ?>
        <?php endif; ?>
      </div>
    <?php endif; ?>
  </div>
  <div class="islandora-remote-resource-metadata">
      <?php if (isset($description)): ?>
        <?php print $description; ?>
      <?php endif; ?>
    <?php if($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $collection): ?>
            <li><?php print l($collection->label, "islandora/object/{$collection->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
    <?php print $metadata; ?>
  </div>
</div>
