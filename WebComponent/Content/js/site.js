// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.

// Panel on Sideline Navbar

window.addEventListener('load', main, false);

/*

let index_js = $('script[src*=index]')

let HangSeng = index_js.attr('data-HangSeng')
let Stoxx50 = index_js.attr('data-Stoxx50')
let SP500 = index_js.attr('data-SP500')

var obj =   JSON.parse(HangSeng)

//
// Price chart
//

var PriceChart = (function () {

    // Variables

    var $chart = $('#kanji-price');


    // Methods

    function init($chart) {

        var priceChart = new Chart($chart, {
            type: 'line',
            options: {
                scales: {
                    yAxes: [{
                        gridLines: {
                            lineWidth: 1,
                            color: Charts.colors.gray[900],
                            zeroLineColor: Charts.colors.gray[900]
                        },
                        ticks: {
                            callback: function (value) {
                                if (!(value % 10)) {
                                    return '$' + value + 'k';
                                }
                            }
                        }
                    }]
                },
                tooltips: {
                    callbacks: {
                        label: function (item, data) {
                            var label = data.datasets[item.datasetIndex].label || '';
                            var yLabel = item.yLabel;
                            var content = '';

                            if (data.datasets.length > 1) {
                                content += '<span class="popover-body-label mr-auto">' + label + '</span>';
                            }

                            content += '<span class="popover-body-value">$' + yLabel + 'k</span>';
                            return content;
                        }
                    }
                }
            },
            data: {
                labels: ['May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'],
                datasets: [{
                    label: 'Performance',
                    data: [60, 20, 10, 30, 15, 40, 20, 60, 60]
                }]
            }
        });

        // Save to jQuery object

        $chart.data('chart', priceChart);

    };


    // Events

    if ($chart.length) {
        init($chart);
    }

})();
*/