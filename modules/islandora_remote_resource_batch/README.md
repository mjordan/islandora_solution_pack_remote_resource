# Islandora Remote Resource Batch

Islandora batch module for ingesting objects managed by the Islandora Remote Resouce Solution Pack.

## Requirements

* [Islandora Batch](https://github.com/Islandora/islandora_batch)
* [Islandora Remote_resource Solution Pack](https://github.com/mjordan/islandora_solution_pack_remote_resource)

## Usage

Enable this module, then run its drush command to import objects:

`drush --user=admin islandora_remote_resource_batch_preprocess --target=/path/to/XML/files --namespace=foo --parent=islandora:mycollection`

Then, to perform the ingest:

`drush --user=admin islandora_batch_ingest`

## Preparing your content files for ingesting

This batch module uses filename patterns to identify the files that are intended for specific datastreams. All of the files you are ingesting should go in the same directory (the one you identify in the drush command with the `--target` option), and you must have at least a file for the OJB datastream. All other files are optional. Using this module, you can batch ingest objects of content model 'islandora:sp_remote_resource' having the following datastreams.

### OBJ datastreams

Text file with the extension `.txt`. This file contains the URL of the remote resource, e.g.:

```
http://example.com/some/path
```

### TN datastreams

OBJ file base name with the double extension `.TN.ext` where `.ext` is one of '.jpg', '.jpeg', '.png', or '.gif'.

### MODS datastreams

OBJ file base name with the double extension `.MODS.xml`.

### Additional, arbitrary datastreams 

Objects managed by the Remote Resource Solution Pack only require an OBJ datastream that contains the URL of the remote resource. MODS, DC, and TN datastreams are optional. For local purposes, you may want your objects to have datastreams in addition to a MODS, DC, or TN. To ingest additional datastreams, add files you want to create the datastreams from using the following file naming convention: for a base text file with the name somefile.txt, a datastream with the datastream ID 'JPEG' will be created from a file with the name `somefile.JPEG.jpg`.

The datastream's mimetype will be derived from its extension, in the example above, `.jpg`. Its label will be 'JPEG datastream'. The DSID and extension have no relationship, they just happend to be consistent in this example.

### Example input directories

Two XML files, which will create two objects. The thumbnail and MODS datastreams for the objects will be set to defaults:

```
.
├── foo.txt
└── bar.txt
```

Two text files, which will create two objects. The thumbnail and MODS datastreams for the objects will be created from the file with TN and MODS in their filenames:

```
.
├── foo.txt
├── foo.MODS.xml
├── foo.TN.jpg
├── bar.txt
├── bar.MODS.xml
└── bar.TN.png
```

Three text files, which will create three objects. The object created from `foo.txt` will have its MODS datastream created from the `foo.MODS.xml` and its thumbnail created from defaults; the object created from `bar.txt` will have its TN datastream created from `bar.TN.png` and its MODS datastream created from defaults.

```
.
├── foo.txt
├── foo.MODS.xml
├── bar.txt
├── bar.TN.png
├── baz.txt
```

Same as the previous example, but for the object created from foo.txt, an additional datastream with the DSID 'SOMETHING' will be ingested from the file `foo.SOMETHING.png`.

```
.
├── foo.txt
├── foo.MODS.xml
├── foo.SOMETHING.png
├── bar.txt
├── bar.TN.png
├── baz.txt
```

## Maintainer

* [Mark Jordan](https://github.com/mjordan)

## Development and feedback

Pull requests are welcome, as are use cases and suggestions.

## License

* [GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
