// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.

// Panel on Sideline Navbar

window.addEventListener('load', main, false);

// Draw graph of kanji's price on sidebars
function createGraph() {
    var ctx = document.getElementById('kanji-price').getContext('2d');
    var kanjiPrice = new Chart(ctx, {
        type: 'line',
        data: data,
        options: options
    });
}

function main() {
    // Draw graph of kanji's price over time
    createGraph();
}
