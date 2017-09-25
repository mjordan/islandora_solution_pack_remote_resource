# Islandora Remote Resource Solution Pack

An Islandora Solution Pack that allows for ingesting and viewing stub objects that refer to resources located elsewhere on the web.

## Introduction

This solution pack manages stub objects that refer to resources located elsewhere on the web. The URL of the remote resource is stored in the object's OBJ datastream, which is a simple text file (all it contains is the remote URL string). On viewing a 'remote resource' object, the user is shown an interstitial page with a clickable link to the remote resource. There is an admin option to redirect the user directly to the resource. Each remote resource object can also have a MODS file so that it can be discovered via search.

The main use case for this solution pack is to allow an Islandora instance to aggregate discovery data (MODS, DC, etc.) from other sites via OAI-PMH or other methods.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Configuration

Configuration options are available at `admin/islandora/solution_pack_config/remote_resource`.

## Batch loading and synching

Objects managed by this module cannot be loaded using the [Islandora Batch](https://github.com/Islandora/islandora_batch), but a custom Drush-based loader is available in the `modules` subdirectory. This module also handles synching datasteams on existing objects that have changed. Its README provides details on preparing content and ingesting/updating it.

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions. Please open a GitHub issue before opening a pull request.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
