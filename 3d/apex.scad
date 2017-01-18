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

// size is the XY plane size, height in Z
module hexagon(size, height) {
  boxWidth = size/1.75;
  for (r = [-60, 0, 60]) rotate([0,0,r]) cube([boxWidth, size, height], true);
}

module platform() {

    difference() {
    color("black") translate ([0, 0, 12+32+4]) difference() {
        union() {
            #linear_extrude(height = 3) intersection() {
                difference() {
                    offset(-10) offset(30) offset(-20)
                       polygon([
                         [-200,25], [-70,25],[-44,60],[34,60],[70,25], [200,25], // extra wide front
                         [200,-25], [70,-25],[34,-60],[-44,-60],[-70,-25], [-200,-25]
                       ]);


                    // central hole
                    translate ([-12.5, 0, 0])
                    offset(-5) offset(10) offset(-5) difference() {

                        polygon([
                         // left
                         [-45,-20], [-31,-37], // bevel [-45,-35],
                         [ 32,-37], [ 45,-27], // bevel [45,-35],

                         [48,-27], [48,27],

                         // right
                         [ 45, 27], [ 32, 37], // bevel [45,35],
                         [-31, 37], [-45, 20]  // bevel [-45,35]
                        ]);

                        //translate ([-45, -35, 0]) circle(r=7.5, center=true);
                        //translate ([-45,  35, 0]) circle(r=7.5, center=true);
                        //translate ([ 45, -35, 0]) circle(r=7.5, center=true);
                        //translate ([ 45,  35, 0]) circle(r=7.5, center=true);
                   }



                }
                square(size=[162,120], center=true); // cut off extra length
            }


            intersection() { translate ([0, 0, 100]) cube(size=[200,200,200], center=true); union() {
            // shock tower mount points
            // rear
            #translate ([-81, -25, 0]) {
                hull() {
                    cube(size=[4, 7, 12-3.5], center=false);
                    translate ([4/2, 3.5, 12-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
                }
                translate ([8/2, 3.5, 12-3.5-6]) rotate([90, 0, 90]) cylinder(d=6, h=8, center=true);
            }
            #translate ([-81, 25-7, 0]) {
                hull() {
                    cube(size=[4, 7, 12-3.5], center=false);
                    translate ([4/2, 7-3.5, 12-3.5]) rotate([90, 0, 90]) cylinder(d=7, h=4, center=true);
                }
                translate ([8/2, 7-3.5, 12-3.5-6]) rotate([90, 0, 90]) cylinder(d=6, h=8, center=true);
            }

            // front
            #translate ([81-6.5, -25, 0]) {
                cube(size=[6.5, 11, 4], center=false);
                translate ([6.5/2, 5.5, 4]) rotate([90, 0, 90]) cylinder(d=11, h=6.5, center=true);
            }
            #translate ([81-6.5, 25-11, 0]) {
                cube(size=[6.5, 11, 4], center=false);
                translate ([6.5/2, 5.5, 4]) rotate([90, 0, 90]) cylinder(d=11, h=6.5, center=true);
            }
            }}
        }

        // wire tie holes
        #translate ([-45, -45, 0]) cylinder(d=5, h=20, center=true);
        #translate ([-45,  45, 0]) cylinder(d=5, h=20, center=true);
        #translate ([-35, -45, 0]) cylinder(d=5, h=20, center=true);
        #translate ([-35,  45, 0]) cylinder(d=5, h=20, center=true);

        // esc power wires
        #hull() {
            translate ([48-12.5-5, 23, 0]) cylinder(d=6, h=20, center=true);
            translate ([54-12.5,   23, 0]) cylinder(d=6, h=20, center=true);
        }

        // esc motor wires
        #hull() {
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
        #hull() {
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
        #translate ([81-4-5/2, 42.5/2-3/2, 8-2-3/2]) rotate([90, 90, 90]) hexagon(5.6, 6);
        // right
        #translate ([81,-42.5/2+3/2, 8-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
        #translate ([81-4-5/2,-42.5/2+3/2, 8-2-3/2]) rotate([90, 90, 90]) hexagon(5.6, 6);

        // rear
        // left
        #translate ([-81, 46.5/2-3/2, 12-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
        #translate ([-81,-46.5/2+3/2, 12-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true);
        // right
        #translate ([-81,-46.5/2+3/2, 12-2-3/2]) rotate([90, 0, 90]) cylinder(d=3, h=20, center=true);
        #translate ([-81, 46.5/2-3/2, 12-2-3/2-6]) rotate([90, 0, 90]) cylinder(d=3, h=12, center=true);
    }
    #arrangebottom(30)
    {
        hbaseholes(center=true, right=1, left=1);
        hbaseholes(center=true, right=0, left=1);
        hbaseholes(center=true, right=0, left=1);
        hbaseholes(center=true, right=1, left=0);
        hbaseholes(center=true, right=1, left=0);
    }
    // cage holes
    #cage(cut=true);

    //arrangebottom(45) hbaseholes();
    }
}

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

module hbaseholes(center = true, left = 2, right = 2)
{
    if (center) translate ([-5,    0, -15]) cylinder(d=3.2, h=15, center=false);

    if (left >= 1) translate ([-5,  9.5, -15]) cylinder(d=3.2, h=15, center=false);
    if (left >= 2) translate ([-5,  19, -15]) cylinder(d=3.2, h=15, center=false);

    if (right >= 1) translate ([-5, -9.5, -15]) cylinder(d=3.2, h=15, center=false);
    if (right >= 2) translate ([-5, -19, -15]) cylinder(d=3.2, h=15, center=false);
}

module hmount() {
    // mount plate
    color ("gray") {

        // front
        difference() {
            translate ([1.5, -48/2, -6]) #hull() {
                cube(size=[3, 48, 25], center=false);
                translate ([0, 48-3, 25]) rotate([90, 0, 90]) cylinder(r=3, h=3, center=false);
                translate ([0, 3, 25]) rotate([90, 0, 90]) cylinder(r=3, h=3, center=false);
            }

            translate ([0, -45/2, 0]) rotate([90, 0, 90]) {

                // sonar
                translate([45/2-(42-16)/2,18/2+1,1.3]) cylinder(d=17, h=15);
                translate([45/2+(42-16)/2,18/2+1,1.3]) cylinder(d=17, h=15);

                //xtal(s)
                hull () {
                    translate([45/2-3.25,20-3.5,1.3])
                    {
                        translate([0,0,0]) cylinder(d=6, h=5);
                        translate([6.5,0,0]) cylinder(d=6, h=5);
                    }
                    translate([45/2-3.25, 4,1.3]) hull()
                    {
                        //translate([0,0,0]) cylinder(d=6, h=5);
                        //translate([6.5,0,0]) cylinder(d=6, h=5);
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

            translate ([3, 0, 10])
                cube(size=[5, 20, 11], center=true);
        }

        //base
        difference() {
            #hull() {
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

module barbox(p1, p2, s=1, r=1) {
    
    a = [p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]];
    l = norm(a);
    b = [a[0]/l, a[1]/l, a[2]/l];
    
    c = cross([0,0,1], b);
    
    az = atan2(c[0], c[1]);
    ax = acos(b[2]);
    
    o = s/2-r;
    s1 = s-2*r;
   
    translate(p1) 
        rotate([0, 0, -az])   
           rotate([0, ax, 0])
               translate([0, 0, l/2]) {
                    cube(size=[s1, s, l], center=true);
                    cube(size=[s, s1, l], center=true);

                    for (x=[o, -o])                   
                        for (y=[o, -o]) {
                            translate([x,y,0]) cylinder(r=r, h=l, center=true);
                            for (z=[l/2, -l/2]) {
                                translate([x,y,z]) sphere(r=r, center=true);
                            }
                        }

                    for (z=[l/2, -l/2]) {
                        translate([0,0,z]) cube(size=[s1, s1, 2*r], center=true);                

                        translate([ 0, o, z]) rotate([0,90,0]) cylinder(r=r, h=s1, center=true);
                        translate([ 0,-o, z]) rotate([0,90,0]) cylinder(r=r, h=s1, center=true);
                        translate([ o, 0, z]) rotate([90,0,0]) cylinder(r=r, h=s1, center=true);
                        translate([-o, 0, z]) rotate([90,0,0]) cylinder(r=r, h=s1, center=true);
                    }
                    
               }
}

module bar(p1, p2, s=1) {
    
    a = [p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]];
    l = norm(a);
    b = [a[0]/l, a[1]/l, a[2]/l];
    
    c = cross([0,0,1], b);
    
    az = atan2(c[0], c[1]);
    ax = acos(b[2]);
    
    translate(p1) sphere(d=s, center=true);
    translate(p2) sphere(d=s, center=true);
   
    translate(p1) 
        rotate([0, 0, -az])   
           rotate([0, ax, 0])
               cylinder(d=s, h=l, center=false);
}

module ball8(p1, s=1, r=1) {

    o = s/2-r;
   
    translate(p1) 
        for (x=[o, -o])                   
            for (y=[o, -o])
                for (z=[o, -o]) 
                    translate([x,y,z])
                        sphere(r=r, center=true);
}

module skew(p1, p2) { // TODO - not working

    a = [p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]];
    b = [a[0]/a[2], a[1]/a[2]];
    
    matrix = [
        [ 1, 0, b[1], 0 ],
        [ 0, 1, b[2], 0 ],
        [ 0, 0, 1, 0 ],
        [ 0, 0, 0, 1 ]
    ];

    translate(p1) 
        multmatrix(matrix) 
                children();
}

module cage_leg(p1, p2, d, db, r)
{
//    bar(p1, p2, d);   
    
    hull() {
        ball8(p2, d, r);
        translate([0, 0, -d/2]) translate(p1) difference() {
            sphere(d=db, center=true);
            translate([0, 0, -db/2]) cube(size=[db, db, db], center=true);
        }
    }
}

module cage_pin(p1, p2, d, pd, ph, cut)
{
    if (cut)
    {
        bar(p1, p2, d+0.5);   
    }
    else
    difference() {
        bar(p1, p2, d);   
        
        translate ([0,0,-ph])
            translate (p1) 
                rotate([90,0,0])
                    cylinder(d=pd, h=2*d, center=true);
    }     
}

module lcd_mount(p1, x1, x2)
{
    translate(p1) difference() {
        #translate([(x1-x2)/2,0,-1]) cube(size=[x1+x2, 6, 2], center=true);
        #cylinder(d=2.2, h=6, center=true);
    }     
}

module cage(cut = false)
{
    
    d = 4;
    r1 = 1;
    n = 4;
    db = 6;
    hb = d;
    pd = 1;
    ph = d/2 + 3 /*platform*/+ 1/*freespace*/;

    // measurements    
    top = 82;
    
    top_side = 30;
    
    top_front = 20;
    top_rear = -40;

    top_esc = top_front-26/*esc size*/+d;
    top_lcd = top_rear+27/*lcd size*/+d+1;
    
    bottom = 51/*platform top*/+d/2;
    
    bottom_front = 27;
    bottom_rear = -50;

    bottom_front_side = 40;
    bottom_rear_side = 40;

    pin = 8;
    pin_base = bottom - pin;

    // points
    top_front_left = [top_front, top_side, top];
    top_front_right = [top_front, -top_side, top];

    top_esc_left = [top_esc, top_side, top];
    top_esc_right = [top_esc, -top_side, top];

    top_lcd_left = [top_lcd, top_side, top];
    top_lcd_right = [top_lcd, -top_side, top];

    top_rear_left = [top_rear, top_side, top];
    top_rear_right = [top_rear, -top_side, top];

    //mid_front_left = [(top_front+bottom_front)/2, (top_side+bottom_front_side)/2, (top+bottom)/2];
    //mid_front_right = [mid_front_left[0], -mid_front_left[1], mid_front_left[2]];

    bottom_front_left = [bottom_front, bottom_front_side, bottom];
    bottom_front_right = [bottom_front, -bottom_front_side, bottom];

    bottom_rear_left = [bottom_rear, bottom_rear_side, bottom];
    bottom_rear_right = [bottom_rear, -bottom_rear_side, bottom];

    pin_front_left = [bottom_front, bottom_front_side, pin_base];
    pin_front_right = [bottom_front, -bottom_front_side, pin_base];

    pin_rear_left = [bottom_rear, bottom_rear_side, pin_base];
    pin_rear_right = [bottom_rear, -bottom_rear_side, pin_base];
     
    // bars
    if (!cut)
    {
        barbox(top_front_left, top_front_right, d, r1);   
        barbox(top_rear_left, top_rear_right, d, r1);   
        barbox(top_front_left, top_rear_left, d, r1);   
        barbox(top_front_right, top_rear_right, d, r1);   

        barbox(top_esc_left, top_esc_right, d, r1);   
        barbox(top_lcd_left, top_lcd_right, d, r1);
        
        lcd_mount([top_rear+d/2+2.5+23+0.5,  23.5/2, top+d/2], 4, 2);
        lcd_mount([top_rear+d/2+2.5+23+0.5, -23.5/2, top+d/2], 4, 2);
        lcd_mount([top_rear+d/2+2.5-0.5,     23.5/2, top+d/2], 2, 4);
        lcd_mount([top_rear+d/2+2.5-0.5,    -23.5/2, top+d/2], 2, 4);

        //bar(mid_front_left, mid_front_right, d);  // unprintable

        cage_leg(bottom_front_left, top_front_left, d, db, r1);
        cage_leg(bottom_front_right, top_front_right, d, db, r1);
        cage_leg(bottom_rear_left, top_rear_left, d, db, r1);
        cage_leg(bottom_rear_right, top_rear_right, d, db, r1);
    }
    
    cage_pin(bottom_front_left, pin_front_left, d, pd, ph, cut);   
    cage_pin(bottom_front_right, pin_front_right, d, pd, ph, cut);   
    cage_pin(bottom_rear_left, pin_rear_left, d, pd, ph, cut);   
    cage_pin(bottom_rear_right, pin_rear_right, d, pd, ph, cut);   
}

module car()
{
    base();
    platform();
    board();
    color("white") cage();

    translate ([9, -8, 79]) rotate([0, 0, 90]) esc();
}

module arrangebottom(a) {
    translate ([68, 0, 57]) if ($children == 1) children(); else children(0);

    translate ([45, 42, 57]) rotate([0, 0, a]) if ($children == 1) children(); else children(1);
    translate ([-5, 45, 57]) rotate([0, 0, 2*a]) if ($children == 1) children(); else children(2);

    translate ([45, -42, 57]) rotate([0, 0, -a]) if ($children == 1) children(); else children(3);
    translate ([-5, -45, 57]) rotate([0, 0, -2*a]) if ($children == 1) children(); else children(4);
}

module lcd1602() {
translate([-15,0,78]) difference() {
    union() {
        translate([0,0,5+8/2]) color("blue") cube(size=[27, 72, 8], center=true);
        translate([0,0,4+1/2]) color("green") cube(size=[36, 80, 1], center=true);
    }
    translate([ 31/2, 75/2,0]) cylinder(d=2.5, h=20, center=false);
    translate([-31/2, 75/2,0]) cylinder(d=2.5, h=20, center=false);
    translate([ 31/2,-75/2,0]) cylinder(d=2.5, h=20, center=false);
    translate([-31/2,-75/2,0]) cylinder(d=2.5, h=20, center=false);
}
}

module lcd() {
    color("blue") translate([-40+2+27.5/2+0.5,0,81]) cube(size=[27.5, 27.5, 2], center=true);
}

module fullb() {
    car();
    arrangebottom(30) hsonar();
    cage();
    lcd();
}

//car();

//cage();


//board();
//platform();

//fullb();


//barbox([0,0,0], [0,0,50], 10, 3);

//translate ([0, 0, 4.5]) rotate([0, 90, 0]) hmount();


/* // small front
intersection() {
    platform();
    translate ([80, -15, 0]) cube(size=[40, 35, 400], center=true);
}*/

/* // full front
intersection() {
    platform();
    #translate ([100+20, 0, 0]) cube(size=[200, 200, 400], center=true);
}*/

/* // rear
intersection() {
    platform();
    translate ([-80, 0, 0]) cube(size=[20, 200, 400], center=true);
}*/

//screw2x6bore();

//rotate([0,0,180]) projection(cut=true) translate ([0, 0, -49]) platform();
//projection() translate ([0, 0, 10]) hsonar();

translate([0,0,90]) rotate([180,0,0]) cage();
