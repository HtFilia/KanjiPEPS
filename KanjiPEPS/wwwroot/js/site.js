// Please see documentation at https://docs.microsoft.com/aspnet/core/client-side/bundling-and-minification
// for details on configuring this project to bundle and minify static web assets.

// Write your JavaScript code.

// Panel on Sideline Navbar

window.addEventListener('load', main, false);

function activateLink() {
    var parent = $(this).parent();
    if (!parent[0].classList.contains("active")) {
        parent.parent().children().each(function () {
            $(this).removeClass("active");
        });
        parent.addClass("active");
    }
}

// Print current date on index page
function printDate() {
    n = new Date();
    y = n.getFullYear();
    m = n.getMonth();
    d = n.getDate();
    document.getElementById("date").innerHTML = ("0" + d).slice(-2) + "/" + ("0" + (m + 1)).slice(-2) + "/" + y;
}

function main() {
    // Event on click on link on sidebars
    $("body").on('click', '.nav-link', activateLink);
    // Print current date on index page
    printDate();
}