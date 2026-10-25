// SPDX-License-Identifier: GPL-2.0-or-later
// The HbbTV edition hands a stream to an OIPF A/V object, which the web view
// does not implement; the video element plays DASH and HLS natively.

document.addEventListener("DOMContentLoaded", function () {
    if (window.GLOBALS)
        window.GLOBALS.hbbtvhtmlfive = 1;
});
