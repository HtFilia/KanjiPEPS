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

*/

function main() {
    let site_js = $('script[src*=site]')

    let KanjiQuot = site_js.attr('data-HangSeng')
    let KanjiHebdo = site_js.attr('data-SP500')
    let KanjiMens = site_js.attr('data-Stoxx50')
}