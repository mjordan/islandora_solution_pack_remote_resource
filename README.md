# Islandora Remote Resource Solution Pack

An Islandora Solution Pack that allows for ingesting and viewing resources located elsewhere on the web.

## Introduction

This solution pack provides tools for ingesting and viewing XML OBJ files in Islandora much like those offered by other solutions packs. These files could be TEI, EAD, DocBook, SVG, or locally defined XML. Some differences between this solution pack and most others include:

* It does not generate any derivatives.
* It is designed specifically to direct the user to another website (or at least another URL).

Users may upload a thumbnail image for each XML object. Objects managed by this solution pack can also have metadata datastreams such as MODS, just like other Islandora objects do.

## Requirements

* [Islandora](https://github.com/Islandora/islandora)

## Configuration

## Metadata and description display

## Batch loading

Objects managed by this module cannot be loaded using the [Islandora Batch](https://github.com/Islandora/islandora_batch), but a custom Drush-based loader is available in the `modules` subdirectory. Its README provides details on preparing content and ingesting it.

## Indexing 

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions. Please open a GitHub issue before opening a pull request.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
