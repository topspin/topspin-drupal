README.txt
==========

A module that integrates with Topspin's (http://topspinmedia.com) REST API.
 
Topspin API Documentation: http://docs.topspin.net


PROJECT PAGE
============

http://drupal.org/project/topspin
 
 
REQUIREMENTS
============

* PHP 5.2.x
* jSON enabled for PHP
* cURL enabled for PHP
* A Topspin account with an API key.


FEATURES
========

* Connection API for making requests from the REST API.
  topspin_request($method, $parameters);

* A basic administration form at admin/settings/topspin.
* A default store view as a normal page configurable through the admin/settings/topspin form.
* A default store view available as a block with its own configurations but receives defaults from admin/settings/topspin.
* Offers can be configured as a block from admin/settings/topspin and then enabled in a region from admin/build/block.


AUTHOR/MAINTAINER
=================

* Steve McKenzie

