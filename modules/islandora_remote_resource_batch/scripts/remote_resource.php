<?php

/**
 * MIK post-write hook script that modifies MIK output to use the naming
 * conventions used by the Islandora Remote Resouce Batch module.
 *
 * To use this script, add it to MIK's extras/scripts/postwritehooks directory
 * and register it at the end of the [WRITER]postwritehooks[] section of your
 * MIK .ini file like this:
 *
 * postwritehooks[] = "php extras/scripts/postwritehooks/remote_resource.php"
 *
 * This script works with MIK OAI toolchains that harvest either MODS or DC,
 * and a thumbnail, for each object.
 */

require 'vendor/autoload.php';

use Monolog\Logger;
use Monolog\Handler\StreamHandler;
use mik\writers\Oaipmh;

$record_key = trim($argv[1]);
$children_record_keys = explode(',', $argv[2]);
$config_path = trim($argv[3]);
$config = parse_ini_file($config_path, true);

// Set up logging.
$path_to_error_log = $config['WRITER']['output_directory'] .
    DIRECTORY_SEPARATOR . 'postwritehook_remote_resource_error.log';

$error_log = new Logger('postwritehooks/remote_resource.php');
$error_handler = new StreamHandler($path_to_error_log, Logger::WARNING);
$error_log->pushHandler($error_handler);

// Instantiate the MIK writer so we can reuse its normalizeFilename() method.
$writer = new Oaipmh($config);

/**
 * Main script logic.
 */

$normalized_record_key = $writer->normalizeFilename($record_key);

if (isset($config['FETCHER']['metadata_prefix'])) {
    if ($config['FETCHER']['metadata_prefix'] == 'oai_dc') {
        $metadata_dsid = 'DC';
    }
    if ($config['FETCHER']['metadata_prefix'] == 'mods') {
        $metadata_dsid = 'MODS';
    }
}
else {
    // If the metadata_prefix isnt' specified, use DC.
    $metadata_dsid = 'DC';
}

// Rename the .xml file to .DC.xml, .MODS.xml, etc.
$oai_metadata_path = $config['WRITER']['output_directory'] . DIRECTORY_SEPARATOR .  $normalized_record_key . '.xml';
$oai_metadata_pathinfo = pathinfo($oai_metadata_path);
$dest_oai_metadata_path = $config['WRITER']['output_directory'] . DIRECTORY_SEPARATOR . $oai_metadata_pathinfo['filename'] .
    '.' . $metadata_dsid . '.' . $oai_metadata_pathinfo['extension'];
if (!rename($oai_metadata_path, $dest_oai_metadata_path)) {
    $error_log->addWarning("Problem with metadata file",
        array('Source file' => $oai_metadata_path, 'Dest file' => $dest_oai_metadata_path, 'Error' => 'Could not rename metadata file.'));
}

// Rename the .jpeg file to .TN.jpeg.
$oai_tn_path = $config['WRITER']['output_directory'] . DIRECTORY_SEPARATOR .  $normalized_record_key . '.jpeg';
$oai_tn_pathinfo = pathinfo($oai_tn_path);
$dest_oai_tn_path = $config['WRITER']['output_directory'] . DIRECTORY_SEPARATOR . $oai_tn_pathinfo['filename'] .
    '.TN.' . $oai_tn_pathinfo['extension'];
if (!rename($oai_tn_path, $dest_oai_tn_path)) {
    $error_log->addWarning("Problem with TN file",
        array('Source file' => $oai_tn_path, 'Dest file' => $dest_oai_tn_path,'Error' => 'Could not rename TN file.'));
}

// Get the cached, raw OAI record from the temp directory so we can pick out the OAI <identifier>.
try {
    $raw_metadata_path = $config['FETCHER']['temp_directory'] . DIRECTORY_SEPARATOR . $record_key . '.metadata';
    $dom = new \DOMDocument;
    $xml = file_get_contents($raw_metadata_path);
    $dom->loadXML($xml);
    $identifier = $dom->getElementsByTagNameNS('http://www.openarchives.org/OAI/2.0/', 'identifier')->item(0);
}
catch (Exception $e) {
    $error_log->addWarning("Problem with OAI-DC to MODS transform",
        array('MODS file' => $mods_path, 'Error' => $e-getMessage()));
}

// There will only be one oai:identifer element. Islandora's OAI identifiers look like
// oai:digital.lib.sfu.ca:foo_112, 'foo_123' being the object's PID.
$raw_pid = preg_replace('#.*:#', '', trim($identifier->nodeValue));
$pid = preg_replace('/_/', ':', $raw_pid);

// Get strings that make up the Islandora instance's host plus port.
$islandora_url_info = parse_url($config['FETCHER']['oai_endpoint']);
if (isset($islandora_url_info['port'])) {
    $port = $islandora_url_info['port'];
} else {
    $port = '';
}
$islandora_host = $islandora_url_info['scheme'] . '://' . $islandora_url_info['host'] . $port;
$object_url = $islandora_host . '/islandora/object/' . $pid;

$obj_path = $config['WRITER']['output_directory'] . DIRECTORY_SEPARATOR .  $normalized_record_key . '.txt';
file_put_contents($obj_path, $object_url);
