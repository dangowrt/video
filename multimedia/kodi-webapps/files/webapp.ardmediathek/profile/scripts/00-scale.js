// SPDX-License-Identifier: GPL-2.0-or-later
// The TV edition lays itself out for 1280x720; fit that into the view
// whatever its size and aspect ratio, centred, as an HbbTV terminal does.

(function () {
    function fit() {
        var root = document.documentElement;
        var scale = Math.min(window.innerWidth / 1280, window.innerHeight / 720);

        if (!root || !scale)
            return;
        root.style.background = "#000";
        root.style.zoom = String(scale);
        root.style.position = "relative";
        root.style.left = (window.innerWidth / scale - 1280) / 2 + "px";
        root.style.top = (window.innerHeight / scale - 720) / 2 + "px";
    }

    fit();
    document.addEventListener("DOMContentLoaded", fit);
    window.addEventListener("resize", fit);
})();
