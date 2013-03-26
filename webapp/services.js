'use strict';

/* Services */


// Demonstrate how to register services
// In this case it is a simple value service.
angular.module('launchPad.services', ['ngResource']).
  factory('$api', function($resource){
		return $resource('/api');
	});