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
    arrange(30) {
        horizontal_sonar(15, false);
        horizontal_sonar(7, true);
        horizontal_sonar(7, true );
        horizontal_sonar(7, false);
        horizontal_sonar(7, false);
    }
    color("white") cage(mode=2/*simple*/);
    esc();
    lcd();
}

all();
