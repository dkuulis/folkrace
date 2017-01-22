$fs = 0.25;
$fa = 8;

use <hcsr04.scad>
use <utils.scad>

// horizontal sonar mounting holes
module mount_holes(center = true, left = 2, right = 2)
{
    if (center) translate ([-5,    0, -15]) cylinder(d=3.2, h=15, center=false);

    if (left >= 1) translate ([-5,  9.5, -15]) cylinder(d=3.2, h=15, center=false);
    if (left >= 2) translate ([-5,  19, -15]) cylinder(d=3.2, h=15, center=false);

    if (right >= 1) translate ([-5, -9.5, -15]) cylinder(d=3.2, h=15, center=false);
    if (right >= 2) translate ([-5, -19, -15]) cylinder(d=3.2, h=15, center=false);
}

// sonar mount bracket
module mount(extra = 7) {

    top_r = 3;
    bottom_r = 5;

    // mount plate
    color ("gray") {

        // front
        difference() {

            // front plate
            translate ([1.5, -48/2, -extra])
                #union() {
                    cube(size=[3, 48, 19+extra], center=false);
                    translate ([0, top_r, 0]) cube(size=[3, 48-2*top_r, 19+extra+top_r], center=false);
                    translate ([0, 48-top_r, 19+extra]) rotate([90, 0, 90]) cylinder(r=top_r, h=3, center=false);
                    translate ([0, top_r, 19+extra]) rotate([90, 0, 90]) cylinder(r=top_r, h=3, center=false);
                }

            // hc-sr04 cutouts
            translate ([0, -45/2, 0]) rotate([90, 0, 90]) {

                // sonar holes
                translate([45/2-(42-16)/2,18/2+1,1.3]) cylinder(d=17, h=15);
                translate([45/2+(42-16)/2,18/2+1,1.3]) cylinder(d=17, h=15);

                //xtals holes - bot upside and downwards
                hull () {
                    translate([45/2-3.25,20-3.5,1.3])
                    {
                        translate([0,0,0]) cylinder(d=6, h=5);
                        translate([6.5,0,0]) cylinder(d=6, h=5);
                    }
                    translate([45/2-3.25, 4,1.3]) hull()
                    {
                        translate([0,-2,0]) cylinder(d=6, h=5);
                        translate([6.5,-2,0]) cylinder(d=6, h=5);
                    }
                }

                // screws
                translate([   1.0,    1.0, 4.5-0.5]) screw2x6bore();
                translate([45-1.0, 20-1.5, 4.5-0.5]) screw2x6bore();
                translate([45-1.0,    1.0, 4.5-0.5]) screw2x6bore();
                translate([   1.0, 20-1.5, 4.5-0.5]) screw2x6bore();
            }

            // center cleanup
            translate ([3, 0, 10]) cube(size=[5, 20, 11], center=true);
        }

        //base
        difference() {
            #union() {
                translate ([-3.5, -48/2, -extra]) cube(size=[10-bottom_r, 48, 5], center=false);
                translate ([-3.5-bottom_r, -48/2+5, -extra]) cube(size=[10, 48-2*bottom_r, 5], center=false);
                translate ([-3.5, -48/2+5, -extra]) cylinder(r=bottom_r, h=5, center=false);
                translate ([-3.5, 48/2-5, -extra]) cylinder(r=bottom_r, h=5, center=false);
            }
            mount_holes();
        }
    }
}

// horizantaly mounted sonar
module horizontal_sonar() {

    // horizontal sonar
    translate ([0, -45/2, 0]) rotate([90, 0, 90]) hcsr04();

    mount();

    // sonar ray
    //color("white") translate ([20+1000/2, 0, 10]) rotate([0, 90, 0]) cylinder(d1=40, d2=520,h=1000, center=true);
}

// horizantaly mounted sonar - mockup
module vertical_sonar() {

    // vertical sonar
    translate ([0, -10, 45]) rotate([0, 90, 0]) hcsr04();

    // mount bracket outline
    color("gray") translate ([1.5, -10, -6]) cube(size=[3, 20, 51], center=false);
    color("gray") translate ([-8.5, -10, -6]) cube(size=[10, 20, 5], center=false);

    // sonar ray
    //color("white") translate ([20+1000/2, 0, 22.5]) rotate([0, 90, 0]) cylinder(d1=40, d2=520,h=1000, center=true);
}

//translate ([0, 0, 4.5]) rotate([0, 90, 0]) mount(); // std version
translate ([0, 0, 4.5]) rotate([0, 90, 0]) mount(15); // tall
