$fs = 0.25;
$fa = 8;

module board() {

    translate ([-12.5, 0, 47]) {
        // base
        color("yellow") cube(size=[90, 70, 1.2], center=true);

        // teensy
        color("green") translate ([-12, 0, 5]) cube(size=[63, 18, 10], center=true);

        // MPU-9250
        color("red") translate ([-24, 21, 5]) cube(size=[18.5, 19, 10], center=true);

        // servo conn
        color("black") translate ([27, -21, 1.5]) cube(size=[10, 7.5, 3], center=true);
    }
}

module esc() {
    translate ([33, -8, 79]) rotate([0, 0, 90]) {
        color("green") cube(size=[36, 26, 2], center=true);
        color("black") translate ([36/2, 0, 0]) rotate([0, 90, 0]) cylinder(d=8, h=16, center=false);
    }
}

module lcd() {
    color("blue") translate([-40+2+27.5/2+0.5,0,81]) cube(size=[27.5, 27.5, 2], center=true);
}

board();
esc();
lcd();
