$fs = 0.25;
$fa = 8;

use <utils.scad>
use <parts.scad>
use <mount.scad>
use <cage.scad>
use <apex.scad>
use <platform.scad>

// whole car
module all()
{
    apex();
    platform();
    board();
    arrange(30) horizontal_sonar();
    color("white") cage(mode=2/*simple*/);
    translate ([33, -8, 79]) rotate([0, 0, 90]) esc();
    lcd();
}

all();
