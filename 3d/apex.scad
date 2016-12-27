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
        translate ([14, 4, 23]) cube(size=[65, 2, 4], center=false);
        translate ([14, -6, 23]) cube(size=[65, 2, 4], center=false);
        translate ([0, 0, 23/2]) cube(size=[158, 12, 23], center=true);
        translate ([-79, 4, 23]) cube(size=[43, 2, 6], center=false);
        translate ([-79, -6, 23]) cube(size=[43, 2, 6], center=false);        
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
        translate ([79, 0, 0]) rotate([90, 0, 90]) linear_extrude(height = 16)
            polygon([[23,41], [-23,41],[-38,0],[38,0]]);

        // rear shocks
        translate ([-79-16, 0, 0]) rotate([90, 0, 90]) linear_extrude(height = 16)
            polygon([[25,45], [-25,45],[-38,0],[38,0]]);
    }
}

module base_plate() {
    polygon([
        [-79,25], [-70,25],[-54,40],[49,40],[70,23], [79,23],
        [79,-23], [70,-23],[49,-40],[-54,-40],[-70,-25], [-79,-25]
    ]);
}

module platform() {

    color("black") translate ([0, 0, 46]) difference() {
        union() {
            #linear_extrude(height = 3)
                difference() {
                    offset(20) offset(-20) 
                       polygon([
                         [-79,25], [-70,25],[-44,60],[44,60],[70,23], [79,23],
                         [79,-23], [70,-23],[44,-60],[-44,-60],[-70,-25], [-79,-25]
                       ]);
                    
                    offset(5) offset(-10) polygon([
                        [-54,40],[29,40],[29,-40],[-54,-40]
                    ]);
                }
               
            // shock tower mount points
            translate ([-79, -25, 0]) cube(size=[5, 50, 11], center=false);    
            translate ([79-5, -23, 0]) cube(size=[5, 46, 7], center=false);    

            // joint mount plates to body
            translate ([-79, -25, 0]) cube(size=[15, 50, 3], center=false);    
            translate ([79-15, -23, 0]) cube(size=[15, 46, 3], center=false);    
        }
        
        // board mount holes
        #translate ([-54, 31, 0])  cylinder(d=3.2, h=10, center=true);
        #translate ([-54, -31, 0]) cylinder(d=3.2, h=10, center=true);
        #translate ([29, 31, 0])   cylinder(d=3.2, h=10, center=true);
        #translate ([29, -31, 0])  cylinder(d=3.2, h=10, center=true);
    }
}

module board() {
    
    // base
    color("yellow") translate ([-12.5, 0, 45]) cube(size=[90, 70, 1], center=true);    

    // teensy
    color("green") translate ([-25, 0, 45+5]) cube(size=[20, 63, 10], center=true);    
    
    // MPU-9250
    color("red") translate ([0, 15, 45+5]) cube(size=[20, 20, 10], center=true);    
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

module esc() {
    color("green") cube(size=[36, 26, 2], center=true);    
    color("black") translate ([36/2, 0, 0]) rotate([0, 90, 0]) cylinder(d=8, h=16, center=false);

    // esc support box ?
    // hcolor("black") translate ([0, 0, -11]) cube(size=[30, 20, 20], center=true);
}

module hsonar() {

    // horizontal sonar
    translate ([0, -45/2, 0]) rotate([90, 0, 90]) hcsr04();
    
    // mount plate
    color("gray") translate ([1.5, -45/2, -6]) cube(size=[3, 45, 26], center=false);
    color("gray") translate ([-8.5, -45/2, -6]) cube(size=[10, 45, 5], center=false);
    
    // sonar ray
    //color("white") translate ([20+1000/2, 0, 10]) rotate([0, 90, 0]) cylinder(d1=40, d2=520,h=1000, center=true);
}

module car()
{
    base();
    platform();
    board();
    
    //translate ([10, -23, 35]) esc();    
    translate ([-10, 0, 40]) rotate([0, 0, 180]) esc();
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

module fullb() {
    car();
    
    translate ([68, 0, 55]) hsonar();
    translate ([40, 40, 55]) rotate([0, 0, 30]) hsonar();
    translate ([40, -40, 55]) rotate([0, 0, -30]) hsonar();  
    
    translate ([5, 50, 40]) rotate([180, 0, 60]) hsonar();
    translate ([5, -50, 40]) rotate([180, 0, -60]) hsonar();        
}

fullb();

//projection(cut=true) translate ([0, 0, -47]) platform();