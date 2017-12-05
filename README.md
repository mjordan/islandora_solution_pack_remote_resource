# Islandora Remote Resource Solution Pack

An Islandora Solution Pack that allows for ingesting and viewing stub objects that refer to resources located elsewhere on the web.

## Introduction

This solution pack manages stub objects that refer to resources located elsewhere on the web. The URL of the remote resource is stored in the object's OBJ datastream, which is a simple text file (all it contains is the remote URL string). On viewing a 'remote resource' object, the user is shown an interstitial page with a clickable link to the remote resource. There is an admin option to redirect the user directly to the resource. Each remote resource object can additional datastreams such as a MODS file, so that it can be discovered via Islandora's functionality.

The main general use case for this solution pack is to allow an Islandora instance to allow discovery of content hosted elsewhere. A common strategy for getting content from other sites is via OAI-PMH harvests. However, the data used to create remote resource objects need not be from OAI-PMH feeds. Objects managed by this solution pack can be added manually, if desired, or data prepared for batch ingest via a variety of means:

![Overview diagram](https://user-images.githubusercontent.com/403918/33591649-8c6740c8-d93b-11e7-8b66-75e03701c1b4.png)

Some specific use cases for this solution pack include:

* you have a general repository (Islandora) and an IR (not Islandora), you want to make both searchable in the same place
* you have a general repository (Islandora), but you have some small specialized collections that don't fit into Islandora easily or that you don't have the resources to migrate
* you want to create curated Islandora collections that include content from a variety of platforms

Remote resource objects are full Islandora objects, so they can be themed, their usage can be tracked, they can fit into ingestion workflows, etc.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Configuration

Configuration options are available at `admin/islandora/solution_pack_config/remote_resource`.

## Batch loading and syncing

Objects managed by this module cannot be loaded using the standard Islandora Batch module, but a custom batch ingest module is available in the `modules` subdirectory. Objects can be ingested either using a GUI or using a Drush command, as with other Islandora batch modules. This batch module also handles syncing datasteams on existing objects that have changed at their remote source. Its README provides details on preparing content and ingesting/syncing it.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Bug reports, use cases and suggestions are welcome, as are pull requests. See CONTRIBUTING.md for more information.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
