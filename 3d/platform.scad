$fs = 0.25;
$fa = 8;

use <utils.scad>
use <parts.scad>
use <mount.scad>
use <cage.scad>
use <apex.scad>

// arange sonars (or mount holes)
module arrange(a) {
    translate ([68, 0, 58]) if ($children == 1) children(); else children(0);

    translate ([45, 42, 58]) rotate([0, 0, a]) if ($children == 1) children(); else children(1);
    translate ([-5, 45, 58]) rotate([0, 0, 2*a]) if ($children == 1) children(); else children(2);

    translate ([45, -42, 58]) rotate([0, 0, -a]) if ($children == 1) children(); else children(3);
    translate ([-5, -45, 58]) rotate([0, 0, -2*a]) if ($children == 1) children(); else children(4);
}

// 3d printed platform to be attached to shock towers
module platform() {

    color("black") difference()
    {
        translate ([0, 0, 12+32+4])
            difference()
            {
                union()
                {

                    // basic plate
                    #linear_extrude(height = 3)
                        difference()
                        {
                            intersection()
                            {
                                offset(-10) offset(30) offset(-20) // rounded corners
                                   polygon([
                                     [-200,25], [-70,25],[-44,60],[34,60],[70,25], [200,25], // extra wide front
                                     [200,-25], [70,-25],[34,-60],[-44,-60],[-70,-25], [-200,-25]
                                   ]);
                               square(size=[162,200], center=true); // cut off extra length
                            }

                            // central hole
                            translate ([-12.5, 0, 0])
                                offset(-5) offset(10) offset(-5) // rounded corners
                                    polygon([
                                     // left
                                     [-45,-20], [-31,-37], // bevel around [-45,-35],
                                     [ 32,-37], [ 45,-27], // bevel around [45,-35],

                                     [48,-27], [48,27],

                                     // right
                                     [ 45, 27], [ 32, 37], // bevel around [45,35],
                                     [-31, 37], [-45, 20]  // bevel around [-45,35]
                                    ]);

                            // wire tie holes
                            #translate ([-45, -45, 0]) circle(d=5);
                            #translate ([-45,  45, 0]) circle(d=5);
                            #translate ([-35, -45, 0]) circle(d=5);
                            #translate ([-35,  45, 0]) circle(d=5);
                        }

                    intersection()
                    {
                        #union()
                        { // shock tower mount points

                            // rear
                            translate ([-81, -25, 0]) {
                                hull() {
                                    cube(size=[4, 7, 12-3.5], center=false);
                                    translate ([4/2, 3.5, 12-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
                                }
                                translate ([8/2, 3.5, 12-3.5-6]) rotate([90, 0, 90]) cylinder(d=6, h=8, center=true);
                            }

                            translate ([-81, 25-7, 0]) {
                                hull() {
                                    cube(size=[4, 7, 12-3.5], center=false);
                                    translate ([4/2, 7-3.5, 12-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
                                }
                                translate ([8/2, 7-3.5, 12-3.5-6]) rotate([90, 0, 90]) cylinder(d=6, h=8, center=true);
                            }

                            // front
                            translate ([81-6.5, -25, 0]) {
                                cube(size=[6.5, 11, 4], center=false);
                                translate ([6.5/2, 5.5, 4]) rotate([90, 0, 90]) cylinder(d=11, h=6.5, center=true);
                            }

                            translate ([81-6.5, 25-11, 0]) {
                                cube(size=[6.5, 11, 4], center=false);
                                translate ([6.5/2, 5.5, 4]) rotate([90, 0, 90]) cylinder(d=11, h=6.5, center=true);
                            }
                        }
                        translate ([0, 0, 100]) cube(size=[200,200,200], center=true);  // cut off bottom parts from all cylynders
                    }
                }

                // local positioned cutouts

                // esc power wires
                #hull()
                {
                    translate ([48-12.5-5, 23, 0]) cylinder(d=6, h=20, center=true);
                    translate ([54-12.5,   23, 0]) cylinder(d=6, h=20, center=true);
                }

                // esc motor wires
                #hull()
                {
                    translate ([48-12.5-5, -23,   0]) cylinder(d=6, h=20, center=true);
                    translate ([54-12.5,   -23+4, 0]) cylinder(d=6, h=20, center=true);
                    translate ([54-12.5,   -23,   0]) cylinder(d=6, h=20, center=true);
                    translate ([48-12.5,   -23+4, 0]) cylinder(d=6, h=20, center=true);
                }

                // space save
                #hull() {
                    translate ([52, -10, 0]) cylinder(d=5, h=20, center=true);
                    translate ([42, -10, 0]) cylinder(d=5, h=20, center=true);
                    translate ([42,  10, 0]) cylinder(d=5, h=20, center=true);
                    translate ([52,  10, 0]) cylinder(d=5, h=20, center=true);
                }

                // space save
                #hull()
                {
                    translate ([-72, -10, 0]) cylinder(d=5, h=20, center=true);
                    translate ([-67, -10, 0]) cylinder(d=5, h=20, center=true);
                    translate ([-67,  10, 0]) cylinder(d=5, h=20, center=true);
                    translate ([-72,  10, 0]) cylinder(d=5, h=20, center=true);
                }

                // board mount holes
                #translate ([-(45-2.5)-10,  (35-2.5), 2.5]) screw2x6bore();
                #translate ([-(45-2.5)-10, -(35-2.5), 2.5]) screw2x6bore();
                #translate ([ (45-2.5)-10,  (35-2.5), 2.5]) screw2x6bore();
                #translate ([ (45-2.5)-10, -(35-2.5), 2.5]) screw2x6bore();

                // mount holes
                // front
                // M3 hex nut in front H=2.4, Dmin = 5.4
                // left
                #translate ([81, 42.5/2-3/2, 8-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
                #translate ([81-4-5/2, 42.5/2-3/2, 8-2-3/2]) rotate([90, 90, 90]) hexagon(5.5, 6);
                // right
                #translate ([81,-42.5/2+3/2, 8-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
                #translate ([81-4-5/2,-42.5/2+3/2, 8-2-3/2]) rotate([90, 90, 90]) hexagon(5.5, 6);

                // rear
                // left
                #translate ([-81, 46.5/2-3/2, 12-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true); // top thru hole
                #translate ([-81,-46.5/2+3/2, 12-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true); // bottom hole for pins? or unused
                // right
                #translate ([-81,-46.5/2+3/2, 12-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true); // top thru hole
                #translate ([-81, 46.5/2-3/2, 12-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true); // bottom hole for pins? or unused
            }

        // global positioned coutouts

        // sonar mount hole cutouts
        #arrange(30)
        {
            mount_holes(center=true, right=1, left=1);
            mount_holes(center=true, right=0, left=1);
            mount_holes(center=true, right=0, left=1);
            mount_holes(center=true, right=1, left=0);
            mount_holes(center=true, right=1, left=0);
        }

        // cage hole cutout
        #cage(mode=1/*cut*/);
    }
}

platform();
