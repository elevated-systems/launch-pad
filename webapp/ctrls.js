'use strict';

/* Controllers */


function DataCtrl($scope, $api) {
	$scope.readingArray = [];
	setInterval(function(){
		$api.get(function(response){
			//Take a sensor reading and carry out a mV->degK->degF conversion
			//First grab it as mV from the API
			var a0ValMV = response.analogChannels["0"];
			//On to DegK
			var a0ValDegK = a0ValMV/10;
			//And finally DegF
			var a0ValDegF = ((a0ValDegK-273.15)*1.8+32);
			var now = new Date();
			$scope.readingArray.unshift(
				{
					timestamp: now,
					valueMV: a0ValMV,
					valueDegK: a0ValDegK,
					valueDegF: a0ValDegF
				}
			);
			});
	}, 2000);
}
DataCtrl.$inject = ["$scope", "$api"];
