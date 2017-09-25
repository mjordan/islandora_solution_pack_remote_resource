<?php

/**
 * @file
 * Utility functions.
 */

/**
 * Queries Islandora's Solr index to get an object's PID.
 *
 * @param string $remote_url
 *   The remote URL to query Solr for.
 *
 * @return string|bool
 *   The corresponding PID, or FALSE if none is found.
 */
function islandora_remote_resource_batch_query_solr($remote_url) {
  $query = 'OBJ_t:"' . trim($remote_url) . '"';
  $query_processor = new IslandoraSolrQueryProcessor();
  $query_processor->solrQuery = $query;
  $query_processor->solrParams['fl'] = 'PID';
  $query_processor->executeQuery(FALSE);
  // @todo: Check to see if the number found is > 1 and if so,
  // report it and move on.
  if ($query_processor->islandoraSolrResult['response']['numFound'] === 1) {
    $pid = $query_processor->islandoraSolrResult['response']['objects'][0]['solr_doc']['PID'];
    return $pid;
  }
  else {
    return FALSE;
  }
}

/**
 * Applies an XSLT transform to an XML string.
 *
 * @param string $xslt
 *   An XSLT stylesheet.
 * @param string $input_xml
 *   An XML string.
 * @param array $params
 *   An associative array of parameters to the stylesheet.
 *
 * @return string
 *   The transformed XML.
 */
function islandora_remote_resource_batch_apply_xslt($xslt, $input_xml, $params = array()) {
  try {
    $xsl_doc = new DOMDocument();
    $xsl_doc->loadXML($xslt);
    $xml_doc = new DOMDocument();
    $xml_doc->loadXML($input_xml);
    $xslt_proc = new XSLTProcessor();
    $xslt_proc->importStylesheet($xsl_doc);
    if (count($params)) {
      foreach ($params as $param_name => $param_value) {
        $xslt_proc->setParameter(NULL, $param_name, check_plain($param_value));
      }
    }
    $output_xml = $xslt_proc->transformToXML($xml_doc);
    return $output_xml;
  }
  catch (exception $e) {
    $success = array(
      'success' => FALSE,
      'messages' => array(
        array(
          'message' => t('Failed to apply XSL transform.'),
          'type' => 'watchdog',
          'severity' => WATCHDOG_ERROR,
        ),
      ),
    );
  }
}
