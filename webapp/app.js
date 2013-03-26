'use strict';

// Declare app level module which depends on services and directives
angular.module('launchPad', ['ngResource', 'launchPad.services', 'launchPad.directives']).
  config(['$routeProvider', function($routeProvider) {
    $routeProvider.when('/data', {templateUrl: 'data.htm', controller: DataCtrl});
    $routeProvider.otherwise({redirectTo: '/data'});
  }]);