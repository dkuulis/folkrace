$fs = 0.25;
$fa = 8;

// model wheel
module wheel() {
    color("black") rotate([90, 90, 0]) difference() {
        #cylinder(d=47, h=23, center=true);
        cylinder(d=30, h=26, center=true);
    }
}

// car base plate outline
module base_plate() {
    polygon([
        [-79,25], [-70,25],[-54,40],[49,40],[70,23], [79,23],
        [79,-23], [70,-23],[49,-40],[-54,-40],[-70,-25], [-79,-25]
    ]);
}

// base car
module apex() {

    // 4 wheels
    translate([77.5, 60, 47/2]) wheel();
    translate([-77.5, 60, 47/2]) wheel();
    translate([77.5, -60, 47/2]) wheel();
    translate([-77.5, -60, 47/2]) wheel();

    // front axe assemby - slightly longer than rear
    translate([77.5-5, 0, 47/2]) color("gray") cube(size=[25, 103, 26], center=true);

    // rear axe assemby
    translate([-77.5, 0, 47/2]) color("gray") cube(size=[26, 97, 26], center=true);

    color("gray") translate ([0, 0, 12]) {

        // base plate
        linear_extrude(height = 2) base_plate();
        linear_extrude(height = 7) // extra lip around base
            difference() {
                base_plate();
                offset(-2) base_plate();
            }

        // shaft assembly
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

        // main gear
        translate ([-79+23, 0, 34/2]) rotate([0, 90, 0]) cylinder(d=34, h=6, center=false);

        // battery
        translate ([-5/2, 30/2+7, 16/2+2]) cube(size=[84, 30, 16], center=true);

        // battery holder
        translate ([-79+23, 20, 0]) cylinder(d=7, h=26, center=false);
        translate ([79-29, 20, 0]) cylinder(d=7, h=26, center=false);
        translate ([-5/2, 20, 22]) cube(size=[110, 13, 2], center=true);

        // front shocks and tower
        translate ([162/2, 0, 0]) rotate([90, 0, 90]) linear_extrude(height = 16)
            polygon([[23,44],[18,48],[5,41],[-5,41],[-18,48],[-23,44], [-38,0],[38,0]]);

        // rear shocks and tower
        translate ([-162/2-16, 0, 0]) rotate([90, 0, 90]) linear_extrude(height = 16)
            polygon([[25,48],[20,48],[5,38],[-5,38],[-20,48],[-25,48], [-38,0],[38,0]]);
    }
}

apex();
