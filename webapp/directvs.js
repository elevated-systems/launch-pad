'use strict';

/* Directives */


angular.module('launchPad.directives', []).
  directive('highchart', function () {
        return {
            restrict: 'E',
            transclude: true,
            template: '<div></div>',
            replace: true,
            link: function (scope, element, attrs) {
                scope.$watch(attrs.ngModel, function(value) {
                    new Highcharts.Chart({
                        chart: {
                            renderTo: attrs.id,
                            type: attrs.type,
                            height: attrs.height,
                            width: attrs.width,
                            marginRight: attrs.marginright,
                            marginBottom: attrs.marginbottom
                        },
                        title: value.title,
                        subtitle: value.subtitle,
                        xAxis: value.xAxis,
                        yAxis: value.yAxis,
                        tooltip: value.tooltip,
                        legend: value.legend,
                        series: value.series
                    });
                });

            }
        };
    });