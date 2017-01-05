//$fs = 0.1;
//$fa = 5;
$fs = 0.25;
$fa = 8;

use <hcsr04.scad>

module wheel() {
    color("black") rotate([90, 90, 0]) difference() {
        #cylinder(d=47, h=23, center=true);
        cylinder(d=30, h=26, center=true);
    }
}

module base() {

    // 4 wheels
    translate([77.5, 60, 47/2]) wheel();
    translate([-77.5, 60, 47/2]) wheel();
    translate([77.5, -60, 47/2]) wheel();
    translate([-77.5, -60, 47/2]) wheel();

    // front axe assemby - slightly longer
    translate([77.5-5, 0, 47/2]) color("gray") cube(size=[25, 103, 26], center=true);
    // rear axe assemby
    translate([-77.5, 0, 47/2]) color("gray") cube(size=[26, 97, 26], center=true);

    color("gray") translate ([0, 0, 12]) {

        // base plate
        linear_extrude(height = 2) base_plate();
        linear_extrude(height = 7) difference() {
            base_plate();
            offset(-2) base_plate();
        }

        // shaft
        translate ([0, 0, 23/2]) cube(size=[158, 12, 23], center=true);
        //shaft ribs
        translate ([14, 4, 23]) cube(size=[67, 2, 4], center=false);
        translate ([14, -6, 23]) cube(size=[67, 2, 4], center=false);
        translate ([0, 0, 23/2]) cube(size=[158, 12, 23], center=true);
        translate ([-81, 4, 23]) cube(size=[45, 2, 6], center=false);
        translate ([-81, -6, 23]) cube(size=[45, 2, 6], center=false);
        // servo
        linear_extrude(height = 15)  polygon([
            [29,0], [79,0],
            [79,-23], [70,-23],[49,-40],[29,-40]
        ]);

        // motor
        translate ([-79+30, -16, 23/2+2]) rotate([0, 90, 0]) cylinder(d=23, h=32, center=false);

        // gear
        translate ([-79+23, 0, 34/2]) rotate([0, 90, 0]) cylinder(d=34, h=6, center=false);

        // battery
        translate ([-5/2, 30/2+7, 16/2+2]) cube(size=[84, 30, 16], center=true);

        // battery holder
        translate ([-79+23, 20, 0]) cylinder(d=7, h=26, center=false);
        translate ([79-29, 20, 0]) cylinder(d=7, h=26, center=false);
        translate ([-5/2, 20, 22]) cube(size=[110, 13, 2], center=true);

        // front shocks
        translate ([162/2, 0, 0]) rotate([90, 0, 90]) linear_extrude(height = 16)
            polygon([[23,44],[18,48],[5,41],[-5,41],[-18,48],[-23,44], [-38,0],[38,0]]);

        // rear shocks
        translate ([-162/2-16, 0, 0]) rotate([90, 0, 90]) linear_extrude(height = 16)
            polygon([[25,48],[20,48],[5,38],[-5,38],[-20,48],[-25,48], [-38,0],[38,0]]);
    }
}

module base_plate() {
    polygon([
        [-79,25], [-70,25],[-54,40],[49,40],[70,23], [79,23],
        [79,-23], [70,-23],[49,-40],[-54,-40],[-70,-25], [-79,-25]
    ]);
}

module platform() {

    difference() {
    color("black") translate ([0, 0, 12+32+4]) difference() {
        union() {
            #linear_extrude(height = 3) intersection() {
                difference() {
                    offset(-10) offset(30) offset(-20)
                       polygon([
                         [-200,25], [-70,25],[-44,60],[44,60],[70,23], [200,23],
                         [200,-23], [70,-23],[44,-60],[-44,-60],[-70,-25], [-200,-25]
                       ]);

                    offset(5) offset(-5) translate ([-2.5, 0, 0]) difference() {
                        polygon([
                         [-50,-35], [50,-35],
                           [50,-30], [60,-15],[60,15], [50,30],
                         [50,35], [-50,35],
                           [-50,30], [-55,20],[-55,-20], [-50,-30]
                        ])

                        square(size=[90,70], center=true);
                        translate ([-45, -35, 0]) circle(r=7.5, center=true);
                        translate ([ 45, -35, 0]) circle(r=7.5, center=true);
                        translate ([-45,  35, 0]) circle(r=7.5, center=true);
                        translate ([ 45,  35, 0]) circle(r=7.5, center=true);
                    }

                }
                square(size=[162,120], center=true);
            }


            intersection() { translate ([0, 0, 100]) cube(size=[200,200,200], center=true); union() {
            // shock tower mount points
            // rear
            #translate ([-81, -25, 0]) { hull() {
                cube(size=[4, 7, 12-3.5], center=false);
                translate ([4/2, 3.5, 12-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
            }
            translate ([8/2, 3.5, 12-3.5-6]) rotate([90, 0, 90]) cylinder(d=6, h=8, center=true);
            }
            #translate ([-81, 25-7, 0]) { hull() {
                cube(size=[4, 7, 12-3.5], center=false);
                translate ([4/2, 7-3.5, 12-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
            }
            translate ([8/2, 7-3.5, 12-3.5-6]) rotate([90, 0, 90]) cylinder(d=6, h=8, center=true);
            }
            // front
            #translate ([81-4, -46/2, 0]) hull() {
                cube(size=[4, 7, 8-3.5], center=false);
                translate ([4/2, 3.5, 8-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
            }
            #translate ([81-4, 46/2-7, 0]) hull() {
                cube(size=[4, 7, 8-3.5], center=false);
                translate ([4/2, 7-3.5, 8-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
            }
            }}
        }

        // board mount holes
        translate ([-(45-2.5)-2.5,  (35-2.5), 2.5]) screw2x6bore(); // cylinder(d=2.2, h=10, center=true);
        translate ([-(45-2.5)-2.5, -(35-2.5), 2.5]) screw2x6bore(); //cylinder(d=2.2, h=10, center=true);
        translate ([ (45-2.5)-2.5,  (35-2.5), 2.5]) screw2x6bore(); //cylinder(d=2.2, h=10, center=true);
        translate ([ (45-2.5)-2.5, -(35-2.5), 2.5]) screw2x6bore(); //cylinder(d=2.2, h=10, center=true);

        // servo wire hole
        //translate ([55, -20, 0]) cylinder(d=10, h=10, center=true);
        //translate ([55,  20, 0]) cylinder(d=10, h=10, center=true);

        // mount holes
        // front
        #translate ([81, 42.5/2-3/2, 8-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
        #translate ([81,-42.5/2+3/2, 8-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
        #translate ([81, 42.5/2-3/2, 8-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true);
        #translate ([81,-42.5/2+3/2, 8-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true);

        #translate ([81-4-5/2, 42.5/2-3/2, 8-2-3/2]) rotate([90, 0, 90]) cylinder(d=6.1, h=5, center=true);
        #translate ([81-4-5/2,-42.5/2+3/2, 8-2-3/2]) rotate([90, 0, 90]) cylinder(d=6.1, h=5, center=true);

        // rear
        #translate ([-81, 46.5/2-3/2, 12-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
        #translate ([-81,-46.5/2+3/2, 12-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
        #translate ([-81, 46.5/2-3/2, 12-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true);
        #translate ([-81,-46.5/2+3/2, 12-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true);
    }
    arrangebottom(30) hbaseholes(center=true);
    //arrangebottom(45) hbaseholes();
    }
}

module board() {

    // base
    color("yellow") translate ([-2.5, 0, 47]) cube(size=[90, 70, 1.2], center=true);

    // teensy
    color("green") translate ([-25, 0, 47+5]) cube(size=[20, 63, 10], center=true);

    // MPU-9250
    color("red") translate ([0, 15, 47+5]) cube(size=[20, 20, 10], center=true);
}


module esc() {
    color("green") cube(size=[36, 26, 2], center=true);
    color("black") translate ([36/2, 0, 0]) rotate([0, 90, 0]) cylinder(d=8, h=16, center=false);

    // esc support box ?
    // hcolor("black") translate ([0, 0, -11]) cube(size=[30, 20, 20], center=true);
}

module vsonar() {

    // vertical sonar
    translate ([0, -10, 45]) rotate([0, 90, 0]) hcsr04();

    // mount plate
    color("gray") translate ([1.5, -10, -6]) cube(size=[3, 20, 51], center=false);
    color("gray") translate ([-8.5, -10, -6]) cube(size=[10, 20, 5], center=false);

    // sonar ray
    //color("white") translate ([20+1000/2, 0, 22.5]) rotate([0, 90, 0]) cylinder(d1=40, d2=520,h=1000, center=true);
}

module hbaseholes(center = false)
{
    translate ([-5,   0, -15]) cylinder(d=3.2, h=15, center=false);
    if (!center) {
        translate ([-5,  19, -15]) cylinder(d=3.2, h=15, center=false);
        translate ([-5, -19, -15]) cylinder(d=3.2, h=15, center=false);
    }
}

module hmount() {
    // mount plate
    color ("gray") {

        // front
        difference() {
            translate ([1.5, -48/2, -6]) #hull() {
                cube(size=[3, 48, 23], center=false);
                translate ([0, 43, 23]) rotate([90, 0, 90]) cylinder(r=5, h=3, center=false);
                translate ([0, 5, 23]) rotate([90, 0, 90]) cylinder(r=5, h=3, center=false);
            }

            translate ([0, -45/2, 0]) rotate([90, 0, 90]) {

                // sonar
                translate([16/2+1,18/2+1,1.3]) cylinder(d=17, h=15);
                translate([16/2+27+1,18/2+1,1.3]) cylinder(d=17, h=15);

                //xtal(s)
                translate([45/2-3.25,20-4,1.3]) hull()
                {
                    translate([0,0,0]) cylinder(d=6, h=5);
                    translate([6.5,0,0]) cylinder(d=6, h=5);
                }
                translate([45/2-3.25, 4,1.3]) hull()
                {
                    translate([0,0,0]) cylinder(d=6, h=5);
                    translate([6.5,0,0]) cylinder(d=6, h=5);
                    translate([0,-2,0]) cylinder(d=6, h=5);
                    translate([6.5,-2,0]) cylinder(d=6, h=5);                }

                // screws
                translate([   1.5,    1.5, 4.5-0.5]) screw2x6bore();
                translate([45-1.5, 20-1.5, 4.5-0.5]) screw2x6bore();
                translate([45-1.5,    1.5, 4.5-0.5]) screw2x6bore();
                translate([   1.5, 20-1.5, 4.5-0.5]) screw2x6bore();
            }

            translate ([3, 0, 10])
                cube(size=[5, 20, 11], center=true);
        }

        //base
        difference() {
            hull() {
                translate ([-3.5, -48/2, -6]) cube(size=[5, 48, 5], center=false);
                translate ([-3.5, -48/2+5, -6]) cylinder(r=5, h=5, center=false);
                translate ([-3.5, 48/2-5, -6]) cylinder(r=5, h=5, center=false);
            }
            hbaseholes();
        }
    }
}

module screw2x6bore() {
    rotate([180, 0, 0]) {
        cylinder(d=2.2, h=6);
        translate ([0, 0, -1.3])
            cylinder(d1=2*3.6, d2=0, h=2*1.3);
    }
}

module hsonar() {

    // horizontal sonar
    translate ([0, -45/2, 0]) rotate([90, 0, 90]) hcsr04();

    hmount();

    // sonar ray
    //color("white") translate ([20+1000/2, 0, 10]) rotate([0, 90, 0]) cylinder(d1=40, d2=520,h=1000, center=true);
}

module car()
{
    base();
    platform();
    board();

    translate ([10, -23, 35]) esc();
//    translate ([-10, 0, 40]) rotate([0, 0, 180]) esc();
}

// full model - vertical sonars
module fullv() {
    car();

    translate ([68, 0, 55]) vsonar();
    translate ([60, 25, 55]) rotate([0, 0, 30]) vsonar();
    translate ([60, -25, 55]) rotate([0, 0, -30]) vsonar();
    translate ([35, 35, 55]) rotate([0, 0, 60]) vsonar();
    translate ([35, -35, 55]) rotate([0, 0, -60]) vsonar();
}

// full model - horizontal sonars
module fullh() {
    car();

    translate ([68, 0, 55]) hsonar();
    translate ([40, 35, 55]) rotate([0, 0, 30]) hsonar();
    translate ([40, -35, 55]) rotate([0, 0, -30]) hsonar();
    translate ([-10, 35, 55]) rotate([0, 0, 60]) hsonar();
    translate ([-10, -35, 55]) rotate([0, 0, -60]) hsonar();
}

module arrangebottom(a) {
    translate ([68, 0, 57]) children();
    translate ([45, 45, 57]) rotate([0, 0, a]) children();
    translate ([45, -45, 57]) rotate([0, 0, -a]) children();

    translate ([0, 52.5, 42]) rotate([180, 0, 2*a]) children();
    translate ([0, -52.5, 42]) rotate([180, 0, -2*a]) children();
}

module fullb() {
    car();
    arrangebottom(30) hsonar();
}

//car();

platform();

/*
intersection() {
platform();
#translate ([80, 0, 0]) cube(size=[20, 200, 400], center=true);
}*/

/*
intersection() {
platform();
#translate ([-80, 0, 0]) cube(size=[20, 200, 400], center=true);
}*/

/*
intersection() {
platform();
#translate ([45-2.5, 30, 0]) cube(size=[20, 20, 400], center=true);
}
*/

//fullb();

//translate ([0, 0, 4.5]) rotate([0, 90, 0]) hmount();

//screw2x6bore();

//projection(cut=true) translate ([0, 0, -47]) platform();