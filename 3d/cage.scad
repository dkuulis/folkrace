$fs = 0.25;
$fa = 8;

use <utils.scad>

// rounded box from p1 to p2, size (~diameter) s, rounding radius r
module rounded_box(p1, p2, s=1, r=1) {

    a = [p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]];
    l = norm(a);
    b = [a[0]/l, a[1]/l, a[2]/l];

    l1 = l+s-2*r;

    c = cross([0,0,1], b);

    az = atan2(c[0], c[1]);
    ax = acos(b[2]);

    o = s/2-r;
    s1 = s-2*r;

    translate(p1)
        rotate([0, 0, -az])
           rotate([0, ax, 0])
               translate([0, 0, l/2]) {
                    cube(size=[s1, s, l1], center=true);
                    cube(size=[s, s1, l1], center=true);

                    for (x=[o, -o])
                        for (y=[o, -o]) {
                            translate([x,y,0]) cylinder(r=r, h=l1, center=true);
                            for (z=[l1/2, -l1/2]) {
                                translate([x,y,z]) sphere(r=r, center=true);
                            }
                        }

                    for (z=[l1/2, -l1/2]) {
                        translate([0,0,z]) cube(size=[s1, s1, 2*r], center=true);

                        translate([ 0, o, z]) rotate([0,90,0]) cylinder(r=r, h=s1, center=true);
                        translate([ 0,-o, z]) rotate([0,90,0]) cylinder(r=r, h=s1, center=true);
                        translate([ o, 0, z]) rotate([90,0,0]) cylinder(r=r, h=s1, center=true);
                        translate([-o, 0, z]) rotate([90,0,0]) cylinder(r=r, h=s1, center=true);
                    }

               }
}

// rounded cylindrical rod p1 to p2, diameter s
module rod(p1, p2, s=1) {

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

// 8 balls of radius r, arranged in bounding box for rounded cybe of size s centered on p
module ball8(p, s=1, r=1) {

    o = s/2-r;

    translate(p)
        for (x=[o, -o])
            for (y=[o, -o])
                for (z=[o, -o])
                    translate([x,y,z])
                        sphere(r=r, center=true);
}

// cage leg, from base p1 to top p2, size s, base size base_s, rounding radius s, mode - simple=2 or full otherwise
module cage_leg(p1, p2, s, base_s, r, mode)
{
    if (mode == 2 /*simple*/) {
        rod(p1, p2, s);
    }
    else {
        hull() {
            ball8(p2, s, r);
            translate([0, 0, -s/2]) translate(p1) difference() { // halfsphere of d = base_s
                sphere(d=base_s, center=true);
                translate([0, 0, -base_s/2]) cube(size=[base_s, base_s, base_s], center=true);
            }
        }
    }
}

// cage mount pin, from base p1 to below p1, size d (bit larger for cuts), pin diameter pd, pin heigh ph, mode - 1 for cutout
module cage_pin(p1, p2, d, pd, ph, mode)
{
    if (mode == 1 /*cut*/)
    {
        rod(p1, p2, d+0.5); // extra spacing
    }
    else // full or simple mode
    difference() {
        #rod(p1, p2, d);

        translate ([0,0,-ph])
            translate (p1)
                rotate([0,90,0])
                    cylinder(d=pd, h=2*d, center=true); // pin hole
    }
}

// lcd mount bracket for cage
module lcd_mount(p, x1, x2)
{
    translate(p) difference() {
        #translate([(x1-x2)/2,0,-1-0.5/2]) cube(size=[x1+x2, 6, 2.5], center=true);
        #translate([0,0,-0.5]) screw2x6bore();
    }
}

// cage
module cage(mode = 0)
{
    // mode 0 - full, mode 1 - cutout for platform, 2 - simple full

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

    top_esc = top_front+26/*esc size*/;
    top_lcd = top_rear+27/*lcd size*/+d+1;

    bottom = 51/*platform top*/+d/2;

    bottom_front = 27;
    bottom_rear = -50;

    bottom_front_side = 40;
    bottom_rear_side = 40;

    pin = 8;
    pin_base = bottom - pin;

    mid = (top+bottom)/2;
    mid_side = 15;
    mid_front = top_esc+10;

    // points
    top_front_left = [top_front, top_side, top];
    top_front_right = [top_front, -top_side, top];

    top_esc_left = [top_esc, top_side, top];
    top_esc_right = [top_esc, -top_side, top];

    top_lcd_left = [top_lcd, top_side, top];
    top_lcd_right = [top_lcd, -top_side, top];

    top_rear_left = [top_rear, top_side, top];
    top_rear_right = [top_rear, -top_side, top];

    mid_front_left = [mid_front, mid_side, mid];
    mid_front_right = [mid_front_left[0], -mid_front_left[1], mid_front_left[2]];

    bottom_front_left = [bottom_front, bottom_front_side, bottom];
    bottom_front_right = [bottom_front, -bottom_front_side, bottom];

    bottom_rear_left = [bottom_rear, bottom_rear_side, bottom];
    bottom_rear_right = [bottom_rear, -bottom_rear_side, bottom];

    pin_front_left = [bottom_front, bottom_front_side, pin_base];
    pin_front_right = [bottom_front, -bottom_front_side, pin_base];

    pin_rear_left = [bottom_rear, bottom_rear_side, pin_base];
    pin_rear_right = [bottom_rear, -bottom_rear_side, pin_base];

    // cage bars
    if (mode != 1 /*cut*/)
    {
        rounded_box(top_front_left, top_front_right, d, r1);
        rounded_box(top_rear_left, top_rear_right, d, r1);
        rounded_box(top_esc_left, top_rear_left, d, r1);
        rounded_box(top_esc_right, top_rear_right, d, r1);

        rounded_box(top_esc_left, top_esc_right, d, r1);
        rounded_box(top_lcd_left, top_lcd_right, d, r1);

        rod(mid_front_left, top_esc_left, d);
        rod(mid_front_right, top_esc_right, d);

        lcd_mount([top_rear+d/2+2.5+23+0.5,  23.5/2, top+d/2], 4, 2);
        lcd_mount([top_rear+d/2+2.5+23+0.5, -23.5/2, top+d/2], 4, 2);
        lcd_mount([top_rear+d/2+2.5-0.5,     23.5/2, top+d/2], 2, 4);
        lcd_mount([top_rear+d/2+2.5-0.5,    -23.5/2, top+d/2], 2, 4);

        cage_leg(bottom_front_left, top_front_left, d, db, r1, mode);
        cage_leg(bottom_front_right, top_front_right, d, db, r1, mode);
        cage_leg(bottom_rear_left, top_rear_left, d, db, r1, mode);
        cage_leg(bottom_rear_right, top_rear_right, d, db, r1, mode);

        // lcd lead protector
        translate([top_lcd+1,0,top-3]) cube(size=[2, 12, 6], center=true);
        translate([top_lcd+1,0,top-5]) cube(size=[2, 16, 2], center=true);
    }

    cage_pin(bottom_front_left, pin_front_left, d, pd, ph, mode);
    cage_pin(bottom_front_right, pin_front_right, d, pd, ph, mode);
    cage_pin(bottom_rear_left, pin_rear_left, d, pd, ph, mode);
    cage_pin(bottom_rear_right, pin_rear_right, d, pd, ph, mode);
}

translate([0,0,90]) rotate([180,0,0]) cage();
