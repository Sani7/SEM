$(function() {
    $.ajax({

        url: 'chart_data.php',
        type: 'GET',
        crossDomain:true,
        success: function(data) {
            chartData = data;
            var chartProperties = {
                "caption": "Fluvius meter data",
                "xAxisName": "Date time",
                "yAxisName": "Data points",
                "numbersuffix": " V",
                "rotatelabels": "1",
                
                "theme": "zune",
            };

            apiChart = new FusionCharts({
                type: 'line',
                renderAt: 'chart-container',
                width: '100%',
                height: '900',
                dataFormat: 'json',
                dataSource: {
                    "chart": chartProperties,
                    "data": chartData
                },
            });
            apiChart.render();
        }
    });
});