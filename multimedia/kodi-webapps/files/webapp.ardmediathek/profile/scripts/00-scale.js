// SPDX-License-Identifier: GPL-2.0-or-later
// The TV edition lays itself out for 1280x720; scale it to the view.

document.documentElement.style.zoom = String(window.innerWidth / 1280);
